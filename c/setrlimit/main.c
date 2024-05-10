#include <stdio.h>
#include <sys/resource.h>

int main() {
  struct rlimit rlim = {};

  // 默认限制
  if (getrlimit(RLIMIT_NOFILE, &rlim) == -1) {
    perror("getrlimit");
    return 1;
  }

  printf("soft limit: %ld\n", rlim.rlim_cur);
  printf("hard limit: %ld\n", rlim.rlim_max);

  rlim.rlim_cur = 7024;                               // 设置软限制
  rlim.rlim_max = 65535;                               // 设置硬限制

  if (setrlimit(RLIMIT_NOFILE, &rlim) == -1) {
    perror("setrlimit");
    return 1;
  }

  // 检查修改后的文件句柄限制
  if (getrlimit(RLIMIT_NOFILE, &rlim) == -1) {
    perror("getrlimit");
    return 1;
  }

  printf("after setted, soft limit: %ld\n", rlim.rlim_cur);
  printf("after setted, hard limit: %ld\n", rlim.rlim_max);

  return 0;
}
