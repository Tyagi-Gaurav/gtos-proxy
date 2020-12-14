#include "network.h"

/*
  Writes n bytes of data to the file descriptor.
  ssize_t is used for functions whose return value could either be a valid size, or a negative value to indicate an error. ... So you should use size_t whenever you mean to return a size in bytes, and ssize_t whenever you would return either a size in bytes or a (negative) error value.
 */
ssize_t writen(int fd, const void *vptr, size_t n) {
  size_t nleft;
  ssize_t nwritten;
  const char *ptr;

  nleft = n;
  ptr = vptr;
  while (nleft > 0) {
    if ((nwritten = write(fd, ptr, nleft)) <= 0) {
      if (nwritten < 0 && errno == EINTR) nwritten = 0;
      else if (nwritten < 0) return -1;
    }

    nleft -= nwritten;
    ptr += nwritten;
  }

  return (n);
}
