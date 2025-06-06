# 修改记录

版本号    |日期            | 更改理由
----------|----------------|----------------
V1.0      | 2022-03-15     | MD化
V1.01     | 2023-02-17     | 零值和-1同时作为非法值
V1.02     | 2023-03-25     | 使用宏或内联函数降低圈复杂度
V1.03     | 2023-06-09     | 增补性能测试对日志要求；增补资源标识分配避免短时间回环的要求
V1.04     | 2023-06-17     | 用性能压力测试暴露低效代码关键区
V1.05     | 2023-08-30     | 增补`-pipe`的GCC常用编译选项，可以加快编译速度
V1.06     | 2023-09-09     | 增补字符串安全操作函数`strn*`和`sn*`系列编码建议；使用宏自动生成代码消除重复的编码技巧
V1.07     | 2023-11-04     | 增补代码风格的要求；增补enum使用经验、模块全局/静态变量提供结构体封装的编程经验
V1.08     | 2023-11-25     | 增补void*类型作为通用参数容器的建议
V1.09     | 2024-04-04     | 增补数组给定值初始化GNU编译器扩展和`-fsanitize=address`和`-fsanitize=*`等编译选项
V1.10     | 2024-05-18     | 增补栈空间可变长数组的建议;走查后修补格式等
V1.11     | 2024-07-17     | 增补对枚举成员直接赋值的建议；增补避免指定字节对齐的建议
V1.12     | 2025-04-02     | 增补健壮性处理经验:信号安全和对接口API返回值的异常处理；补充，优化查看汇编指令的必要性
V1.13     | 2025-04-16     | 增补对自测、走查的新认识；增补复杂宏函数参数的优先级问题；阅读后文档润色



# 文档约定
+ 一级标题前使用三个空行
+ 正例和反例之间空两行
+ 代码示例内头尾无空行
+ 代码每行按照两个空格对齐
+ 区分具体编码技巧与与具体代码无关的规范技巧



# 总则

**仅总结一些常用、且实用的编程规范和技巧，以及避免记忆负担，聚焦影响比较大的20% !**



# 编译器
## 打开全warning编译器开关

**正例**
```bash
gcc -W -Wall -pipe -g -o someProc main.c
```


~~**反例**~~
```bash
gcc -g -o someProc main.c
```

### 建议增加-W -Wall -pipe -g编译器开关

+ 打开`-W -Wall` 警告开关，可以让编译器尽量将代码中的告警进行展示
+ 打开`-pipe`编译选项，使得编译中间结果用管道传递，避免中间临时文件的磁盘IO操作，可以提高编译效率 
+ 打开`-g`开关，以利于在出错的情况下，可以较为准确地获知堆栈信息

>  `-pipe`
>           `Use pipes rather than temporary files` for communication between the various stages of compilation.
>            This fails to work on some systems ~~where the assembler is unable to read from a pipe~~; 
>            **but the GNU assembler has no trouble.**


### 持续消除warning

在warning开关全开的情况下，应分析检查产生warning的情况，最好做到零warning。

代码中出现warning代表着对编程语言没有掌握的知识，可以通过对于warning的研究学习，增强编程能力。

### 建议将某些warning转为编译error

+ 可以根据工程特点，总结出来一批，影响比较大，而且容易出现问题的warning，直接转化为编译器error选项，以利于用编译工具，自动保证代码规范和质量
+ 有条件的项目编译工程，可以通过-Werror将全部warning转为编译error。

**正例**
```bash
gcc -W -Wall -pipe -Werror=overflow -g -o someProc main.c

或

gcc -W -Wall -pipe -Werror [-Wno-*...] -g -o someProc main.c
```

> 即使在-Werror开关的情况下，依然可以通过-Wno-\*的编译选项进行定制排除，将某些影响不大的warning排除


## 使用高版本编译器

> 借用高版本编译器，往往能够检查出代码中更多的重大隐患，和产生更高质量的代码。建议在能够使用高版本编译器的情况下，尽量、尽早使用

### 建议使用docker构建高版本编译器环境

在docker某基础镜像中加入devtoolset-\*组件，对于CentOS系列发行版，建议安装devtoolset-7或devtoolset-9。这样对于仍然依赖旧编译器的工程，可以无影响地进行高版本编译编译，避免交叉影响。


**启动镜像时脚本**

```bash
docker run -it --rm -h devtoolsetVersion -w /usegccversion -v $PWD:/usegccversion image bash
```

