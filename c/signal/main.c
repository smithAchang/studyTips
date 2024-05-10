#include <signal.h>
#include <stdio.h>

void custom_signal_handler(int signum) {
  // 自定义处理逻辑...
  printf("Received signal %d, executing custom logic...\n", signum);

  // 调用默认信号处理程序前，可能需要恢复信号处理函数为默认值
  struct sigaction sa;
  sa.sa_handler = SIG_DFL;                                // 设置为默认处理方式
  sigemptyset(&sa.sa_mask);                               // 清空信号集
  sa.sa_flags = 0;                                        // 不启用任何特殊标志

  sigaction(signum, &sa, NULL);                               // 更新信号处理方式

  // 发送信号给自己，让内核执行默认操作
  kill(getpid(), signum);
}

int main() {
  struct sigaction sa;

  sa.sa_handler = custom_signal_handler;                               // 设置自定义处理函数
  sigemptyset(&sa.sa_mask);                                           // 阻塞该信号在处理函数执行期间不被递送
  sa.sa_flags = SA_NODEFER | SA_RESETHAND;                               // 不屏蔽信号且执行完后恢复默认处理方式

  if (sigaction(SIGINT, &sa, NULL) == -1) {                               // 示例以SIGINT为例
    perror("sigaction");
    return 1;
  }

  while (1) {
    // 主循环或程序主体
    pause();                                                             // 或其他阻塞操作
  }

  return 0;
}
