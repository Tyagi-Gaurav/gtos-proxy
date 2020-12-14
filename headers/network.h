#ifndef __NETWORK_H
#define __NETWORK_H

#include <sys/socket.h> /* Socket Definitions */
#include <sys/types.h> /* System data types */
#include <sys/time.h> /* timeval{} for select() */
#include <stdarg.h>
#include <netinet/in.h> /* sockaddr_in{} and other Internet definitions */
#include <syslog.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h> /* inet functions*/
#include <unistd.h>
#include <errno.h>
#define SA struct sockaddr //Generic address structure. Protocol addresesses need to be cast into generic types.

#define LISTENQ 1024
#define MAXLINE 4096
//Do we have 
#define HAVE_VSNPRINTF 1

#define bzero(ptr, n) memset(ptr, 0, n)

extern void err_sys(const char* fmt, ...);

extern void err_quit(const char* fmt, ...);

extern ssize_t writen(int, const void *, size_t);

extern ssize_t readn(int, void *, ssize_t);

extern void err_print(int, int, const char*, va_list);

extern void print_socket_info(struct sockaddr_in *addr);

#endif