## 使用高版本编译标准

> 在低版本编译器中，尽量使用高版本编译标准，可以使用到某些友好的编译特性和编译器错误检查

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

> + gcc编译器可以指定--std=gnu11等一些高版本编译标准。临近使用变量特性将使得封装性更好，避免临时变量的提前干扰，有利于学习阅读代码
> + `Linux`内核未来也要转向`C11`的编译语言标准

### 建议O2优化标准

编译器优化可以将代码运行性能加速到极致；但为了避免一些负优化，建议使用与Linux内核一致的O2优化级别。

> 工具优化虽在，我们仍提倡撰写高质量的代码，作为比较好的编译器输入，提高编程能力和编译速度！

#### 适当地方使用register变量

 C语言支持寄存器变量，`x86-64`上又提供更多的寄存器可用，所以在计算校验和等比较密集耗费CPU的场景，特别仅利用寄存器就可以完成的计算部分，建议对于关键变量建议使用register关键词修饰。



# 编程规范

## 以人为本

### 利用代码风格格式化工具实现统一的代码风格
  业界存在很多种代码风格格式化的工具，它们之间没有好坏之分，只有喜欢不喜欢，习惯不习惯的问题。

  而且，每种代码风格均有所依据，都有其道理，使用多了就习惯了，也能体会代码规范之美。

  这里的关键是代码风格上的一致和统一！

  > + 编码风格使用工具进行格式化，可以避免手工维护的低效和不统一

  > + 很多开发IDE都提供了与各种代码风格工具的集成能力，例如，uncrustify
  
  ```bash
  uncrustify -c linux.cfg --no-backup somefile.c
  ```

#### 源码编辑Tab转为空格

+ 编辑器设定Tab键空格数量为4，或更紧凑的布局，设定为2
+ 代码编辑器"设置"支持将Tab固定转为空格
+ 代码编辑器一般都支持将旧文件中Tab批量转为空格

> 如果使用代码风格格式化工具，此条建议可以自动实现

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

> 以人为本的理念，在编程实践时，如果对于机器阅读没有困难，而对于程序员阅读存在困难的代码，都应该通过空格、空行分割、打印日志分割、scope对齐等常用方式，进行无注释性提示编码。

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

> { }分割有助于人眼分辨，而不是机器可以正确处理。在后期if分支修改时，也不容易引起维护上的问题。

### 用const关键词修饰栈变量和函数入参指针参数

+ 用编译器拦截一部分意外修改
+ 增加编译器优化的深度，例如，对于不变量可以进行深度优化
+ 对于const类型的数据量，熟练阅读代码的人，可以有选择地忽略和聚焦重点到易变化的代码部分

> 多数较为新的C语言开源组件，在API接口设计，多遵从const 修饰符原则，很容易区分**入参**、**出参**

### 巧用<>和""包含头文件区别依赖可变性

**正例**
```c
// system api
#include <sys/time.h>

// third-party api
#include <third-party/time.h>

// self module api
#include "otherApiInModule.h"
```


~~**反例**~~
```c
// system api
#include <sys/time.h>

// third-party api
#include "third-party/time.h"

// self module api
#include "otherApiInModule.h"
```

> 原则上有区别的差别，最好采用*有区别*的写法
>
> 从书写字面区分头文件组件内外不同，显示引用API的可变程度不同，对于后续阅读代码有利

### 分支语句尽量采用likely/unlikely标注

**正例**
```c
if(likely(conditiontest))
{
  // most hit code
  ...
}
else
{
  // less hit code
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

> + likely/unlikely提高代码可阅读性，自然关注主要逻辑流程
> + 让编译器生成代码对于分支预测机制更为友好

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

  // normal flow
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

  // normal flow
  ...
}
```

### typedef有名结构体

**正例**
```c
typedef struct tagPerson
{
  unsigned char name[MAX_NAME_LEN];
  int age;
} Person;
```


~~**反例**~~
```c
typedef struct
{
  unsigned char name[MAX_NAME_LEN];
  int age;
} Person;
```

> + typedef结构体不具有名称，则在旧版IDE编辑器中，会造成输入辅助提示的混乱
> + 在typedef命名的新名称前加入"tag"作为结构体的名称的前缀，也是一种惯用法

### 尽量使用枚举enum且声明必须含有名字

**正例**
```c
typedef enum WEEK
{
  SUN = 1,
  MON = 2,
  ...
} WEEK;
```


