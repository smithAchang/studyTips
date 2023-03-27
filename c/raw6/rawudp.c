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
  char    szFromAddrBuf[64] = {};
  char    szToAddrBuf[64]   = {};

  printf("Hello World!\n");
  struct sockaddr_in6 from = {}, to = {};
  from.sin6_family = to.sin6_family = AF_INET6;
  inet_pton(PF_INET6, "FEC0:2021::192.168.31.1", &from.sin6_addr);
  inet_pton(PF_INET6, "FEC0:2021::192.168.31.2", &to.sin6_addr);

  int iovec_flag, opt;
  uint16_t  u16ContentLen = 2049;
  iovec_flag = 0;

  while ((opt = getopt(argc, argv, "vs:d:l:")) != -1) {
     switch (opt) {
     case 'v':
         iovec_flag = 1;
         break;
     case 's':
          printf("change src addr to %s\n", optarg);
         inet_pton(PF_INET6, optarg, &from.sin6_addr);
         break;
     case 'l':
         u16ContentLen = atoi(optarg);        
         break;
     case 'd':
         printf("change dst addr to %s\n", optarg);
         inet_pton(PF_INET6, optarg, &to.sin6_addr);
         break;
     default: /* '?' */
         fprintf(stderr, "Usage: %s [-s src] [-d dst] [-l len] [-v] name\n",
                 argv[0]);
         return 1;
     }
  }


  inet_ntop(AF_INET6, &from.sin6_addr, szFromAddrBuf, sizeof(szFromAddrBuf));
  inet_ntop(AF_INET6, &to.sin6_addr,   szToAddrBuf, sizeof(szToAddrBuf));

  printf("src=%s; dst=%s; u16ContentLen=%u; iovec_flag=%d; optind=%d\n",
         szFromAddrBuf, szToAddrBuf, u16ContentLen, iovec_flag, optind);


  if (optind >= argc) {
     fprintf(stderr, "can Expected argument after options\n");
  }


  int tRawUDPSock = socket(PF_INET6, SOCK_RAW, IPPROTO_UDP);

  if(tRawUDPSock < 0)
  {
    strerror_r(errno, szErrno, sizeof(szErrno));	  
    printf("create raw udp protocol sock errno: %d -> %s\n", errno, szErrno);
    return -1;
  }

  printf("create raw udp protocol sock successfully, got sock fd: %d\n", tRawUDPSock);

   
  //old bugs
  //from.sin6_port = htons(IPPROTO_UDP);
  int rc = bind(tRawUDPSock, (struct sockaddr*)&from, sizeof(from));
  if(rc < 0)
  {
    strerror_r(errno, szErrno, sizeof(szErrno));    
    printf("bind addr: %s , errno: %d -> %s\n", szFromAddrBuf, errno, szErrno);

    close(tRawUDPSock);
    return -1;
  }
  printf("bind successfully !\n");

  uint8_t abPktInfo[4096] = {};
 
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
  ssize_t sendLen ;
  if(iovec_flag)
  {
    struct sockaddr_in6 iovec_to = to;
    iovec_to.sin6_port           = 1234;

    struct iovec udpvec[2];
    udpvec[0].iov_base = ptUdpHdr;
    udpvec[0].iov_len  = sizeof(UDP_HEADER_t);
    udpvec[1].iov_base = "hello world";
    udpvec[1].iov_len  = sizeof("hello world");
    ptUdpHdr->u16Length    = htons(sizeof(UDP_HEADER_t) + udpvec[1].iov_len);   

    struct msghdr msghdr  = {};
    msghdr.msg_name       = (void*)&iovec_to;       /* Optional address */
    msghdr.msg_namelen    = sizeof(iovec_to);    /* Size of address */
    msghdr.msg_iov        = udpvec;        /* Scatter/gather array */
    msghdr.msg_iovlen     = sizeof(udpvec)/sizeof(udpvec[0]);     /* # elements in msg_iov */
    msghdr.msg_control    = 0;    /* Ancillary data, see below */
    msghdr.msg_controllen = 0; /* Ancillary data buffer len */
    msghdr.msg_flags      = 0;      /* Flags (unused) */


    sendLen = sendmsg(tRawUDPSock, &msghdr, 0/*flags*/);
    
    if (sendLen < 0) {
     strerror_r(errno, szErrno, sizeof(szErrno));   
     fprintf(stderr, "send fail when sin6_port is not zero; errno:%d -> %s\n", errno, szErrno);
    }

    iovec_to.sin6_port           = 0;
    sendLen = sendmsg(tRawUDPSock, &msghdr, 0/*flags*/);
  }
  else
  {
    sendLen = sendto(tRawUDPSock, ptUdpHdr, reqLen, 0/*flags*/, (const struct sockaddr*)&to,  sizeof(to));
  }
 
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
