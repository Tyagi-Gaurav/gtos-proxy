#include "network.h"

/*
  Description: Reads upto a maximum of `MAXLEN` characters from the stream.
  Each time this function is called, it reads a character from the pre-read stream
  and returns it to the caller. 

  Parameters: 
  1. sockfd: File descriptor from which characters have to be read.
  2. ch: Pointer to the character that should have the data.
  3. read_buf: Buffer to hold upto `MAXLEN` characters.
 */
ssize_t read_char(int sockfd, char *ch, char* read_buf, int *c_count, int n) {
  char *ptr;
  int rc = *c_count;

  ptr = ch;

  //printf("Initial rc: %d\n", rc);
  if (rc <= 0) {
  again:
    rc = read(sockfd, read_buf, sizeof(read_buf)); //Read upto MAXLINE characters
    //printf("Return code after reading. %d\n", rc);
    
    if (errno == EINTR) {
      //printf("Interrupted.\n");
      goto again;
    }
    if (rc < 0) return -1; //Failed to read
    if (rc == 0) return 0; //Nothing to read

    *c_count = rc;
  }

  if (rc > 0) {
    //printf("Next char: %c\n", *(read_buf+n));
    *ptr = *(read_buf+n);
    --*c_count;
  }

  return (1);
}

ssize_t readline(int sockfd, char* vptr, int maxline) {
  char *ptr;
  int n = 0, rc, total = 0;
  char read_buf[MAXLINE];
  char c;

  ptr = vptr;

  for (int i=0; i < maxline;++i) {
    //In each iteration, read a single character.
    //printf("Iteration %d\n", i);
    rc = read_char(sockfd, &c, read_buf, &n, i); 
    if (rc == 1) {
      //printf("Charcter read: %c\n", c);
      *ptr++ = c;
      total++;
      if (c == '\n') break;
    } else if (rc == 0) {
      //printf("Rc: 0\n");
      *ptr = 0;
      return (i - 1);
    } else {
      //printf("Rc: -1\n");
      return (-1);
    }
  }

  *ptr = 0;
  return (total);
}

void str_cli(FILE *fp, int sockfd) {
  char send_line[MAXLINE];
  char recv_line[MAXLINE];

  while (fgets(send_line, MAXLINE, fp) != NULL) {
    writen(sockfd, send_line, strlen(send_line));

    if (readline(sockfd, recv_line, MAXLINE) == 0)
      err_quit("str_cli: server terminated prematurely");

    fputs(recv_line, stdout);
  }
}
