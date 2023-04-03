#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>  
#include <sys/socket.h>
#include <arpa/inet.h>

#include "header.h"

void sendV6(int tRawUDPSock, struct sockaddr_in6 from, struct sockaddr_in6 to, int iovec_flag, uint16_t  u16ContentLen);
void sendV4(int tRawUDPSock, struct sockaddr_in from,  struct sockaddr_in to, int iovec_flag,  uint16_t  u16ContentLen);

int main(int argc , char* argv[])
{
  char szErrno[256];
  char    szFromAddrBuf[64] = {};
  char    szToAddrBuf[64]   = {};
  int rc;

  printf("Hello World!\n");
  struct sockaddr_in6 from = {}, to = {};
  from.sin6_family = to.sin6_family = AF_INET6;
  inet_pton(PF_INET6, "FEC0:2021::192.168.31.1", &from.sin6_addr);
  inet_pton(PF_INET6, "FEC0:2021::192.168.31.2", &to.sin6_addr);

  struct sockaddr_in from4 = {}, to4 = {};
  from4.sin_family = to4.sin_family = AF_INET;
  inet_pton(PF_INET, "192.168.31.1", &from4.sin_addr);
  inet_pton(PF_INET, "192.168.31.2", &to4.sin_addr);

  int iovec_flag,  opt;
  enum IPTYPES ip_flag = IPTYPE_V6;
  uint16_t  u16ContentLen = 2049;
  iovec_flag = 0;

  while ((opt = getopt(argc, argv, "4vs:d:l:")) != -1) {
     switch (opt) {
     case 'v':
         iovec_flag = 1;
         break;
     case 's':
          printf("Change src addr to %s\n", optarg);
         inet_pton(PF_INET6, optarg, &from.sin6_addr);
         inet_pton(PF_INET,  optarg, &from4.sin_addr);
         break;
     case 'd':
         printf("Change dst addr to %s\n", optarg);
         inet_pton(PF_INET6, optarg, &to.sin6_addr);
         inet_pton(PF_INET,  optarg, &to4.sin_addr);
         break;
     case 'l':
         u16ContentLen = atoi(optarg);        
         break;
     case '4':
         ip_flag = IPTYPE_V4;        
         break;
     default: /* '?' */
         fprintf(stderr, "Usage: %s [-s src] [-d dst] [-l len] [-v] [-4] name\n",
                 argv[0]);
         return 1;
     }
  }


  inet_ntop(AF_INET6, &from.sin6_addr, szFromAddrBuf, sizeof(szFromAddrBuf));
  inet_ntop(AF_INET6, &to.sin6_addr,   szToAddrBuf, sizeof(szToAddrBuf));

  if(ip_flag == IPTYPE_V4)
  {
    inet_ntop(AF_INET, &from4.sin_addr, szFromAddrBuf, sizeof(szFromAddrBuf));
    inet_ntop(AF_INET, &to4.sin_addr,   szToAddrBuf, sizeof(szToAddrBuf));
  }

  printf("ip_flag=%d; Src=%s; dst=%s; u16ContentLen=%u; iovec_flag=%d; optind=%d\n",
         ip_flag, szFromAddrBuf, szToAddrBuf, u16ContentLen, iovec_flag, optind);


  if (optind >= argc) {
     fprintf(stderr, "Can Expected argument after options\n");
  }

  int tRawUDPSock;

  if(ip_flag == IPTYPE_V6)
  {
    tRawUDPSock = socket(PF_INET6, SOCK_RAW, IPPROTO_UDP);
  }
  else
  {
    tRawUDPSock = socket(PF_INET, SOCK_RAW, IPPROTO_UDP);
  }

  if(tRawUDPSock < 0)
  {
    strerror_r(errno, szErrno, sizeof(szErrno));	  
    printf("Create raw udp protocol sock errno: %d -> %s\n", errno, szErrno);
    return -1;
  }

  printf("Create raw udp protocol sock successfully, got sock fd: %d\n", tRawUDPSock);

  if(ip_flag == IPTYPE_V6)
  {
    rc = bind(tRawUDPSock, (struct sockaddr*)&from, sizeof(from));
  }
  else
  {
    rc = bind(tRawUDPSock, (struct sockaddr*)&from4, sizeof(from4));
  }

   
  if(rc < 0)
  {
    strerror_r(errno, szErrno, sizeof(szErrno));    
    printf("Bind addr: %s , errno: %d -> %s\n", szFromAddrBuf, errno, szErrno);

    close(tRawUDPSock);
    return -1;
  }
  printf("Bind successfully !\n");

  
  if(ip_flag == IPTYPE_V6)
  {
    sendV6(tRawUDPSock, from, to, iovec_flag, u16ContentLen);
  }
  else
  {
    sendV4(tRawUDPSock, from4, to4, iovec_flag, u16ContentLen);
  }
  
  
  
  printf("Test program exit!\n"); 
  close(tRawUDPSock);

  return 0;
}

