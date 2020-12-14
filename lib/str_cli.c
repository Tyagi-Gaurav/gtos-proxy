#include "network.h"

ssize_t readline(int sockfd, const char* vptr, int maxline) {
  return 1;
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
