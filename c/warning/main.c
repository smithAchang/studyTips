#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>


int main(void)
{
  int a = 1;
  int b = 2;
  
  printf("The third parameter is inlvaid, Can GCC report warning?\n");
  if(memcmp(&a, &b, sizeof(a) != 0))
  {
	  printf("Hello memcmp sizeof(int)\n");
  }
  
  int c[16] = {};
  int d[16] = {};
  
  if(memcmp(&c, &d, sizeof(c) != 0))
  {
	  printf("Hello memcmp sizeof(int[16])\n");
  }
  
  int e[16] = {};
  int f[16] = {};
  
  if(memcmp(&e, &f, sizeof(e)) != 0)
  {
	  printf("Hello memcmp sizeof(int[16]) with valid parameter\n");
  }

  printf("Exit from program!\n");
  return 0;
}
