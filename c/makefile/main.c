#include <stdlib.h>
#include <stdio.h>

extern int foo();

int main(int argc , char* argv[])
{
   #ifdef _DEBUG
   printf("DEBUG VERSION Running ...\n");  
   #endif

   printf("foo: %d\n", foo());
   return 0;
}
