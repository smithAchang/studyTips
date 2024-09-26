# 栈空间限制的疑惑
使用`ulimit -s stackKBSize`可以限制运行程序的可使用栈空间的大小。

但，如果是多线程场景，该系统限制，是限制一个进程内的所有线程共享一个总共系统受限制大小的栈空间，还是每个线程都独立一份栈空间，而且每个大小都与系统限制相同呢？

# 结论先行

**系统栈空间限制是线程级的**

# 验证方法设计
+ 非线程声明大size栈变量并清零完成分配占用，同时保持无限循环，持有栈空间
+ 主线程声明大size栈变量并清零完成分配占用，且阻塞读取输入，每新读取一行，则创建一新线程
+ 逐次用比较工具观察`cat /proc/$pid/maps`内存映射的变化

## 创建线程后的发现
+ 线程的栈空间是在**Heap**上分配的，体现为每增加一个线程，则在堆映射上增加新的内存映射
+ 随着开辟线程的增多，进程虚拟内存增长量会增多，创建线程存在空间代价

# 程序代码
```c
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

```
