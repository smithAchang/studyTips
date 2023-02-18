# 修改记录

版本号    |日期             | 更改理由  
----------|----------------|----------------
V1.0      | 2022-03-15     | MD化
V1.01     | 2023-02-17     | 零值和-1同时作为非法值





# 总则

**仅总结一些常用且实用的编程规范和技巧，且避免记忆负担，聚焦影响比较大的20% !**



# 编译器
## 打开全warning编译器开关

**正例**
```bash
gcc -W -Wall -g -o someProc main.c
```


~~**反例**~~
```bash
gcc -g -o someProc main.c
```

### 建议增加-W -Wall -g编译器开关

+ 打开-W -Wall 警告开关，可以让编译器尽量将代码中的告警进行展示
+ 打开-g开关，以利于在出错的情况下，可以较为准确地获知堆栈信息

### 持续消除warning

在warning开关全开的情况下，应检查warning的情况，最好做到零warning。

### 建议将某些warning转为编译error

可以根据工程特点，总结出来一批，影响比较大，而且容易出现问题的warning，直接转化为编译器error选项，以利于用编译工具保证质量。有条件的项目编译工程，可以通过-Werror将全部warning转为编译error。

**正例**
```bash
gcc -W -Wall -Werror=overflow -g -o someProc main.c

或

gcc -W -Wall -Werror [-Wno-*...] -g -o someProc main.c
```

> 即使在-Werror开关的情况下，依然可以通过-Wno-\*的编译选项进行定制排除，将某些影响不大的warning排除


## 使用高版本编译器

> 借用高版本编译器往往能够检查出代码中重大隐患，和产生更高质量的代码，建议在能够使用高版本编译器的情况下，尽量使用

### 建议使用docker构建高版本编译器环境

在docker某基础镜像中加入devtoolset-\*组件，对于CentOS系列发行版，建议安装devtoolset-7或devtoolset-9。这样对于当前旧编译器依赖的工程，可以无影响地进行高版本编译编译，提前将一些隐患排除。


**启动镜像时脚本**

```bash
docker run -it --rm -h devtoolsetVersion -w /usegccversion -v $PWD:/usegccversion image bash
```

## 使用高版本编译标准

> 在低版本编译器中，尽量使用高版本编译标准，可以使用到某些友好特性和错误检查

### 获得支持临时循环指示变量临近使用特性

**正例**
```c
for(unsigned int i = 0; i < MAX; ++i) 
{ 
  ... 
}
```


~~**反例**~~
```c
void f()
{
  unsigned int i = 5;
  // some other code
  ...

  for(i=0; i < MAX; ++i)
  {
    ...
  }

}
```

> gcc编译器可以指定--std=gnu11等一些高版本编译标准。临近使用变量特性将使得封装性更好，避免无效的提前干扰，有利于阅读代码

### 建议O2优化标准

编译器优化可以将代码运行性能加速到极致；但为了避免一些负优化，建议使用与Linux内核一致的O2优化级别。

> 优化虽在，我们仍提倡撰写高质量的代码，作为比较好的编译器输入！

#### 适当地方使用register变量

 C语言支持寄存器变量，x86-64上又提供更多的寄存器可用，所以在计算校验和等比较密集耗费CPU的场景，对于关键变量建议使用register关键词修饰。




# 编程规范

## 以人为本

### 空格间隔操作符与左值右值

**正例**
```c
void f(int * intPtr)
{
  if(NULL == intPtr)
  {
    ...
  }

  ...
}
```


~~**反例**~~
```c
void f(int * intPtr)
{
  if(NULL==intPtr)
  {
    ...
  }

  ...
}
```

> 以人为本的理念，在编程实践时，如果对于机器阅读没有困难，而对于程序员阅读存在困难的代码，都应该通过空格、空行分割、打印日志分割、scope对齐等常用方式进行辅助编码。

### 源码编辑Tab转为空格

+ 编辑器设定Tab键空格数量为4
+ 代码编辑器"设置"支持将Tab固定转为空格
+ 代码编辑器支持将旧文件中Tab批量转为空格

