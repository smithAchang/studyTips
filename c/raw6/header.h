#ifndef _MY_HEADER_H
#define _MY_HEADER_H
       
typedef struct _IP_HEADER_t 
{
   uint8_t  bVersionAndHeaderLen;//版本信息(前4位)，头长度(后4位)
   uint8_t  bTypeOfService;      // 服务类型8位
   uint16_t u16TotalLenOfPacket;    //数据包长度
   uint16_t u16PacketID;      //数据包标识
   uint16_t u16Sliceinfo;       //分片使用
   uint8_t  bTTL;       //存活时间
   uint8_t  bTypeOfProtocol;    //协议类型
   uint16_t u16CheckSum;      //校验和
   uint32_t u32SourIp;    //源ip
   uint32_t u32DestIp;    //目的ip
} __attribute__((packed)) IP_HEADER_t, *PIP_HEADER_t ;

typedef struct _IPv6_HEADER_t 
{
 union
 {
   uint8_t abVTF[4];//版本信息 traffic flow label;
   uint32_t dwVTF;  
 };
 
 uint16_t sPayloadLen; //数据包长度
 uint8_t  bNextHeader; //数据包标识
 uint8_t  bHopLimit; // max hop limit

 union 
 {
    uint8_t abSrcAddr[16];
    u_int32_t au32SrcAddr[4];
    u_int64_t au64SrcAddr[2];
 };

 union 
 {
    uint8_t abDstAddr[16];
    u_int32_t au32DstAddr[4];
    u_int64_t au64DstAddr[2];
 };
 
} __attribute__((packed)) IPv6_HEADER_t, *PIPv6_HEADER_t ;


typedef struct _UDP_HEADER_t 
{
   uint16_t u16SrcPort;    
   uint16_t u16DstPort;   
   uint16_t u16Length;   
   uint16_t u16CheckSum;
} __attribute__((packed)) UDP_HEADER_t, *PUDP_HEADER_t ;


#endif