~~**反例**~~
```c
enum
{
  SUN,
  MON,
  ...
};
```

> + 枚举代表某种概念集合，相比较于独立的宏定义，通常更适合适合定义常量序列、限定合法值等相互之间有关联的场合
> + 枚举声明的名字，不影响枚举值类似宏定义的直接使用方式；但能够在阅读代码时更方便了解其含义，应避免使用无名枚举值
> + 直接对于枚举成员赋值，特别是大量错误码或返回值声明场景，以利于能够在日志中分析进行直接对应
> + 编译器对于枚举值友好，作为函数参数，将在编译器期能够校验入参的合法性
> + 调试对于枚举值友好，能够对于值直接显示更有意义的声明名称，避免增加代码翻译

### 代码体行数限制

200行为**上限**，*太长*则需分拆

> 首先是代码逻辑上梳理清晰，然后就可以分步骤、分组件，分为3~5个概念步骤，理解最顺畅

### 让规范代码模式重复出现

在维护过程中，由于历史存量的原因，规范的代码和不规范的代码并存，应尽量批量修改掉某种范式的不规范代码，以避免将来拷贝粘贴代码进一步扩大"污染"。 

> 例如，`sprintf`字符串函数可以自动补零，但是对于拷贝目的地的长度没有保护，所以，在涉及的地方，几乎可以无危险代价地替换为`snprintf`函数 。

**可以使用代码编辑器中提供的调用关系Graph、读写引用位置、正则表达式等工具批量发现修改涉及点**

### 避免简单重复代码出现

此条建议看似与前面建议冲突，实则不然。简单重复的代码是代码中最大的坏味道，和体现程序员的懒惰，一时快，长时悔，后面带来无穷无尽的维护麻烦，和已投入的沉没成本！  
当某些代码重复出现三次以上时，是应该考虑用公共组件来代替简单重复的时候了，这时候规律也就自现了。

### 敢为人先避免破窗效应

~~破窗效应~~是指某个地方出现了"坏"的味道后，后面这种坏的情况在自然情况下，没有人愿意改变，只能变得更糟！  
作为程序员，应该敢于迈出第一步，逐步让代码变得比昨天更好。

在此过程中，建议**小步提交、[用例验证]、每步走查、逐步自举**，慢慢一个大的变化，习惯了后，对于重构技巧的掌握会更精深。

### 注重重构技巧
重构代码也是有方法论的，具体可见《重构改善即有代码的设计》。  
在编译器、自动化测试用例保护、以及重构指导步骤的导引下，进行代码重构，尽量降低出错概率，给自己或他人增强对变动代码的信心。


## 注释

### 进行必要的简短注释

注释的两个极端是没有注释或充满注释，都不是很好的策略，仅进行必要的注释。

### 不简明代码的注释等同于一句道歉

对于不简明的代码进行注释，实际上等同于道歉！
说明这些代码太复杂了，作者怕后来阅读者不能尽懂，不得不用注释进行特殊说明。

可以考虑是否有简明的实现，进行代码重构，代码自身就是最好的注释。

> 来自网络观点的启发

### 如果注释的地方可以增补日志用日志代替

在避免重复的原则下，如果同时出现注释和日志，那么用日志代替注释即可，而且日志可以打印更完整的运行时信息，也利于事后分析问题。


## 编码技巧

### 避免指定字节对齐编译指令

**正例**
```c
// c header file
typedef struct tagSome_t 
{
   char field1;
   short field3;
   int field4;
   double field2;
   ...;
} Some_t;

```

~~**反例**~~
```c
#pragma pack(push, 1)

typedef struct tagSome_t 
{
   char field1;
   double field2;
   short field3;
   int field4;
   ...;
} Some_t;

#pragma pack(pop)

// Or

typedef struct tagSome_t 
{
   char field1;
   double field2;
   short field3;
   int field4;
   ...;
} __attribute__((packed)) Some_t;

```

#### 应对方法
> + 编译默认对齐方式已足够好，相当于**自动**处理结构体成员的字节对齐问题，但可能有结构体膨胀风险
> + 混合、嵌套不同字节对齐的结构体，或扩展结构体新成员，在不注意间会引发不必要访存性能损失。对大规模协作开发提出了更高要求

> + 除了网络协议，或外部接口要求的字节对齐硬性要求，其它场景尽量避免指定字节对齐方式
> + 在大量创建膨胀结构体的场景，在保证结构体成员语义相关和内聚的前提下，可通过调整成员声明顺序，来消除不合理的声明顺序，引发的结构体内存膨胀问题