### 控制语句即使只有一行也使用花括号作为块分割

**正例**
```c
void main(void)
{
  if(someConditionOk)
   {
     someFlag = TRUE;
   }
   
   ...
}
```


~~**反例**~~
```c
void main(void)
{
  if(someConditionOk)
     someFlag = TRUE;

   ...
}
```

> { }分割有助于人眼分辨，而不是机器可以正确处理。在后期if分支修改时，不容易引起维护上的问题。

### 用const关键词修饰栈变量和函数入参指针参数

+ 用编译器拦截一部分意外修改
+ 增加编译器优化的深度，例如，对于不变量可以进行深度优化
+ 对于const类型的数据量，熟练阅读代码的人，可以有选择地忽略和选择重点变化代码部分

> 多数较为新的C语言开源组件，在API接口设计，多遵从const 修饰符原则

### 巧用<>和""包含头文件区别依赖可变性

**正例**
```c
// system api
#include <sys/time.h>

// third-party api
#include <third-party/time.h>

//self module api
#include "otherApiInModule.h"
```


~~**反例**~~
```c
// system api
#include <sys/time.h>

// third-party api
#include "third-party/time.h"

//self module api
#include "otherApiInModule.h"
```

> 原则上有区别的差别，最好采用有区别的写法。从书写字面区分头文件组件内外不同，显示引用API的可变程度不同，对于后续阅读代码有利。

### 分支语句尽量采用likely/unlikely标注

**正例**
```c
// system api
if(likely(conditiontest))
{
 //most hit code
 ...
}
else
{
  //less hit code
  ...
}
```


~~**反例**~~
```c
if(conditiontest)
{

  ...
}
else
{
  ...
}
```

> + likeyly/unlikely提高代码可阅读性，自然关注主要逻辑流程
  + 让编译器生成代码对于分支预测机制更为友好

### 异常分支必须打印异常日志

**正例**
```c
int f(int * intPtr)
{
  if(NULL == intPtr)
  {
    exception_log("some must parameter is NULL");
    return errorValue;
  }

  //normal flow
  ...
}
```


~~**反例**~~
```c
int f(int* intPtr)
{
 if(NULL == intPtr)
 {
   return errorValue;
 }

 //normal flow
 ...
}
```

### typedef有名结构体

**正例**
```c
typedef struct tagPerson
{
  unsigned char name[MAX_NAME];
  int age;
} Person;
```


~~**反例**~~
```c
typedef struct
{
  unsigned char name[MAX_NAME];
  int age;
} Person;
```

> + typedef结构体不具有名称，则在旧版IDE编辑器中，会造成输入辅助提示的混乱
  + 在typedef命名的新名称前加入"tag"作为结构体的名称，也是一种惯用法

### 代码体行数限制

200行为**上限**，*太长*则需分拆

> 首先是代码逻辑上梳理清晰，然后就可以分步骤、分组件，分为3~5个概念步骤，理解最顺畅

### 让规范代码模式重复出现

在维护过程中，由于历史存量的原因，规范的代码和不规范的代码并存，应尽量批量修改掉某种范式的不规范代码，以避免将来拷贝粘贴代码进一步扩大"污染"。 

> 例如，`sprintf`字符串函数可以自动补零，但是对于拷贝目的地的长度没有保护，所以，在涉及的地方，几乎可以无危险代价地替换为`snprintf`函数 。

**可以使用代码编辑器中提供的调用堆栈、正则表达式等工具批量发现修改涉及点**

### 避免简单重复代码出现

此条建议看似与前面建议冲突，实则不然。简单重复的代码是代码中最大的坏味道，和体现程序员的懒惰。  
当某些代码重复出现三次以上时，是你应该考虑用公共组件来代替简单重复的时候了。

### 敢为人先避免破窗效应

破窗效应是指某个地方出现了"坏"的味道后，后面这种坏的情况在自然情况下，没有人愿意改变，只能变得更糟。  
作为程序员，应该敢于迈出第一步，让代码变得比昨天更好。

