#include "network.h"

#define SERV_PORT 9877

void str_echo(int sockfd) {
  ssize_t n;
  char buf[MAXLINE];

  /*
    `read` would read data from the socket and the line is echoed back to the client using writen. If the client closes the connection (the normal scenario), the receipt of the client's FIN causes the `read` to return 0.
   */
 again:
  while ((n = read(sockfd, buf, MAXLINE)) > 0)
    writen(sockfd, (void*)buf, n);

  if (n < 0 && errno == EINTR) //EINTR: Interrupted function call.
    goto again;
  else if (n < 0)
    err_sys("str_echo: read_error");
}

int main(int argc, char **argv) {
  /*
    Listen File Descriptor and Connection File Descriptor.
   */
  int listenfd, connfd, pid, errno;

  socklen_t clilen;
  /*
    Standard Socket Address Structure.
   */
  struct sockaddr_in servaddr, cliaddr;
  
  /*
    Create a socket. At this point its not listening on any 
    interface.
   */
  listenfd = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));

  /*
    Constants like INADDR_xxx defined in <netinet/in.h> are in 
    host byte order. We must use htonl to convert them to network 
    byte order.
   */
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  /*
    bind - Provide an interface on which we could potentially listen.
   */
  bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

  /*
    Now Listen.
   */
  listen(listenfd, LISTENQ);

  for (; ;) {
    clilen = sizeof(cliaddr);
    printf("Listening...\n");
    /*
      New connection descriptor received after each accept call. Convert the listening socket into an accept socket.
     */
    connfd = accept(listenfd, (SA *) NULL, NULL);

    printf("Accepted new connection.\n");

    if ((pid = fork()) == 0) {
      printf("Fork of child successful.\n");
      close(listenfd); //Close listenfd. Reduce its reference count.
      str_echo(connfd); //Process the request.
      exit(0);
    }

    close(connfd);
  }
}