#### 计算2的幂次内存长度的快速方法
```c
#define ROUND_UP4(n) (((n) + 3) & ~3)

#define ROUND_UP8(n) (((n) + 7) & ~7)

#define ROUND_UP(n) (((n) + (sizeof(void*) - 1)) & ~(sizeof(void*) - 1))

```

### 模块内提供对全局/静态变量的结构体封装

**正例**
```c
// c source file
#define MAX_NAME_LEN ((uint16_t)256)
typedef struct tagModuleMain_t 
{
   int iExitFlag;
   char szName[MAX_NAME_LEN];
   ...;
} ModuleMain_t;

static ModuleMain_t s_tMain;
```

~~**反例**~~
```c
#define MAX_NAME_LEN ((uint16_t)256)

static int  s_iExitFlag;
static char s_szName[MAX_NAME_LEN];
...;
```

> 对于模块内使用的全局/静态变量，提供结构体封装，将更有利于代码组织和阅读，以及减少~~全局变量污染~~

> 如果全局/静态默认值不符合使用要求，可以模块提供init/fini对外初始化和销毁接口
>> 如使用`GCC`编译器，也可以考虑使用`__attribute__((__constructor__))`修饰符，设定模块初始化函数

> 对于**全局变量**同样可以考虑如此实践，仅将结构体封装的全局变量提供出去

### 可变长数组

**正例**
```c
void f(unsigned int len)
{
  char szName[len];
  ...
}
```

~~**反例**~~
```c
void f1(unsigned int len)
{
  char* szName = (char*)malloc(len);
  ...
  // 注意需要考虑多分支return
  free(szName)
}

#define MAX_NAME_LEN_FOR_ALLCASE ((uint16_t)512)
void f2()
{
  // too long for all case
  char szName[MAX_NAME_LEN_FOR_ALLCASE];
  ...
}
```
> + `Arrays of Variable Length`在`ISO C99`中作为语言标准，可以提高编译器的编译标准进行支持
> + 相比较于栈溢出风险，能够将代码写的更简洁的技术，避免啰嗦，总是值得推荐

### 使用零长度数组作为类TLV结构体成员

**正例**
```c
// can be used as base pointer
struct A
{
  unsigned int type;
  unsigned int len;
  
  unsigned char data[0];
};

struct B
{
  struct A head;
  int i;
  int j;
  int k;
};

struct C
{
  struct A head;
  int h;
  int m;
  int n;
};

struct Msgs
{
  union
  {
    struct A a;
    struct B b;
    struct C c;
  };
};

...

unsigned char encodeBuf[2048];
struct A *a = (struct A *)encodeBuf;

...

memcpy(a->data, src, copyLen);

...

struct B *b = (struct B *)encodeBuf;

...

memcpy(b->head.data, src, copyLen);
...
```

> 适用于广泛的TLV内存操作中，避免在运行期动态计算指针偏移，将在编译期确定

#### 网络收发报文缓冲区预留前缀后缀空间的最佳实践

**正例**
```
----------------------------------------------------------------------
  ... prefix space  | | | | | | | | | | suffix space ...           
----------------------------------------------------------------------
^                   ^                 ^                              ^
|                   |                 |                              |
base                rd_ptr            wr_ptr                         capacity
```

```c
  unsigned char base[MORE_BIGGER_THAN_PKT_SIZE];
  // reserve prefix space
  rd_ptr = wr_ptr = prefix_offset;
  ...
     
  nsize = sock_recv(base + wr_ptr, capacity - prefix_offset);
  wr_ptr += nsize;
  
  rd_ptr -= sizeof(extended_prefix_info);
  add_some_extended_prefix_info(base + rd_ptr);
  
  
  add_some_extended_suffix_info(base + wr_ptr);
  wr_ptr += sizeof(extended_suffix_info);
  
  // so can avoid the memory copy if adding extended info
  sock_send(base + rd_ptr, wr_ptr - rd_ptr);
```

> 通过预留编解码缓冲区前后缀空间，可以在收发外部网络报文后，再次封装或加入扩展时，避免反复内存拷贝

### 使用空{}清零初始化结构和数组栈变量

