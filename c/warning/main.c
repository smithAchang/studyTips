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
	  printf("Hello 1\n");
  }

  printf("Exit from program!\n");
  return 0;
}