### 注重重构技巧
重构代码也是有方法论的，具体可见《重构改善即有代码的设计》。  
在编译器、自动化测试用例保护、以及重构指导步骤的导引下，进行代码重构，尽量降低出错概率，给自己或他人增强修改代码的信心。


## 注释

### 进行必要的简短注释

注释的两个极端是没有注释或充满注释，都不是很好的策略，仅进行必要的注释。

### 不简明代码的注释等同于一句道歉

对于不简明的代码进行注释，实际上等同于道歉，说明这些代码太复杂了，作者怕后来阅读者不能尽懂，不得不用注释进行特殊说明。  
可以考虑是否有简明的实现，进行代码重构。

> 来自网络观点的启发

### 如果注释的地方可以增补日志用日志代替

在避免重复的原则下，如果能够同时出现注释和日志，那么用日志代替注释即可，而且日志可以打印更完整的运行时信息，利于后期分析问题。


## 编码技巧

### 使用空{}清零初始化结构和数组栈变量

**正例**
```c
void f()
{
  struct A a = {};
  unsigned char buffer[512] = {};
 
  //some code using the above vars
  ...
}
```


~~**反例**~~
```c
void f()
{
  struct A a ;
  unsigned char buffer[512] ;
 
  memset(&a, 0, sizeof(a));
  memset(buffer, 0, sizeof(buffer));

  //some code using the above vars
  ...
}
```

> + 空花括号清零简洁、美观、高效
  + 小块内存清零应避免调用函数的代价。在C23 C语言建议标准中，已作为标准建议替代memset
  + 空花括号和花括号带零，适应不同的情况；空花括号清零应对结构体和结构体内含有结构体成员的复杂结构体

#### 建议零和-1同时作为非法值

因为业务代码惯用法通常会清零结构体，作为结构体成员的初始状态。在大型工程中，存在某些异常，未设定成员成为真正的非法值-1，在异常流程下，业务会对零值成员，因为不是非法值-1，会发起资源清理动作。<br>
如果零值正好作为某个资源管理模块的正常值，资源将会被异常流程释放掉。例如，定时器模块定时器ID可以为零值，可能会造成正常拥有零值定时器资源的逻辑，被异常逻辑释放掉!

> 零值和-1同时作为非法值，虽然比较奇怪，但对于大型C语言工程，具有实用性。


### 内存操作长度sizeof(var)变量化

**正例**
```c
void f(const unsigned char* pBuffer)
{
  struct A a ;
  struct A aa;

  memcpy(&a,  pBuffer,  sizeof(a));

  struct A* ptA  = &aa;
  memcpy(ptA,  pBuffer,  sizeof(*ptA));
}
```


~~**反例**~~
```c
void f(const unsigned char* pBuffer)
{
  struct A a ;
  struct A aa;

  memcpy(&a,  pBuffer,  sizeof(struct A));

  struct A* ptA  = &aa;
  memcpy(ptA,  pBuffer,  sizeof(struct A));
}
```

> 此条建议可能有所争议；但变量化存在至少两个好处：   
>> + 自适应类型变化，避免书写类型  
>> + 如果sizeof后面参数填写错误，利于立即发现问题；使用不匹配类型，一些少量的内存操作越界不易被发现

### return卫语句缓解复杂度

**正例**
```c
if(conditionTest)
{
   exception_log(...);
   return flow_end;
}

//normal code flow
...
```


~~**反例**~~
```c
void f()
{
  if(condition == TRUE)
  {
   return flow_end;
  }
  else
  {
    ...
  }
 
  return end_of_code;
}
```

### 就近访存

#### 原地化访问

从计算机机器结构上来看，计算机访问存储器偏向于局部访问，CPU可以在不改变基地址的情况下，用小范围的地址偏移增减就可以获得快速访问。
如果访问存储器位置在非常局部的内存，也可从计算机缓存体系得到受益。

#### 远端访问栈变量化

**正例**
```c
struct A* const ptA = getASingleton();

while(TRUE)
{
   //some usage
   f(ptA);
}

```


~~**反例**~~
```c
extern struct A*  getASingleton();

while(TRUE)
{
   //some usage
   f(getASingleton());
}
```

