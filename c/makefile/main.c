#include <stdlib.h>
#include <stdio.h>

extern int foo();

int        main(int argc, char *argv[])
{
         #ifdef _DEBUG
  printf("DEBUG VERSION Running ...\n");
         #endif /* ifdef _DEBUG */

  printf("foo: %d\n", foo());
  return 0;
}
