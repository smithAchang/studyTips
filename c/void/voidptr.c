#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <stdint.h>

typedef struct Byte8 
{
  uint8_t  ab[8];
} Byte8;

typedef struct tagT_KeyInfo 
{
  uint8_t bType;
  uint16_t wLen;
  union
   {
     uint8_t  data[5]
     uin16_t  wData;
     uint32_t dwData;
} __attribute__((packed)) T_KeyInfo;

int f(void *arg)
{
  //arg中存储keyInfo信息，所以，按照约定，用求地址运算得到T_KeyInfo的指针，然后解引用
  T_KeyInfo tKeyInfo = *(T_KeyInfo*)&arg;
  
  printf("Passed arg info: bType: %u, wLen: %u, wData: %u\n", tKeyInfo.bType, tKeyInfo.wLen, tKeyInfo.wData);
  
  return 0;
}

int main(int argc, char* argv[])
{
  printf("Hello world !\n");

  void* p1 = (void*)1;
  Byte8  para1;
  
  void* p2 = *(void**)&para1;

  printf("Cannot convert to a pointer type\n");
  void* p3 = (void*)para1;


  
  T_KeyInfo tKeyInfo = {};
  tKeyInfo.bType = 100;
  tKeyInfo.wLen  = sizeof(uint16_t);
  tKeyInfo.wData = 60000;
  
  //作为字节容器，将机器字长度信息传入，用求地址运算得到地址，将其类型转化为void**，则解引用后就可以将字节信息装载到void*中
  void* arg = *(void**)&tKeyInfo; 
  f(arg);
  
  printf("Exit from program !\n");
  return 0;
}
