#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>

void readAnyKey()
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF); 	
}

int main(int argc , char* argv[])
{
  printf("Hello world!\n"
         "To see the `top` command memory statistics!\n\n");

  int sleeptime = 3;

  if(argc > 1)
  {
    sleeptime = atoi(argv[1]);
  }

  printf("Using sleeptime: %d argument to watch the incresing RES(physical) statistic\n"
         "But the VIRT statistic keeps unchanged after malloc\n\n", sleeptime);
  
  printf("You can see the program's VIRT&RES&%%MEM statistics using `top` command\n\n"
         "The initial VIRT&RES statistics must be small\n"
         "Enter any key to continue to malloc a big memory area ...\n");
  readAnyKey();

  const size_t nBigSize = 1*1024*1024*1024;
  char* const pbBigMem  = malloc(nBigSize);

  if(pbBigMem == NULL)
  {
    perror("Malloc a big memory");
    return -1;
  }

  printf("\nThe VIRT statistic is changed after malloc\n");

  char* const pbBigMemEnd = pbBigMem + nBigSize;
  char* cursor = pbBigMem;

  do
  {
     // Access the memory to cause the physical memory to be assigned
     for(size_t i = 0; i < (100*1024*1024) && cursor < pbBigMemEnd; ++i, ++cursor)
      {
        // one by one
        cursor[0] = (char)i;
      }  

      printf("The RES statistic is changed after the memory is really scaned one by one, cursor: %p\n", cursor);
      sleep(sleeptime);
  }
  while(cursor < pbBigMemEnd);
  
  free(pbBigMem);
  printf("You can see the program's VIRT&RES statistics changed after big memroy freed\n"
         "Enter any key to exit ...\n");
  readAnyKey();

  printf("\n\nExit from program !\n");
  return 0;
}
