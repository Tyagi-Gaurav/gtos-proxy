#include "network.h"

void print_socket_info(struct sockaddr_in *addr) {
  char buf[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, (void *)&addr->sin_addr, buf, sizeof(buf));
  printf("Family: %d , Port: %d, Address: %s\n",
  	 addr->sin_family,
  	 ntohs(addr->sin_port),
	 buf);
}

void err_quit(const char* fmt, ...) { //Variable arguments
  va_list valist;

  va_start(valist, fmt); //Initialise valist with fmt strings
  err_print(0, LOG_ERR, fmt, valist);
  va_end(valist); //Remove memory reserved for valist
  return;
}

void err_print(int errnoflag, int level, const char* fmt, va_list ap) {
  int errno_save, n;
  char buf[MAXLINE + 1];

  errno_save = errnoflag; //This variable is never used  ??????
  #ifdef HAVE_VSNPRINTF
  vsnprintf(buf, MAXLINE, fmt, ap);
  #else
  vsprintf(buf, fmt, ap);
  #endif

  n = strlen(buf);
  if (errnoflag) {
    syslog(level, "%s", buf); //Log into syslog
  } else {
    fflush(stdout);
    fputs(buf, stderr);
    fflush(stderr);
  }
}

/*
  Fatal error related to system call. Print message and terminate.
*/
void err_sys(const char* fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  err_print(1, LOG_ERR, fmt, ap);
  va_end(ap);
  exit(1);
}
