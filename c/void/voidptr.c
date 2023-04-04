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

int main(int argc , char* argv[])
{
  printf("Hello world !\n");

  void* p1 = (void*)1;
  Byte8  para1;
  
  void* p2 = *(void**)&para1;

  printf("Cannot convert to a pointer type\n");
  void* p3 = (void*)para1;

  printf("Exit from program !\n");
  return 0;
}
