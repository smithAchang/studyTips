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
  inet_pton(PF_INET6, "FEC0:2021::192.168.31.1", &from.sin6_addr);
  inet_pton(PF_INET6, "FEC0:2021::192.168.31.2", &to.sin6_addr);

  if(argc > 1 )
  {
    if(argc < 3)
    {
      printf("change dst addr to %s\n", argv[1]);
      inet_pton(PF_INET6, argv[1], &to.sin6_addr);
    }
    else
    {
      printf("change src addr to %s\n", argv[1]);
      inet_pton(PF_INET6, argv[1], &from.sin6_addr);

      printf("change dst addr to %s\n", argv[2]);
      inet_pton(PF_INET6, argv[2], &to.sin6_addr);
    }
  }
  
  //old bugs
  //from.sin6_port = htons(IPPROTO_UDP);
  int rc = bind(tRawUDPSock, (struct sockaddr*)&from, sizeof(from));
  if(rc < 0)
  {
    char    szToAddrBuf[64] = {};
    inet_ntop(AF_INET6, &from.sin6_addr, szToAddrBuf, sizeof(szToAddrBuf));
    strerror_r(errno, szErrno, sizeof(szErrno));    
    printf("bind addr: %s , errno: %d -> %s\n", szToAddrBuf, errno, szErrno);

    close(tRawUDPSock);
    return -1;
  }
  printf("bind successfully !\n");

  uint8_t abPktInfo[4096] = {};
  uint16_t  u16ContentLen = 2049;
  from.sin6_port = 0;
  to.sin6_port   = 0;

  IPv6_HEADER_t* ptIPv6_header = (IPv6_HEADER_t*)abPktInfo;
  memset(ptIPv6_header, 0, sizeof(*ptIPv6_header));

  ptIPv6_header->abVTF[0]      = 0x60;
  ptIPv6_header->u16PayloadLen = htons(sizeof(UDP_HEADER_t) + u16ContentLen); 
  ptIPv6_header->bNextHeader   = IPPROTO_UDP; 
  ptIPv6_header->bHopLimit     = 128; 
  memcpy(ptIPv6_header->abSrcAddr, &from.sin6_addr, sizeof(from.sin6_addr));
  memcpy(ptIPv6_header->abDstAddr, &to.sin6_addr, sizeof(to.sin6_addr));

  UDP_HEADER_t* ptUdpHdr = (UDP_HEADER_t*)(abPktInfo + sizeof(IPv6_HEADER_t));

  ptUdpHdr->u16SrcPort   = htons(12760);    
  ptUdpHdr->u16DstPort   = htons(22760);   
  ptUdpHdr->u16Length    = htons(sizeof(UDP_HEADER_t) + u16ContentLen);   
  ptUdpHdr->u16CheckSum  = 0;

  size_t reqLen = sizeof(UDP_HEADER_t) + u16ContentLen;
  printf("will send %zu bytes\n", reqLen);
  ssize_t sendLen = sendto(tRawUDPSock, ptUdpHdr, reqLen, 0/*flags*/, (const struct sockaddr*)&to,  sizeof(to));
 
  if(sendLen < 0)
  {
    strerror_r(errno, szErrno, sizeof(szErrno));	  
    printf("send data errno: %d -> %s\n", errno, szErrno);
  }
  else
  {
    printf("has send data %zd bytes!\n", sendLen); 
  }
  
  
  
  printf("test program exit!\n"); 
  close(tRawUDPSock);

  return 0;
}