**正例**
```c
#define SMALL_SIZE ((uint8_t)16)
#define BIG_SIZE ((uint16_t)512)

// default is OK. Will be alloced at bss segment
static unsigned char s_buffer[BIG_SIZE];
// alloced at data segment
static unsigned char s_bufferFF[SMALL_SIZE] = {[0 ... (sizeof(s_bufferFF) - 1)] = 0xFF};

void f()
{
  struct A a = {};
  unsigned char buffer[BIG_SIZE] = {};
  unsigned char bufferFF[BIG_SIZE] = {[0 ... (sizeof(bufferFF) - 1)] = 0xFF};

  // some code using the above vars
  ...
}
```


~~**反例**~~
```c
void f()
{
  struct A a ;
  unsigned char buffer[BIG_SIZE];
  unsigned char bufferFF[BIG_SIZE];

  memset(&a, 0, sizeof(a));
  memset(buffer, 0, sizeof(buffer));
  memset(bufferFF, oxFF, sizeof(bufferFF));

  // some code using the above vars
  ...
}
```

> + 空花括号清零简洁、美观、高效
> + 小块内存清零应避免调用函数的代价。在`C23 C`语言建议标准中，已作为标准建议替代memset
> + 空花括号和花括号带零，适应不同的情况；空花括号清零可以应对结构体和结构体内含有结构体成员的复杂结构体
> + 可以使用GNU编译器扩展的`array range intializer [first ... last] = value`将数组元素清理为特定值，代替[memset](https://gcc.gnu.org/onlinedocs/gcc/Designated-Inits.html)

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

// normal code flow
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

### 使用宏或内联函数减轻圈复杂度

**正例**
```c
#define PARA_CHECK_EXCEPTION_RETURN(expr, rc, exception_log_format, args...) do {\
     if(expr)\
     {\
        printf(exception_log_format"\n", ## args);\
        return rc; \
     }\
}while(0)

// use as gloabl but temporary scan timer
static int s_RetryTimerId = -1;

static inline void startRetryTimer(...)
{
  addSomeControlInfoToTimerGlabalDataStructure(...);
  
  if(s_RetryTimerId == -1)
  {
    s_RetryTimerId = new_timer(...);
  }
}

int f(const char* data)
{
   PARA_CHECK_EXCEPTION_RETURN(unlikely(data == NULL), -1, " %s input para error!", __func__);
   
   // some service code
   ...
   
   if(isNeedRetryLately)
   {  
      startRetryTimer(...);
   }
   
   return 0;
}
```


~~**反例**~~
```c
int f(const char* data)
{
   if(unlikely(data == NULL))
   {
      printf("%s input para error!\n", __func__);
      return -1;
   }
   
   // some service code
   ...
   
   if(isNeedRetryLately)
   {  
      addSomeControlInfoToTimerGlabalDataStructure(...);
  
      if(s_RetryTimerId == -1)
      {
        s_RetryTimerId = new_timer(...);
      }
   }
   
   return 0;
}
```

> + 形式上减少了if语句的嵌套，函数名字从字面上更体现了代码意图
> + 宏函数相对于内联函数，可以在需要终结流程继续执行，进行提前返回的时间更适合使用

### 用括号避免复杂宏函数参数在扩展后优先级问题

**正例**
```c
#define GetPtr(T, ptr) ( (ptr) != NULL ? (T*)(ptr) : NULL)

unsigned char abArr[16];

unsigned short* pShort = GetPtr(unsigned short, (unsigned char*)abArr + 1)
```


~~**反例**~~
```c
#define GetPtr(T, ptr) ( (ptr) != NULL ? (T*)ptr : NULL)

unsigned short* pShort = GetPtr(unsigned short, (unsigned char*)abArr + 1)
```

> 在反例中，宏扩展后因为**操作符的优先级**，将会出现逻辑错误
> 
> 在宏参数为表达式时，最容易出现，需要在宏函数设计时进行评估。或者懒惰点，一律加括号

### 使用宏自动生成重复代码

> 如果从减少代码行数的标准来看待代码质量，使用宏自动产生代码，消除重复代码，将是非常好的语言工具

>> + 宏类似C++语言中的模板，具有弱的接口要求，类型扩展上有**鸭子辨型法**的特点

>> + 如果宏“模板”逻辑是稳定可靠的，那么编译器产生的代码也是可靠的

### 枚举序列定义最后一个未使用值作为边界

**正例**
```c
#define MAX_STUDENT ((uint16_t)1024)
typedef enum STUDENT_TYPE
{
  BOY = 0,
  GIRL= 1,
  STUDENT_TYPE_END 
} STUDENT_TYPE;

uint16_t usHeight[STUDENT_TYPE_END][MAX_STUDENT];
```

> 此建议存在争议，没有反例，是否使用，根据方便和喜好

> 此条经验对于声明数组会节省一个宏定义的声明，而且可以自适应版本间的变更

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
   // some usage
   f(ptA);
}
```


~~**反例**~~
```c
extern struct A*  getASingleton();

