#include <stdlib.h>
#include <stdio.h>

extern int foo();

int main(int argc , char* argv[])
{
   printf("foo: %d\n", foo());
   return 0;
}
