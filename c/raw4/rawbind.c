#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>  
#include <sys/socket.h>
#include <arpa/inet.h>

#include "header.h"


int main(int argc , char* argv[])
{
  char szErrno[256];

  printf("Hello World!\n");

  int tRawUDPSock = socket(PF_INET, SOCK_RAW, IPPROTO_UDP);

  if(tRawUDPSock < 0)
  {
    strerror_r(errno, szErrno, sizeof(szErrno));	  
    printf("create raw udp protocol sock errno: %d -> %s\n", errno, szErrno);
    return -1;
  }

  int tRebindRawUDPSock = socket(PF_INET, SOCK_RAW, IPPROTO_UDP);

  if(tRebindRawUDPSock < 0)
  {
    strerror_r(errno, szErrno, sizeof(szErrno));	  
    printf("create rebind raw udp protocol sock errno: %d -> %s\n", errno, szErrno);
    return -1;
  }

  printf("create raw udp protocol sock successfully, got sock fd: %d\n", tRawUDPSock);

  struct sockaddr_in from = {};
  from.sin_family = AF_INET;
  inet_pton(PF_INET, "192.168.2.9", &from.sin_addr);


  if(argc > 1 )
  {
      printf("change bind addr to %s\n", argv[1]);
      inet_pton(PF_INET, argv[1], &from.sin_addr);
  }
  
 
  int rc = bind(tRawUDPSock, (struct sockaddr*)&from, sizeof(from));
  if(rc < 0)
  {
    char    szToAddrBuf[64] = {};
    inet_ntop(AF_INET, &from.sin_addr, szToAddrBuf, sizeof(szToAddrBuf));
    strerror_r(errno, szErrno, sizeof(szErrno));    
    printf("bind addr: %s , errno: %d -> %s\n", szToAddrBuf, errno, szErrno);

    close(tRawUDPSock);
    close(tRebindRawUDPSock);
    return -1;
  }
  printf("bind successfully !\n");

  rc = bind(tRebindRawUDPSock, (struct sockaddr*)&from, sizeof(from));
  if(rc < 0)
  {
    char    szToAddrBuf[64] = {};
    inet_ntop(AF_INET, &from.sin_addr, szToAddrBuf, sizeof(szToAddrBuf));
    strerror_r(errno, szErrno, sizeof(szErrno));    
    printf("rebind addr: %s , errno: %d -> %s\n", szToAddrBuf, errno, szErrno);

    close(tRawUDPSock);
    close(tRebindRawUDPSock);
    return -1;
  }
  printf("rebind successfully !\n");

 
  
  
  printf("test program exit!\n"); 
  close(tRawUDPSock);
  close(tRebindRawUDPSock);

  return 0;
}
