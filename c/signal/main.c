#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <errno.h>

unsigned char g_thread_test;
unsigned char g_no_pass_again;
unsigned char g_epoll_blocked = 0xff;
pthread_t g_childThreadId;
int g_signumpara = SIGINT;


// 自定义处理逻辑...
void custom_signal_handler(int signum)
{
  if (g_signumpara != signum)
    return;

  printf("Received signal %d, executing custom logic. it simply restores the default signal process, threadid:0x%lx ... ...\n", signum, pthread_self());

  // 调用默认信号处理程序前，可能需要恢复信号处理函数为默认值
  struct sigaction sa;
  sa.sa_handler = SIG_DFL;                                // 设置为默认处理方式
  sigemptyset(&sa.sa_mask);                               // 清空信号集
  sa.sa_flags = 0;                                        // 不启用任何特殊标志

  sigaction(signum, &sa, NULL);                           // 更新信号处理方式

  if (g_thread_test == 0) {
    if (g_no_pass_again == 0) {
      printf("Send signal %d to self again, to execute default logic ...\n", signum);
      kill(getpid(), signum);
    } else {
      printf("To see whether the process exits without passing the signal again ...\n");
    }
  } else {
    printf("Send signal to child thread again ...\n");
    pthread_kill(g_childThreadId, g_signumpara);
  }
}

void * thread_entry(void *para)
{
  int rc;

  printf("Child thread begins to run, threadid:0x%lx ...\n", pthread_self());
  if (g_epoll_blocked == 0xff) {
    printf("Using pause api to be blocked until a signal was caught\n");
    pause();
    printf("Child thread will exit when blocked by pause api ...\n");
    return para;
  }

  printf("Using epoll api be blocked\n");
  int epollfd = epoll_create(1);
  if (epollfd < 0) {
    perror("epoll_create");
    return para;
  }

  int pipes[2];
  rc = pipe(pipes);

  if (rc < 0) {
    perror("pipe");
    return para;
  }
  
  struct epoll_event evt = {};
  evt.events = EPOLLIN|EPOLLHUP|EPOLLRDHUP|EPOLLET;
  evt.data.fd = pipes[0];
  
  rc = epoll_ctl(epollfd, EPOLL_CTL_ADD, pipes[0], &evt);
  if (rc < 0) {
    perror("epoll_ctl add");
    return para;
  }
  
  struct epoll_event evts[5] = {};
   
  if(g_epoll_blocked == 0)
  {
      rc = epoll_pwait(epollfd, evts, 5, 60*1000/*ms*/, NULL);
      if(rc < 0)
      {
          printf("epoll_pwait rc: %d, errno: %d\n", rc, errno);
      }
  }
  else if(g_epoll_blocked == 1)
  {
      sigset_t  sigset = {};
      sigemptyset(&sigset);
      sigaddset(&sigset, g_signumpara);
      rc = epoll_pwait(epollfd, evts, 5, 60*1000/*ms*/, &sigset);
      if(rc < 0)
      {
          printf("epoll_pwait has sigset para, rc: %d, errno: %d\n", rc, errno);
      }
  }


  printf("Child thread will exit ...\n");
  return para;
}

int main(int argc, char *argv[])
{
  int opt, rc;

  while ((opt = getopt(argc, argv, "tns:e:")) != -1) {
    switch (opt) {
    case 't':
      printf("Set thread test flag ...\n");
      g_thread_test = 1;
      break;
    case 'n':
      printf("Set no pass signal again flag ...\n");
      g_no_pass_again = 1;
      break;
    case 'e':
      printf("Set epoll blocked flag ...\n");
      g_epoll_blocked = atoi(optarg);
      break;
    case 's':
      printf("Set signum to be %d ...\n", g_signumpara);
      g_signumpara = atoi(optarg);
      break;
    default:                                                             /* '?' */
      fprintf(stderr, "Usage: %s [-t --thread] [-n --no_pass] [-s sinalnum] [-e 0 for null para ; 1 will provide sigset para]\n", argv[0]);
      return 1;
    }
  }

  struct sigaction sa;
  sa.sa_handler = custom_signal_handler;                              // 设置自定义处理函数
  sigemptyset(&sa.sa_mask);                                           // 阻塞该信号在处理函数执行期间不被递送
  sa.sa_flags = SA_NODEFER | SA_RESETHAND;                            // 不屏蔽信号且执行完后恢复默认处理方式

  if (sigaction(g_signumpara, &sa, NULL) == -1) {                     // 示例以SIGINT为例
    perror("sigaction");
    return 1;
  }

  printf("Create child thread to run ...\n");

  pthread_t tid;
  rc = pthread_create(&tid, NULL, thread_entry, NULL);
  if (rc < 0) {
    perror("pthread_create");
    return 1;
  }
  g_childThreadId = tid;

  printf("You can tirgger the desired signum ...\n");

  pthread_join(g_childThreadId, NULL);

  printf("Main thread will exit ...\n");
  return 0;
}
