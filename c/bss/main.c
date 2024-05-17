#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/resource.h>

// data段
unsigned char g_abBufferAtDataSegment[2 * 1024 * 1024] = { [0 ... (sizeof(g_abBufferAtDataSegment) - 1)] = 0xff };

// bss段
unsigned char g_abBufferAtBssSegment[4 * 1024 * 1024];

int main(void)
{
  char szInputBuffer[256];
  printf("global variable at dss segment: %zu, at bss segment: %zu, but only in VIRT space\nEnter any key to continue:", sizeof(g_abBufferAtDataSegment), sizeof(g_abBufferAtBssSegment));
  
  fgets(szInputBuffer, sizeof(szInputBuffer), stdin);
  
  printf("Will access the global memory, to see the RES stat change\nEnter any key to continue:");
  
  for(unsigned int i= 0; i< sizeof(g_abBufferAtDataSegment); ++i)
  {
	  g_abBufferAtDataSegment[i] = g_abBufferAtDataSegment[i];
  }
  
  for(unsigned int i= 0; i< sizeof(g_abBufferAtBssSegment); ++i)
  {
	  g_abBufferAtBssSegment[i] = g_abBufferAtBssSegment[i];
  }
  
  fgets(szInputBuffer, sizeof(szInputBuffer), stdin);

  struct rlimit rl = {};
  if (getrlimit(RLIMIT_STACK, &rl) == 0) {
    printf("Current stack size soft limit: %ld bytes, %ld MB\n", (long)rl.rlim_cur, (long)rl.rlim_cur/(1024*1024));
    printf("Current stack size hard limit: %ld bytes, %ld MB\n", (long)rl.rlim_max, (long)rl.rlim_max/(1024*1024));
  }


  printf("Enter the size factor of statck dynamic array(Unit: MB); q char will exit!\n");
  
  while (fgets(szInputBuffer, sizeof(szInputBuffer), stdin)) {
    if (szInputBuffer[0] == 'q')
      break;

    // 使用M级别以利于观察
    unsigned int size = atoi(szInputBuffer);
    unsigned char tmpBuffer[size * (1024 * 1024)];
    memset(tmpBuffer, 0, sizeof(tmpBuffer));
    printf("Memset the temp buffer, to see the memory uasge\n");
  }

  printf("Exit from program!\n");
  return 0;
}
