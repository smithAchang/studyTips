#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>  
#include <sys/socket.h>
#include <arpa/inet.h>


typedef struct _IP_HEADER_t 
{
 uint8_t cVersionAndHeaderLen;//版本信息(前4位)，头长度(后4位)
 uint8_t cTypeOfService;      // 服务类型8位
 uint16_t sTotalLenOfPacket;    //数据包长度
 uint16_t sPacketID;      //数据包标识
 uint16_t sSliceinfo;       //分片使用
 uint8_t cTTL;       //存活时间
 uint8_t cTypeOfProtocol;    //协议类型
 uint16_t sCheckSum;      //校验和
 uint32_t uiSourIp;    //源ip
 uint32_t uiDestIp;    //目的ip
} __attribute__((packed)) IP_HEADER_t, *PIP_HEADER_t ;

typedef struct _IPv6_HEADER_t 
{
 uint8_t abVTF[4];//版本信息 traffic flow label
 
 uint16_t sPayloadLen; //数据包长度
 uint8_t  bNextHeader; //数据包标识
 uint8_t  bHopLimit; // max hop limit

 union 
 {
    uint8_t abSrcAddr[16];
    u_int32_t ai32SrcAddr[4];
    u_int64_t ai64SrcAddr[2];
 };

 union 
 {
    uint8_t abDstAddr[16];
    u_int32_t ai32DstAddr[4];
    u_int64_t ai64DstAddr[2];
 };
 
} __attribute__((packed)) IPv6_HEADER_t, *PIPv6_HEADER_t ;

typedef struct _UDP_HEADER_t {
  uint16_t u16SrcPort ;   // 源端口号16bit
  uint16_t u16DstPort ;   // 目的端口号16bit
  uint16_t u16Length ;   // 数据包长度16bit
  uint16_t u16CheckSum ;   // 校验和16bit
} __attribute__((packed)) UDP_HEADER_t, *PUDP_HEADER_t ;




int main(int argc , char* argv[])
{
  char szErrno[256];

  printf("Hello World!\n");

  int tRawSendOnlySock = socket(PF_INET6, SOCK_RAW, IPPROTO_RAW);

  if(tRawSendOnlySock < 0)
  {
    strerror_r(errno, szErrno, sizeof(szErrno));	  
    printf("create raw udp protocol sock errno: %d -> %s\n", errno, szErrno);
    return -1;
  }

  printf("create raw all protocol sock successfully, got sock fd: %d\n", tRawSendOnlySock);

  struct sockaddr_in6 from = {}, to = {};
  from.sin6_family = to.sin6_family = AF_INET6;
  inet_pton(PF_INET6, "FEC0:2021::192.168.31.1", &from.sin6_addr);
  inet_pton(PF_INET6, "FEC0:2021::192.168.31.2", &to.sin6_addr);

  if(argc >1 )
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
  

  int rc = bind(tRawSendOnlySock, (struct sockaddr*)&from, sizeof(from));
  if(rc < 0)
  {
    strerror_r(errno, szErrno, sizeof(szErrno));	  
    printf("bind errno: %d -> %s\n", errno, szErrno);
    close(tRawSendOnlySock);
    return -1;
  }
  printf("bind successfully !\n");

  uint8_t abPktInfo[4096];
  uint16_t  u16ContentLen = 512;
  from.sin6_port = 0;
  to.sin6_port   = 0;

  IPv6_HEADER_t* ptIPv6_header = (IPv6_HEADER_t*)abPktInfo;
  memset(ptIPv6_header, 0, sizeof(*ptIPv6_header));

  ptIPv6_header->abVTF[0]    = 0x60;
  ptIPv6_header->sPayloadLen = htons(sizeof(UDP_HEADER_t) + u16ContentLen); 
  ptIPv6_header->bNextHeader = IPPROTO_UDP; 
  ptIPv6_header->bHopLimit   = 128; 
  memcpy(ptIPv6_header->abSrcAddr, &from.sin6_addr, sizeof(from.sin6_addr));
  memcpy(ptIPv6_header->abDstAddr, &to.sin6_addr, sizeof(to.sin6_addr));

  UDP_HEADER_t* ptUdpHdr = (UDP_HEADER_t*)(abPktInfo + sizeof(IPv6_HEADER_t));

  ptUdpHdr->u16SrcPort   = htons(12760);    
  ptUdpHdr->u16DstPort   = htons(22760);   
  ptUdpHdr->u16Length    = htons(sizeof(UDP_HEADER_t) + u16ContentLen);   
  ptUdpHdr->u16CheckSum  = 0;

  u16ContentLen = 512;

  do
  {
    size_t reqLen = sizeof(IPv6_HEADER_t) + sizeof(UDP_HEADER_t) + u16ContentLen;
    printf("will send %zu bytes\n", reqLen);
    ssize_t sendLen = sendto(tRawSendOnlySock, ptIPv6_header, reqLen, 0/*flags*/, (const struct sockaddr*)&to,  sizeof(to));
  
    if(sendLen < 0)
    {
      strerror_r(errno, szErrno, sizeof(szErrno));	  
      printf("send data errno: %d -> %s\n", errno, szErrno);
    }
    else
    {
      printf("has send data %zd bytes!\n", sendLen); 
    }
    u16ContentLen += 1500;
  } while (u16ContentLen < 2048);
  
  
  
  
  
  printf("test program exit!\n"); 
  close(tRawSendOnlySock);

  return 0;
}