### 应使用简明数值操作写法 

在算术操作，尽量使用++、--、+=、\*=等简写语言操作符；一方面书写简明，另一方面以利于编译做优化。


~~**反例**~~
```c
int offset = initValue;

...

//在编译器不能很好优化代码情况下，可以能会多一次访存操作
offset     = offset + 1;
```

### 使用匿名联合代替单一命名以利于多用途操作

**正例**
```c
typedef struct tagIPv4_Addr
{
  union {
    unsigned char abAddr[4];
    unsigned int dwAddr;
  };
} IPv4_Addr;

void f()
{
  IPv4_addr ip ;
  unsigned char buffer[512];

  //for whole value usage
  printf("encoding IPv4 Addr Value:%u\n",  ip.dwAddr);
 
  //for byte operation
  memcpy(buffer,  ip.abAddr,  sizeof(ip.abAddr));
}
```


~~**反例**~~
```c
void f()
{
  unsigned char  ip[4] ;
  unsigned char buffer[512];

  //for whole value usage
  printf("encoding IPv4 Addr Value:%u\n",  *(unsigned int *)ip);
 
  //for byte operation
  memcpy(buffer,  ip,  sizeof(ip));
}
```

> 使用匿名联合几乎无代价，和更简明。在某些不便修改的地方，可以用匿名联合做别名访问

### 准确使用类型让编译器做更多事情

+ 数组下标应使用无符号数
+ 如果输入可以限定在某一范围，请使用枚举类型；也利于调试时将值显示为更有意义的命名，而非数值。能用枚举类型的场景，尽量用枚举。

#### 数值型宏定义指定明确类型

**正例**
```c
#define MAX_CAPACITY (unsigned char)64
```


~~**反例**~~
```c
#define MAX_CAPACITY 64
```

#### 注意类型比较类型一致

**正例**
```c
#define MAX_CAPACITY (unsigned char)64

unsigned char bStartIndex  = 12;
unsigned char bEndIndex    = 36;

if((unsigned char )(bEndIndex  -  bStartIndex) >  MAX_CAPACITY)
{
  exception_log(...);
  return FALSE;
}
```


~~**反例**~~
```c
#define MAX_CAPACITY 64

unsigned char bStartIndex  = 12;
unsigned char bEndIndex    = 11;

//编译器生成代码有点出乎意料，允许负值的产生
if((bEndIndex  -  bStartIndex  ) >  MAX_CAPACITY)
{
  exception_log(...);
  return FALSE;
}

//error 无法防止负值的出现
... 
```


## 头文件

### 头文件设计内外有别

头文件区分对外部使用头文件和对内使用头文件，对外API头文件仅提供少量公共、必要的声明，以利于模块间的隔离和API接口Bridge桥模式独立演化演化。

**正例**
```bash
  -- someComponent
   +--  include
    ++  component.h 
    ++  component_internal.h
    ++  ...
   +--  src
    ++  ...
```

~~**反例**~~
```bash
 -- someComponent
   +--  include
    ++  component_api_all_in_one.h 
   +--  src
```

### 头文件加入C++支持和头宏定义避免重复包含支持 

**正例**
```c
#ifndef __XX_YY_h
#define __XX_YY_h

#ifdef  __cplusplus
extern "C" {
#endif

  //some declarations
  ...

#ifdef  __cplusplus
 }
#endif

#endif
```


~~**反例**~~
```c
// a header file
//some C language declarations
...
```

### 稳定API设计技巧利用上下文对象指针和操作函数

**正例**
```c
#ifndef __XX_YY_h
#define __XX_YY_h

#ifdef  __cplusplus
extern "C" {
#endif

//some declarations
typedef struct tag_T_Context { ... } T_Context;

T_Context* api_Alloc_Context();
int api_set_Context_Option(T_Context* ptContext, someType optionPara);
int api_request(T_Context* ptContext, someType optionParas ...);
int api_responset(T_Context* ptContext, someType optionParas ...);
int api_setCallback(T_Context* ptContext, someCallbackFunction fn ...);


#ifdef  __cplusplus
 }
#endif

#endif
```
> 这样的设计让上下文对象成为可以保证不同实现间的相互隔离、独立、并发并行，而且不操作具体内存相关的字段，具有二进制布局依赖独立性，可以视作C语言编程领域内对**接口**进行编程。



