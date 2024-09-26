#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

/* Must: less than `ulimit -s` and great than `ulimit -s`/2 */

#define STACK_BIG_SIZE (5*1024*1024)

void* f(void* arg);
void* f(void* arg)
{
    char childStackArray[STACK_BIG_SIZE] = {};
    
    while(1)
    {
        sleep(1);
    }
    return NULL;
}


int main(void)
{
  char mainSackArray[STACK_BIG_SIZE] = {};

  printf("Use big stack var in every thread, so can see the `ulimit -s` setting is for each thread!\n");

  unsigned char seq = 1;

  while(1)
  {
      printf("Enter any key to create child thread:\n");
        
      char szBuffer[256];    
      char* line = fgets(szBuffer, sizeof(szBuffer), stdin);
      if(!line)
      {
          return -1;
      }

      printf("Enter key '%c', so create child thread seq: %u\n\n", szBuffer[0], seq);
      pthread_t thread_id;
      int result = pthread_create(&thread_id, NULL, f, NULL);
      if(result != 0)
      {
          fprintf(stderr, "Error when creating thread! rc: %d\n", result);
          return -1;
      }

      ++seq;
  }
 
  return 0;
}
