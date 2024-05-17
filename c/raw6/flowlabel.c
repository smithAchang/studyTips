#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>


#include "header.h"


#ifndef IPV6_HDRINCL
#warning "The env has not defined IPV6_HDRINCL marco"
#define IPV6_HDRINCL 36
#endif /* ifndef IPV6_HDRINCL */


int main(int argc, char *argv[])
{
  char szErrno[256];
  char szFromAddrBuf[64] = {};
  char szToAddrBuf[64]   = {};

  printf("Hello World!\n");

  int tRawUDPSock = socket(PF_INET6, SOCK_DGRAM, 0);

  if (tRawUDPSock < 0)
  {
    strerror_r(errno, szErrno, sizeof(szErrno));
    printf("create raw udp protocol sock errno: %d -> %s\n", errno, szErrno);
    return -1;
  }

  printf("create raw udp protocol sock successfully, got sock fd: %d\n", tRawUDPSock);

  struct sockaddr_in6 from = {}, to = {};
  from.sin6_family = to.sin6_family = AF_INET6;

  int rc = bind(tRawUDPSock, (struct sockaddr *)&to, sizeof(to));

  if (rc < 0)
  {
    inet_ntop(AF_INET6, &to.sin6_addr, szToAddrBuf, sizeof(szToAddrBuf));
    strerror_r(errno, szErrno, sizeof(szErrno));
    printf("bind addr: %s , errno: %d -> %s\n", szToAddrBuf, errno, szErrno);
    close(tRawUDPSock);

    return -1;
  }

  inet_ntop(AF_INET6, &from.sin6_addr, szFromAddrBuf, sizeof(szFromAddrBuf));
  socklen_t addrlen = sizeof(from);
  getsockname(tRawUDPSock, (struct sockaddr *)&from, &addrlen);
  printf("bind successfully wildcard addr: %s, port: %u !\n", szFromAddrBuf, ntohs(from.sin6_port));

  to = from;

  to.sin6_flowinfo = htons(1234);

  sendto(tRawUDPSock, "hello world", sizeof("hello world"), 0, (struct sockaddr *)&to, sizeof(to));

  char recvBuf[256];

  memset(&from, 0, sizeof(from));
  addrlen = sizeof(from);
  rc      = recvfrom(tRawUDPSock, recvBuf, sizeof(recvBuf), 0, (struct sockaddr *)&from, &addrlen);

  printf("recv %d bytes, flowinfo: %u\n", rc, ntohs(to.sin6_flowinfo));


  printf("test program exit!\n");
  close(tRawUDPSock);

  return 0;
}