## 日志规范

### 日志信息中提供必要信息

提供必要的信息，以利于从日志中获取代码运行时走入的逻辑分支

**正例**
```c
void f(int indicator)
{
  log("%s some flow info indicator:%d", __func__, indicator);

  if(indicator > someValue)
  {
   ...
  }
  else
  {
   ...
  }

}
```


~~**反例**~~
```c
void f(int indicator)
{
  log("%s begin run ...", __func__);

  //看日志并不容易得到运行分支信息 ???
  if(indicator > someValue)
  {
     ...
  }
  else
  {
     ...
  }

}
```

### 不同分支日志信息尽量避免重复信息

**正例**
```c
if(conditiontest)
  {
     log("process setup gracefully ...");
     ...
  }
else
  {
    log("process setup forcely ...");
    ...
  }
```


~~**反例**~~
```c
if(conditiontest)
  {
     log("the same print info ...");
     ...
  }
else
  {
    log("the same print info ...");
    ...
  }
```

> 更大范围的日志信息重复也应该被注意，这样可以保证日志信息的相对唯一、精准，利于准确分析代码位置和运行逻辑.

### 函数内日志提供函数名称等信息

**正例**
```c
void f(int indicator)
{
  log("%s: enter in with indicator:%d", __func__,  indicator);

  if(indicator > someValue)
  {
    ...
  }
  else
  {
    ...
  }
}
```


~~**反例**~~
```c
void f(int indicator)
{
  log("enter in with indicator:%d", indicator)

  if(indicator > someValue)
  {
    ...
  }
  else
  {
    ...
  }
}
```

>  日志中甚至可以提供`__func__、 __file__、__line__`等编译信息打印



# 杂项

## 频繁使用的短函数建议inline化

在1~10行之内的短小函数，如果频繁调用，可以考虑inline化

```c
static inline  __attribute__((__always_inline__)) f(...) 
{ 
  ...
}
```

## 使用并行利器静态线程变量

**正例**
```c
//can be used by multiple threads
const char* f(unsigned char bType)
{
  static __thread char buffer[512];
  // format information into buffer using bType para
  ...
  return buffer;//线程间使用内存区别开
}
```


~~**反例**~~
```c
//maybe used by multiple threads
const char* f(unsigned char bType)
{
  static char buffer[512];
  // format information into buffer using bType para
  ...
  return buffer;//多线程场景存在多线程竞争
}
```

> + 使用静态线程变量，具有静态变量和栈变量的双重优点，在多线程编程场景可有利于充分并行
  + 多核编程考虑到是核的亲和性，以及内存访问的亲和性，与多线程考量问题有别

## GNU构造函数扩展

```c
static void __attribute__((__constructor__)) f(...)
{
  ...
}
```

> 可以对于一些无依赖和无先后顺序的初始化场景，在main函数运行前自动被初始化




# 版本管理

## 应该小步提交并提交前走查代码

+ 程序员应学会应版本化管理思想，每一步一个小的变更，逐渐螺旋迭代
+ 将提交代码限定在一个小的变化中，也有利于提交前进行代码走查；原子小步提交代码的工作方法，也有利于后期使用版本管理软件进行合并、回退变更、代码走查等活动
+ 避免代码意外丢失或损坏，也应该及时提交版本服务器，而非累积大的变化提交

> 无论Git或SVN均支持本地提交，某些不适合提交正规版本服务器的变化，可以用本地版本库进行变更管理 




# 自测

## 应该注重测试

只有编码水平非常高的人，才可以一次性将代码编写正确。一般情况下，普通程序员都很难达到一次编码正确，所以，应该注重对于代码的测试。

## 尽可能用自动化测试

对于可以自动化测试保证接口或流程正确的场景，应该尽可能地形成自动化测试用例，避免重复劳作。 

