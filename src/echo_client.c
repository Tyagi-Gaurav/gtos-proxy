#include "network.h"

#define SERV_PORT 9877

extern void str_cli(FILE*, int);

int main(int argc, char **argv) {
  int sockfd;
  struct sockaddr_in serv_addr;

  if (argc != 2)
    err_quit("usage: tcpcli <IPaddress>");

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(SERV_PORT);

  // This is needed because we read server's IP address from command line. 
  inet_pton(AF_INET, argv[1], &serv_addr.sin_addr); //Presentation to network

  connect(sockfd, (SA *)&serv_addr, sizeof(serv_addr));

  str_cli(stdin, sockfd);
  exit(0);
}
