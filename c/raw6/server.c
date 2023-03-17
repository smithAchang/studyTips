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
#endif


int main(int argc , char* argv[])
{
  char szErrno[256];

  printf("Hello World!\n");

  int tRawUDPSock = socket(PF_INET6, SOCK_RAW, IPPROTO_UDP);

  if(tRawUDPSock < 0)
  {
    strerror_r(errno, szErrno, sizeof(szErrno));	  
    printf("create raw udp protocol sock errno: %d -> %s\n", errno, szErrno);
    return -1;
  }

  printf("create raw udp protocol sock successfully, got sock fd: %d\n", tRawUDPSock);

  struct sockaddr_in6 from = {}, to = {};
  from.sin6_family = to.sin6_family = AF_INET6;
  inet_pton(PF_INET6, "FEC0:2021::192.168.200.34", &to.sin6_addr);
  

  if(argc > 1 )
  {
    
      printf("change server listen addr to %s\n", argv[1]);
      inet_pton(PF_INET6, argv[1], &to.sin6_addr);
  }
  
 
  int rc = bind(tRawUDPSock, (struct sockaddr*)&to, sizeof(to));
  if(rc < 0)
  {
    char    szToAddrBuf[64] = {};
    inet_ntop(AF_INET6, &to.sin6_addr, szToAddrBuf, sizeof(szToAddrBuf));
    strerror_r(errno, szErrno, sizeof(szErrno));	  
    printf("bind addr: %s , errno: %d -> %s\n", szToAddrBuf, errno, szErrno);
    close(tRawUDPSock);
    
    return -1;
  }
  printf("bind successfully !\n");

  uint8_t abPktInfo[4096]  = {};
  char    szFmtTimeBuf[64] = {};
  char    szSrcAddrBuf[64] = {};

  
  int no = 0;
  printf("will enter recv loop !\n");

  while(1)
  {
     socklen_t addrlen = sizeof(from);
     ssize_t  len = recvfrom(tRawUDPSock, abPktInfo, sizeof(abPktInfo), 0 /*flags*/, (struct sockaddr*)&from, &addrlen);
     
     if(len < 0)
     {
        strerror_r(errno, szErrno, sizeof(szErrno));    
        printf("recvfrom in failure, rc: %zd, errno: %d -> %s\n", len, errno, szErrno);
        break;
     }

     time_t cur = time(NULL);
     strftime(szFmtTimeBuf, sizeof(szFmtTimeBuf), "%Y-%m-%d %H:%M:%S", localtime(&cur));

     UDP_HEADER_t *ptUDPHdr               = (UDP_HEADER_t*)(abPktInfo + sizeof(IPv6_HEADER_t));
     UDP_HEADER_t *ptUDPHdr_NO_IPHDRINCL  = (UDP_HEADER_t*)(abPktInfo);
     inet_ntop(AF_INET6, &from.sin6_addr, szSrcAddrBuf, sizeof(szSrcAddrBuf));
     printf("%s recv from peer: %s, len: %zd with IPPROTO_IPV6 setting: %d . src port: %u, dst port: %u;  no ip header included src port: %u, dst port: %u\n", 
                        szFmtTimeBuf, 
                        szSrcAddrBuf, len, no,
                        ntohs(ptUDPHdr->u16SrcPort), ntohs(ptUDPHdr->u16DstPort),
                        ntohs(ptUDPHdr_NO_IPHDRINCL->u16SrcPort), ntohs(ptUDPHdr_NO_IPHDRINCL->u16DstPort));

     no = !no;
     printf("change IPPROTO_IPV6 setting: %d\n", no);
     rc = setsockopt(tRawUDPSock, IPPROTO_IPV6, IPV6_HDRINCL, (const void*)&no ,sizeof(no));
     if(rc < 0)
     {
        strerror_r(errno, szErrno, sizeof(szErrno));    
        printf("setsockopt IPV6_HDRINCL in failure errno: %d -> %s\n", errno, szErrno);
     }


  }
  
  printf("test program exit!\n"); 
  close(tRawUDPSock);

  return 0;
}