while(TRUE)
{
   // some usage
   f(getASingleton());
}
```

### 应使用简明数值操作写法 

在算术操作，尽量使用++、--、+=、\*=等简写语言操作符；一方面书写简明，另一方面以利于编译做优化。


~~**反例**~~
```c
int offset = initValue;

...

// 在编译器不能很好优化代码情况下，可以能会多一次访存操作
offset     = offset + 1;
```

### 使用匿名联合代替单一命名以利于多用途操作

**正例**
```c
typedef struct tagIPv4_Addr
{
  union 
  {
    unsigned char abAddr[4];
    unsigned int dwAddr;
  };
} IPv4_Addr;

void f()
{
  IPv4_addr ip ;
  unsigned char buffer[512];

  // for whole value usage
  printf("encoding IPv4 Addr Value:%u\n",  ip.dwAddr);
 
  // for byte operation
  memcpy(buffer,  ip.abAddr,  sizeof(ip.abAddr));
}
```


~~**反例**~~
```c
void f()
{
  unsigned char  ip[4] ;
  unsigned char buffer[512];

  // for whole value usage
  printf("encoding IPv4 Addr Value:%u\n",  *(unsigned int *)ip);
 
  // for byte operation
  memcpy(buffer,  ip,  sizeof(ip));
}
```

> 使用匿名联合几乎无代价，和更简明。在某些不便修改已有代码的地方，可以用匿名联合做成员的别名访问

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

// 编译器生成代码有点出乎意料，允许负值的产生
if((bEndIndex  -  bStartIndex  ) >  MAX_CAPACITY)
{
  exception_log(...);
  return FALSE;
}

// error 无法防止负值的出现
... 
```

#### 使用安全的字符串操作函数

**正例**
```c
#define PARA_MAX_LEN ((size_t)64)

void f(const char* szPara)
{
  ...
  size_t  len = strnlen(szPara, PARA_MAX_LEN);
  ...
}
```


~~**反例**~~
```c
void f(const char* szPara)
{
  ...
  // 在某些外界输入或恶意攻击场景，将会内存访问越界
  size_t  len = strlen(szPara);
  ...
}
```

> C语言字符串以零结束的设计，使得不限定最大范围的字符串操作，在不安全的场景下，特别容易出现风险
>
> 强烈建议`strn*`以及`*sn*`系列字符串安全操作函数，例如，`snprintf`、`snscanf`等，特别在输入不可信时

#### 使用void指针作为通用参数容器

此技巧特别在某些请求、响应事务型处理，需要上下文关联的场合，使用`void*`作为通用参数容器，更有利于扩展

**正例**
```c
typedef struct tagT_Req
{
  union
  {
    void* cookie;
    unsigned int slot;
  };

  ...

} T_Req;

typedef struct tagT_Rsp
{
  union
  {
    void* cookie;
    unsigned int slot;
  };
  
  ...
} T_Rsp;
```


~~**反例**~~
```c
typedef struct tagT_Req
{
  int   cookie;
  ...
} T_Req;


typedef struct tagT_Rsp
{
  
  unsigned int slot;
  ...
} T_Rsp;
```

> `void*`指针类型，其存储代表机器字大小，可以作为通用容器存储多种类型的值，有利于自动适用不同的硬件平台。
>> 用具体类型作为请求和响应的上下文的关联，在跨硬件平台时，有些信息并不能完成充分的传递
>
> 联合使用`匿名union`技巧，可以将具体使用场景，定义为更有意义的名字


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

  // some declarations
  ...

#ifdef  __cplusplus
 }
#endif

#endif
```


~~**反例**~~
```c
// a header file
// some C language declarations
...
```

#### 头文件内声明顺序

**正例**
```c
#ifndef __XX_YY_h
#define __XX_YY_h

// 包含系统或第三方不可变头文件,用惯用的尖括号包括
#include <...>
...

// 包含模块内部依赖头文件
#include "..."