static uint8_t abPktInfo[4096];

void sendV6(int tRawUDPSock, struct sockaddr_in6 from, struct sockaddr_in6 to, int iovec_flag, uint16_t  u16ContentLen)
{
  char szErrno[256];

 
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

  size_t reqLen = sizeof(UDP_HEADER_t) + u16ContentLen;
  ptUdpHdr->u16SrcPort   = htons(12760);    
  ptUdpHdr->u16DstPort   = htons(22760);   
  ptUdpHdr->u16Length    = htons(reqLen);   
  ptUdpHdr->u16CheckSum  = 0;
  udp6_header_checksum(ptIPv6_header, ptUdpHdr, reqLen);

  
  printf("Will send IPv6 %zu bytes\n", reqLen);

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
    reqLen             = sizeof(UDP_HEADER_t) + udpvec[1].iov_len;
    ptUdpHdr->u16Length    = htons(reqLen);
    udp6_header_checksum(ptIPv6_header, ptUdpHdr, reqLen);   

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
     fprintf(stderr, "Send fail when sin6_port is not zero; errno:%d -> %s\n", errno, szErrno);
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
    printf("Send data errno: %d -> %s\n", errno, szErrno);
  }
  else
  {
    printf("Has send data %zd bytes!\n", sendLen); 
  }

}

void sendV4(int tRawUDPSock, struct sockaddr_in from, struct sockaddr_in to, int iovec_flag, uint16_t  u16ContentLen)
{
  char szErrno[256];

 
  from.sin_port = 0;
  to.sin_port   = 0;

  IPv4_HEADER_t* ptIPv4_header = (IPv4_HEADER_t*)abPktInfo;
  memset(ptIPv4_header, 0, sizeof(*ptIPv4_header));

  ptIPv4_header->bVersionAndHeaderLen = 0x45;
  ptIPv4_header->u16TotalLenOfPacket = htons(sizeof(IPv4_HEADER_t) + sizeof(UDP_HEADER_t) + u16ContentLen); 
  ptIPv4_header->bTypeOfProtocol     = IPPROTO_UDP; 
  ptIPv4_header->bTTL                = 128; 
  memcpy(ptIPv4_header->abSrcAddr, &from.sin_addr, sizeof(from.sin_addr));
  memcpy(ptIPv4_header->abDstAddr, &to.sin_addr, sizeof(to.sin_addr));

  UDP_HEADER_t* ptUdpHdr = (UDP_HEADER_t*)(abPktInfo + sizeof(IPv4_HEADER_t));

  size_t reqLen          = sizeof(UDP_HEADER_t) + u16ContentLen;
  ptUdpHdr->u16SrcPort   = htons(12760);    
  ptUdpHdr->u16DstPort   = htons(22760);   
  ptUdpHdr->u16Length    = htons(reqLen);   
  ptUdpHdr->u16CheckSum  = 0;
  udp4_header_checksum(ptIPv4_header, ptUdpHdr, reqLen);

  
  printf("Will send IPv4 %zu bytes\n", reqLen);

  ssize_t sendLen ;
  if(iovec_flag)
  {
    struct sockaddr_in  iovec_to = to;
    iovec_to.sin_port            = 1234;

    struct iovec udpvec[2];
    udpvec[0].iov_base = ptUdpHdr;
    udpvec[0].iov_len  = sizeof(UDP_HEADER_t);
    udpvec[1].iov_base = "hello world";
    udpvec[1].iov_len  = sizeof("hello world");
    reqLen             = sizeof(UDP_HEADER_t) + udpvec[1].iov_len;
    ptUdpHdr->u16Length    = htons(reqLen);
    udp4_header_checksum(ptIPv4_header, ptUdpHdr, reqLen);   

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
     fprintf(stderr, "Send fail when sin6_port is not zero; errno:%d -> %s\n", errno, szErrno);
    }

    iovec_to.sin_port           = 0;
    sendLen = sendmsg(tRawUDPSock, &msghdr, 0/*flags*/);
  }
  else
  {
    sendLen = sendto(tRawUDPSock, ptUdpHdr, reqLen, 0/*flags*/, (const struct sockaddr*)&to,  sizeof(to));
  }
 
  if(sendLen < 0)
  {
    strerror_r(errno, szErrno, sizeof(szErrno));    
    printf("Send data errno: %d -> %s\n", errno, szErrno);
  }
  else
  {
    printf("Has send data %zd bytes!\n", sendLen); 
  }

}
