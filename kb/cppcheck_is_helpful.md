# 缘由
最近与一开源社区交流，了解到它的项目采用`cppcheck`做一项代码检查。

最近一直在试用`cppcheck`，看其效果。直到今天看到一个逻辑大致正确，但存在笔误的代码，觉得`cppcheck`对程序员还是有很大辅助价值的！

# 隐患代码
```c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>


int main(void)
{
  int a = 1;
  int b = 2;
  
  printf("The third parameter is inlvaid, Can GCC report warning?\n");
  if(memcmp(&a, &b, sizeof(a) != 0))
  {
      printf("Hello memcmp sizeof(int)\n");
  }
  
  int c[16] = {};
  int d[16] = {};
  
  if(memcmp(&c, &d, sizeof(c) != 0))
  {
      printf("Hello memcmp sizeof(int[16])\n");
  }
  
  int e[16] = {};
  int f[16] = {};
  
  if(memcmp(&e, &f, sizeof(e)) != 0)
  {
      printf("Hello memcmp sizeof(int[16]) with valid parameter\n");
  }

  printf("Exit from program!\n");
  return 0;
}
```

# 缺陷
这里存在一个明显的笔误，`GCC 7 or 9`都没有上报出来，用`cppcheck`工具就可以将存在隐患的问题上报出来 :)

```c
cppcheck main.c
Checking main.c ...
main.c:13:31: error: Invalid memcmp() argument nr 3. A non-boolean value is required. [invalidFunctionArgBool]
  if(memcmp(&a, &b, sizeof(a) != 0))
```
# 深入讨论
通过查看样例汇编代码，如果比较的是4个字节的信息，很有可能笔误，还是能够歪打正着的 :(

## cppcheck的优势
+ 免费
+ 易于定制，且可以扩充rule
+ 事实社区代码静态检查标准


