#include "network.h"

/*
 Reads n bytes from a given descriptor
 */
ssize_t readn(int fd, void *vptr, ssize_t n) {
  size_t nleft = n;
  ssize_t nread;
  char *ptr;

  vptr = ptr;
  
  while (nleft > 0) {
    if ((nread = read(fd, ptr, nleft) < 0)) {
      if (errno == EINTR)
	nread = 0; //Intertrrupted. Call read again.
      else
	return (-1);
    } else if (nread == 0)
      break;

    nleft -= nread;
    ptr += nread;
  }

  return (n - nleft);
}


