#ifndef _MY_HEADER_H
#define _MY_HEADER_H

#include <stddef.h>
#include <stdint.h>
#include <arpa/inet.h>

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

#pragma pack(push)
#pragma pack(1)

enum IPTYPES
{
  IPTYPE_V4 = 0x4,
  IPTYPE_V6 = 0x6
};

typedef struct _IP_HEADER_t
{
  uint8_t bVersionAndHeaderLen;                               // 版本信息(前4位)，头长度(后4位)
  uint8_t bTypeOfService;                                  // 服务类型8位
  uint16_t u16TotalLenOfPacket;                               // 数据包长度
  uint16_t u16PacketID;                                    // 数据包标识
  uint16_t u16Sliceinfo;                                   // 分片使用
  uint8_t bTTL;                                            // 存活时间
  uint8_t bTypeOfProtocol;                                 // 协议类型
  uint16_t u16CheckSum;                                    // 校验和
  union
  {
    uint8_t abSrcAddr[4];
    uint32_t u32SourIp;                                                             // 源ip
  };

  union
  {
    uint8_t abDstAddr[4];
    uint32_t u32DestIp;                                                             // 目的ip
  };
} IP_HEADER_t, IPv4_HEADER_t, *IP_HEADER_t_Ptr, *IPv4_HEADER_t_Ptr;

typedef struct _IPv6_HEADER_t
{
  union
  {
    uint8_t abVTF[4];                                                             // 版本信息 most top 4 bits , traffic 8 bits
    uint32_t dwVTF;                                                             // flow label lefted 20 bits;
  };

  uint16_t u16PayloadLen;                               // 数据包长度
  uint8_t bNextHeader;                               // 数据包标识
  uint8_t bHopLimit;                                // max hop limit

  union
  {
    uint8_t abSrcAddr[16];
    uint32_t au32SrcAddr[4];
    uint64_t au64SrcAddr[2];
  };

  union
  {
    uint8_t abDstAddr[16];
    uint32_t au32DstAddr[4];
    uint64_t au64DstAddr[2];
  };
} IPv6_HEADER_t, *IPv6_HEADER_t_Ptr;

typedef struct _UDP_HEADER_t
{
  uint16_t u16SrcPort;
  uint16_t u16DstPort;
  uint16_t u16Length;
  uint16_t u16CheckSum;
} UDP_HEADER_t, *UDP_HEADER_t_Ptr;

#pragma pack(pop)


/* Checksum a block of data */
static uint16_t csum(const uint16_t *packet, int packlen) {
  register unsigned long sum = 0;

  while (packlen > 1) {
    sum     += *(packet++);
    packlen -= 2;
  }

  if (packlen > 0) sum += *(unsigned char *)packet;

  /* TODO: this depends on byte order */

  while (sum >> 16) sum = (sum & 0xffff) + (sum >> 16);

  return (uint16_t) ~sum;
}

static void udp4_header_checksum2(IPv4_HEADER_t_Ptr pIPv4Header, UDP_HEADER_t_Ptr ptUDPHeader,
                                  size_t udpLen /*included header*/)
{
  uint32_t sum = 0;


  //
  ptUDPHeader->u16Length = htons(udpLen);
  sum                   += udpLen;

  sum = csum((uint16_t *)&pIPv4Header->abSrcAddr, 4);
  sum = csum((uint16_t *)&pIPv4Header->abDstAddr, 4);

  sum += ((uint16_t)pIPv4Header->bTypeOfProtocol & 0x00FF);

  // finish the pseudo header checksum

  // udp section
  ptUDPHeader->u16CheckSum = 0;
  sum                      = csum((uint16_t *)ptUDPHeader, udpLen);

  ptUDPHeader->u16CheckSum = htons((uint16_t)(~sum));
}

static void udp6_header_checksum2(IPv6_HEADER_t_Ptr pIPv6Header, UDP_HEADER_t_Ptr ptUDPHeader,
                                  size_t udpLen /*included header*/)
{
  uint32_t sum = 0;

  //
  ptUDPHeader->u16Length = htons(udpLen);
  sum                   += udpLen;

  sum = csum((uint16_t *)&pIPv6Header->abSrcAddr, 16);
  sum = csum((uint16_t *)&pIPv6Header->abDstAddr, 16);

  sum += ((uint16_t)pIPv6Header->bNextHeader & 0x00FF);

  // finish the pseudo header checksum

  // udp section
  ptUDPHeader->u16CheckSum = 0;
  sum                      = csum((uint16_t *)ptUDPHeader, udpLen);

  ptUDPHeader->u16CheckSum = htons((uint16_t)(~sum));
}

uint32_t Checksum(uint32_t cksum, const uint8_t *pBuffer, uint32_t size)
{
  if ((NULL == pBuffer) || (0 == size))
  {
    // return passed value
    return cksum;
  }

  uint32_t num     = 0;
  const uint8_t *p = pBuffer;

  while (size > 1)
  {
    cksum += ((uint16_t)p[num] << 8 & 0xff00) | ((uint16_t)p[num + 1] & 0x00FF);
    size  -= 2;
    num   += 2;
  }

  if (size > 0)
  {
    cksum += ((uint16_t)p[num] << 8) & 0xFFFF;
    num   += 1;
  }

  while (cksum >> 16)
  {
    cksum = (cksum & 0xFFFF) + (cksum >> 16);
  }

  return cksum;
}

void ipv4_header_checksum(IPv4_HEADER_t_Ptr pIpHeader)
{
  uint8_t ipHeadLen = (pIpHeader->bVersionAndHeaderLen & 0x0F) << 2;

  pIpHeader->u16CheckSum = 0;

  uint32_t sum = Checksum(0, (uint8_t *)pIpHeader, ipHeadLen);

  pIpHeader->u16CheckSum = htons((uint16_t)(~sum));
}

void udp4_header_checksum(IPv4_HEADER_t_Ptr ptIPv4Header, UDP_HEADER_t_Ptr ptUDPHeader,
                          size_t udpLen /*include header*/)
{
  uint32_t sum = 0;

  //
  ptUDPHeader->u16Length = htons(udpLen);
  sum                   += udpLen;

  sum = Checksum(sum, (uint8_t *)&ptIPv4Header->abSrcAddr, 4);
  sum = Checksum(sum, (uint8_t *)&ptIPv4Header->abDstAddr, 4);

  sum += ((uint16_t)ptIPv4Header->bTypeOfProtocol & 0x00FF);

  // finish the pseudo header checksum

  // udp section
  ptUDPHeader->u16CheckSum = 0;
  sum                      = Checksum(sum, (uint8_t *)ptUDPHeader, udpLen);

  ptUDPHeader->u16CheckSum = htons((uint16_t)(~sum));
}

void udp6_header_checksum(IPv6_HEADER_t_Ptr pIpHeader, UDP_HEADER_t_Ptr ptUDPHeader, size_t udpLen /*include header*/)
{
  uint32_t sum = 0;

  //
  ptUDPHeader->u16Length = htons(udpLen);
  sum                   += udpLen;

  sum = Checksum(sum, (uint8_t *)&pIpHeader->abSrcAddr, 16);
  sum = Checksum(sum, (uint8_t *)&pIpHeader->abDstAddr, 16);

  sum += ((uint16_t)pIpHeader->bNextHeader & 0x00FF);

  // finish the pseudo header checksum

  // udp section
  ptUDPHeader->u16CheckSum = 0;
  sum                      = Checksum(sum, (uint8_t *)ptUDPHeader, udpLen);

  ptUDPHeader->u16CheckSum = htons((uint16_t)(~sum));
}

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif // ifndef _MY_HEADER_H
