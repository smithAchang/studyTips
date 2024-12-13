#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>

#include "inline.h"

int main(void)
{
  printf("foo result:%d\n", foo(1));
  return 0;
}