> 尽量借助于测试框架进行测试代码开发，以利于代码的规范和复用

## Linux QT Creator GUI调试

在Linux图形化环境中，可以使用开源QTCreator IDE编码工具，依赖工程的Makefile文件，就可以建立起来调试环境。




# GDB调试

## 在~/.gdbinit文件扩展全局常用命令

在此文件中增加的gdb指令，会在GDB打开过程中被自动执行，例如，设置库加载路径、代码搜索路径、常用命令别名等

```bash
define ab
thread apply all bt
end

define ff
set confirm off
file a.out
set confirm on
end

define dasm
disassemble /m
end

define exit
quit
end
```

## 多线程程序获知全部堆栈

```bash
thread apply all bt
```

> 对于CPU挂死的线程，其运行堆栈，在数次输出下，均稳定在某一个堆栈跳动范围内，即可比较准确地定位到死循环或阻塞的发生地方

## 对于调试程序定制化初始化设定

```bash
gdb --command=self.gdb -p $(pidof selfApp)
```

> 通过--command参数项，设定被调试程序的自动化执行gdb指令

### self.gdb 定制样例

```bash
cd /path/to/proc
set env LD_LIBRARY_PATH=/path/to/so:$LD_LIBRARY_PATH
set args 1 57 1 1 2 somePara
file selfApp
```

## watch内存断点识别意外内存修改

```bash
watch var
watch *(int*)0x22cbc0
```

## 断点自动执行命令序列

对于频繁在断点需要执行的的手工输入命令操作，可以通过command breakpoint_num的方法提前设定制动执行的命令，避免重复。

```bash
(gdb) b do_mmap_pgoff 
Breakpoint 1 at 0xffffffff8111a441: file mm/mmap.c, line 940.
(gdb) command 1
Type commands for when breakpoint 1 is hit, one per line.
End with a line saying just "end".
>print addr
>print len
>print proto
>end
(gdb)
```

## 快速选择已输入历史命令

+ Ctrl + R 输入关键词，匹配到历史命令，要再次运行该匹配命令，请按Enter。

+ 要查找下一个匹配的命令，请再次单击Ctrl + R

+ 要编辑命令，请按左或右光标键，相当于选中后编辑



# 内存故障定位

## valgrind辅助定位

```bash
valgrind --leak-check=full --track-origins=yes ./someProc [para...]
```



# 调优

## 总则

除了常识性、规范性的优化，其它性能优化应该尽量地晚，~~不应该提前优化~~ ！！！

## perf程序性能分析工具

通过perf 工具可以对程序运行期的动态行为进行记录，通过量化的方式发现可以优化的代码位置，特别推荐!

### perf record

```bash
perf record -g -p ${pid}  [-o  </path/to/output_perfdata_file>]
```

如果不指定-o 输出文件参数，将默认输出到perf.data文件中。  
如果想形成多份不同的输出文件，易于后期比较，可以指定-o参数，设定不同的输出文件名字，例如，a.perf.data。  
补充，perf支持工具级不同性能采集数据之间的diff差异法分析。

### perf report

```bash
perf  report  [-i </path/to/perfdata_file>]
```

>  + 如果不指定性能统计分析数据，则默认寻找perf.data文件，即perf  record默认输出的文件
>  + 报告应关注两列均是红色的警醒

### perf archive脱机分析

在某些特殊场景，我们可能需要在A机器采集的性能统计数据，在B机器上分析，那么不单单需要perf.data的文件，还需要辅助性能采集统计数据，需要利用用`perf archive`子命令形成辅助文件后，一并在B机器上部署，才可以进行分析。

```bash
perf archive  [</path/to/perfdata_file>]
```

#### 其他机器分析

*采集机器上收集*

```bash
# at A machine has perf.data
perf archive

scp perf.data perf.data.tar.bz2 root@other_B_machine:~/
```

*在其他机器上重新部署*

```bash
# at other B machine
cd ~
tar xvf perf.data.tar.bz2 -C ~/.debug

# 进行查看分析
perf report
```