// 模块自身的声明
#ifdef  __cplusplus
extern "C" {
#endif

  // 常量宏
  #define  CONST_MACRO_QUEUE_MAX_LEN  (int)16
  ...

  // 枚举值
  ...

  // 类型定义 typedefs or structs
  ...

  // 全局变量或静态变量

  // 宏函数
  ...

  // 普通函数
  ...

#ifdef  __cplusplus
 }
#endif

#endif
```

> 原则上应该按照模块自身搭建顺序，作为头文件内条目的声明顺序，以体现元素间的依赖关系

> 此处着重`宏方法`应该与普通方法在同一区域中，因为它的操作对象和普通函数一样为它前面声明的元素

### 稳定库API设计技巧利用上下文对象指针和Getter/Setter操作函数

**正例**
```c
#ifndef __XX_YY_h
#define __XX_YY_h

#ifdef  __cplusplus
extern "C" {
#endif

// some declarations
struct tag_T_Context;
typedef struct tag_T_Context T_Context;

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


**反例**
```c
#ifndef __XX_YY_h
#define __XX_YY_h

#ifdef  __cplusplus
extern "C" {
#endif

// struct declarations
typedef struct tag_T_Context 
{ 
  int field1;
  int field2;
  ...
} T_Context;

#ifdef  __cplusplus
 }
#endif

// some.c

void f()
{
   
   T_Context   tContext = {};
   // without Setter
   tSetter.field1 = 1;

   // without Getter
   printf("%d\n", tSetter.field1);
   ...
}

#endif


> + 这样的设计让上下文对象成为可以保证不同实现间的相互隔离、独立、并发并行，
> + 不操作具体内存相关的字段，具有二进制布局依赖独立性，可以视作C语言编程领域内对**接口**进行编程
> + 特别适合于库API的场景


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

  // 看日志并不容易得到运行分支信息 ???
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


### ERROR级别日志输出能够支持性能测试

>  高于和等于ERROR级别的日志，都必须为非频繁打印日志，以利于在性能测试情况下，可以开启ERROR日志级别控制日志输出，而非全部关闭掉日志

>  在性能测试出现问题时，必要的日志信息将非常有利于分析问题

>  当然，此条建议建立在异常过程日志都存在日志打印的基础上，需要有良好的习惯



# 杂项

## 资源标识分配避免短时间内回环

>  资源标识分配应充分避免短时间内回环。标识信息在短时内回环，对于类似日志分析等常见开发场景相当不利


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
// can be used by multiple threads
const char* f(unsigned char bType)
{
  static __thread char buffer[512];
  // format information into buffer using bType para
  ...
  return buffer;// 线程间使用内存区别开
}
```


~~**反例**~~
```c
// maybe used by multiple threads
const char* f(unsigned char bType)
{
  static char buffer[512];
  // format information into buffer using bType para
  ...
  return buffer;// 多线程场景存在多线程竞争
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

+ 程序员应学会应版本化管理思想，每一步提交一个小的变更，然后逐渐螺旋迭代
+ 将提交代码限定在一个小的变化中，也有利于在变更提交前，进行代码走查，也因为变化规模较小，会有利于进行代码走查
+ + 原子小步提交代码的工作方法，同时有利于后期使用版本管理软件进行分支合并、回退失败变更、同行代码走查等活动
+ 从避免代码意外丢失或损坏的角度出发，也应该及时将变更提交到版本服务器，而非累积大的变化后才提交

> 无论Git或SVN均支持本地提交，某些不适合提交正规版本服务器的变化，可以用本地版本库进行变更管理 



# 自测

## 应该注重测试

只有编码水平非常高的人，才可以一次性将代码编写正确。一般情况下，普通程序员都很难达到一次编码正确，所以，应该注重对于代码的测试。

## 尽可能用自动化测试

对于稳定、缺少变化的`API`，必须要考虑用自动化测试的方法，来保证接口或流程正确的场景，避免重复测试的低效劳作。

+ 没有对事物**形而上学**的认识，就缺乏对它的深刻把握
+ 测试代码的稳定性，代表了对**需求**或`API`的深刻分析和认识
+ + 在`TDD`开发流程的保证下，可以比较随意、且有信心地修改重构代码，去除代码中的**坏味道**

> 尽量借助于测试框架进行测试代码开发，以利于代码的规范和复用

## Linux QT Creator GUI调试

在`Linux`图形化环境中，可以使用开源`QTCreator IDE`编码工具，仅依赖工程的Makefile文件，就可以建立起来调试环境。



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
>
> `CPU`是否出现冲高也可以作为辅助观察手段

## 对于调试程序定制化初始化设定

```bash
gdb --command=selfApp.gdb -p $(pidof selfApp)
```

> 通过--command参数项，设定被调试程序的自动化执行gdb指令

### selfApp.gdb 定制样例

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
> 适合全面、低速测试场景，而且被测试应用不需要出具特殊编译版本

## AddressSanitizer辅助定位

```bash
   gcc -O0 -g -pipe -fsanitize=address main.c ...
```

> 利用GCC的编译功能扩展选项，开启编译器自带的内存检查工具`asan`库，进行检查内存错误
>
> 此种手段适合需要快速运行的场合，但需要出定制编译版本
>
> 更高的编译器版本携带更多的`-fsanitize=*`功能选项，建议使用高版本编译器



# 健壮性经验

## 考虑外部接口输入不可能的场景
 外部接口输入数据，在对端伙伴出错的情况下，很有可能携带异常值，不能认为不会出现，而不进行预防性处理。
 就像`1+1`在算错的时间等于`3`一样，应考虑异常情况的`完备性`处理。

+ 对于外部接口，应避免`未定义行为`，对于值域应该进行严格的校验
+ 建议在打印高级别日志的情况下，终结掉外部输入`会话`，以利于重建`会话`，和让隐患更加明显

> 例如，对于`sctp`用户态协议栈，如果对端伙伴输入有序报文请求，但在传输序号`TSN`有序的情况下，流序号`SSN`却出现了不能准确匹配期望的情况下，应视为异常

## 信号异常
 在`c++`通常有是否异常安全的考量，对于`c`语言程序而言，信号异常安全考虑也是必须的。特别地，在`Linux`系统中，对于支持程序优雅退出，非常必须的`TERM`信号，在程序有自身内态，需要持久化动作时，必须进行自定义处理。

### 接口返回值的规范化处理

+ 对于接口api具有指示正常和异常的情况，必须对于返回值进行异常处理

*正例**
```c
// 
void* someThreadProc(void* arg)
{
  while(!g_bExitFlag)
  {
    int lefted = sleep(interval);

    // consider the safety for signal
    if(g_bExitFlag)
    {
      break;
    }

    // another choice
    if(lefted > 0)
    {
      continue;
    }

    someAction(...);
  }
  return NULL;
}
```


~~**反例**~~
```c
//  sleep 'system api' maybe be interupted by 'TERM' signal when exiting
void* someThreadProc(void* arg)
{
  while(!g_bExitFlag)
  {
    sleep(interval);
    // ignoring the return value or signal has a risk of processing some service when exiting
    someAction(...);
  }
  return NULL;
}
```



# 调优

## 总则

除了常识性、规范性的优化，其它性能优化应该尽量地晚，~~不应该提前优化~~ ！！！

## 在合理性能模型下暴露程序性能损失关键区

> 通过对市场需求、部署硬件环境等各方面分析后，合理确定性能测试模型，然后在此性能模型下，进行压力性能测试，并通过专业工具定量分析程序性能损失区域
>
> 优化通常体现为深度的思考，费时、费力，所以，对在合理模型下暴露出来的低效进行优化，才是必要的。不然，容易陷入~~过度自嗨的迷失~~
>
> 优化类似果树上的果实，越到后期，很少有低矮的、全局性的优化；所谓~~优化~~，很有可能仅是针对特定场景，特定应用的**偏化**，尽量用配置策略进行解决

## 查看汇编指令分析优化效果
+ 建议必要时查看汇编指令，以确定优化的效果
+ 优化效果在`图灵机`的机器结构底层看来，**标准**必须是更少的指令、更少的内存读写、更局部化的内存访问

> [Linux为什么那么快？SCTP协议栈为减少一条指令的优化提交](https://github.com/torvalds/linux/commit/182691d0998400f35ad304718024e60feaa864aa#diff-e977da7d91d5b6af97d8f596daf2393c07f078e050c7389c6e2fafa9e9d2e4d4R313)

## perf分析程序性能分析工具

通过`perf`工具可以对程序运行期的动态行为进行记录，通过量化的方式发现可以优化的代码位置，特别推荐!


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



# 持续更新
[我在Linux中C语言编程经验总结](https://github.com/smithAchang/studyTips/edit/master/c_language_coding_guidelines_in_linux.md)