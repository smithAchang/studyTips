#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#define __USE_GNU
#include <sched.h>
#include <signal.h>


static int g_exit;
static void term_handler(int s)
{
    g_exit = 1;
}

/*
 * cat /proc/sys/kernel/sched_rt_runtime_us
 * cat /proc/sys/kernel/sched_rr_timeslice_ms
 * cat /sys/fs/cgroup/cpu/user.slice/cpu.rt_runtime_us
 * echo $((10*1000)) > /sys/fs/cgroup/cpu/user.slice/cpu.rt_runtime_us
*/
int main(int argc , char* argv[])
{
  char szErrorDesc[256];
  
  struct sigaction act, oldact;
  act.sa_handler  = term_handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  
  sigaction(SIGINT, &act, &oldact);
  sigaction(SIGTERM, &act, &oldact);
  //signal(SIGINT, SIG_IGN);
  
  pid_t pid = getpid();
  printf("pid: %d, policy: %d\n", pid, sched_getscheduler(pid));
  int policy = -1;
  
  int opt;
  
  while((opt = getopt(argc, argv, "rb")) != -1) {
     switch(opt) {
         case 'r':
         policy = SCHED_RR;
         break;
         case 'b':
         policy = SCHED_BATCH;
         break;
         default: /* '?' */
         fprintf(stderr, "Usage: %s [-r --SCHED_RR] [-b --SCHED_BATCH] \n", argv[0]);
         return 1;
     }
  }
  
  if(policy != -1) {
     struct sched_param tParam = {};
     
     if(policy == SCHED_RR)
        tParam.sched_priority = 1;
  
    int rc = sched_setscheduler(pid, policy, &tParam);
  
    if(rc < 0)
    {
      strerror_r(errno, szErrorDesc, sizeof(szErrorDesc));
      fprintf(stderr, "sched_setscheduler fail, errno: %d, %s", errno, szErrorDesc);
    }
    else
    {
      printf("after set thread policy: %d\n", sched_getscheduler(pid));
    }   
  }
  
  char szCmd[64];
  snprintf(szCmd, sizeof(szCmd), "cat /proc/%d/status", pid);
  printf("can see %s to see the context switch\n", szCmd);
  
  uint64_t count = 0;
  
  while(g_exit == 0) {
      ++count;
  }
  
  printf("cat %s to see the context switch result\n", szCmd);
  system(szCmd);
  
  return 0;
}
