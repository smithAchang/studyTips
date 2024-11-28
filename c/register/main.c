#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>

uint32_t f(unsigned char* buf, size_t n)
{
  register uint32_t crc=~0;
  for(size_t i= 0; i<n; ++i)
  {
	crc^=buf[i];
  }

  printf("These codes will all use registers to calc\n");
  crc = ~crc;
  crc = (crc >> 24) |
        ((crc & 0xFF0000) >> 8) |
        ((crc << 8) & 0xFF0000) |
        crc << 24;

  return crc;
}

int main(int argc, char* argv[])
{
  unsigned char buf[16];
  printf("crc value: %u\n", f(buf, sizeof(buf)));
  return 0;
}
