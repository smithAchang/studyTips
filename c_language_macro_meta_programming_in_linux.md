# 修改记录

版本号    |日期            | 更改理由
----------|----------------|----------------
V1.0      | 2023-10-13     | 初稿



# 文档约定
+ 一级标题前使用三个空行，二级标题前使用两个空行，其它至少一个空行
+ 代码示例内头尾无空行
+ 代码每行按照两个空格对齐



# 前言
本来想把`C`语言利用`MACRO`宏进行元编程的技巧经验，总结到《C语言编程规范》中，但是想要说的太多，体量太大，还是单独成文档，希望能够说得清楚 :)



# 元编程的定义
> 能够生产代码的代码，都可以称之为“元编程”。

在`C`语言的层面，用宏可以在**编译期**进行生产代码，所以，可以说宏是`C`语言元编程的工具！



# 宏的基础

## 定义多行
利用`\`可以对于宏定义多行的内容，定义出相当复杂度的实现。

> 因为C语言中不可以对宏方法进行调试，所以，对于宏方法中多行，尽量控制在少数，以利于代码走查就可以确定其正确性

> 通过`gcc`的`-E`选项，可以将宏展开后的结果展现出来，也非常有利于对于宏编程进行代码走查

```shell
  gcc -g3 -O0 -W -Wall -std=gnu11 -pipe -E macro.c -o macro.c.preprocess.txt
```

### 模板


## 常量宏
> 定义为符号、数字、字符串等


## 宏方法
> 宏可以类似函数，传递各种参数，但参数类型支持的比普通函数更广泛


## 临时无名宏
在`C`语言的宏惯用法中经常用`_`作为临时无名宏的名字，且通过`#define`和`#undef`语言关键词实现宏的变量化。

用户可以创建出很多不同的无名宏，进行不同的扩展和再解释，后面将展示其经典用法！

```c

#define _  ...

#undef _

```


## 宏的嵌套
+ 宏内可以使用其它宏
+ 宏方法的参数可以传递其它宏“指针”，或宏展开的结果


## 宏的展开
宏可以进行递归展开，直至没有宏的出现

> 特别地，在存在对宏参数的`#`和`##`运算时，体现为对符号的操作


## 宏与泛型
因为宏在编译期进行展开，所以，传递的类型，仅用满足“鸭子辨型法”，就可以被正确编译通过，可以看做在`C`语言领域的泛型编程。


## 宏与条件编译
根据条件编译，最简单的是控制宏定义为不同的版本，例如，`Release`与`Debug`版本



# 再谈宏元编程

## 全息宏模板

在`C`语言中，如果代码中存在重复，或某种模式，则可以定义用宏的多行定义，配置出来全信息的宏模板，以利于后期根据此配置进行展开，生成代码。


## 宏模板遍历器
经常使用无名临时宏，作为宏模板的遍历器，临时无名宏，可以看为不断变化的宏变量，可以对宏模板进行不同切面的展开，所以，需要宏模板的**全息**。

遍历器根据宏模板的指导，可以生成枚举、联合或联合成员、结构体或结构体成员、全局变量声明或定义、全局变量或数组的初始化列表、函数声明和定义等语言基本构件，基本上已经非常全面了！

### 适应场合
在代码重复度比较高的场合，例如，数据库驱动模块，特别适合用`宏元编程`去降低模块重复度，不然，重复拷贝、粘贴的代码，会引起很大的维护代价！

在元编程的加持下，新增接口时，仅用输入必要的信息，而其它可以按照宏模板进行自动生成 :)


## 宏元编程劣势
+ 不直观，对于宏方法不能调试
+ 在某些`IDE`中无法成为正常的符号，浏览代码不方便

> 如果在支持`C++`的场景，建议用模板类、模板方法来替换宏在结构体和方法中的使用。但，模板无法完全替代宏，在合适的场景，用宏来产生卫语句，提前终结流程，还是非常方便的


## 宏函数模板遍历器的设计
将固定部分，在遍历器中用固定的代码写死，如果存在差异的代码，建议在全息宏模板中以宏方法作为宏参数，进行隔离差异！

```c
#define FOREACH_TEMPLATE_DECL \
  _(name_token, T_DAT_REQ, T_DAT_ACK, REQ_SET_MACRO_FUNC, PRE_ACK_PROC_MACRO_FUNC)\


#define FEATURE1_REQ_SET_FUNC(ptReq, arg) ...


#define FEATURE1_PRE_ACK_PROC_FUNC(ptAck, arg) ...

// 利用变参，可以忽略某些不需要的参数  
#define FEATURE2_REQ_SET_FUNC(ptReq, ...) ...
#define FEATURE2_PRE_ACK_PROC_FUNC(ptAck, ...) ...


#define _(name_token, T_DAT_REQ, T_DAT_ACK, REQ_SET_FUNC, PRE_ACK_PROC_FUNC) \
  int name_token##_dao(T_DAT_REQ* ptReq, T_DAT_ACK* ptAck, void* arg);
  int name_token##_dao(T_DAT_REQ* ptReq, T_DAT_ACK* ptAck, void* arg)
  {
    int rc;

    ...
    REQ_SET_FUNC(ptReq, arg);

    ...

    PRE_ACK_PROC_FUNC(ptAck, arg);

    return rc;
  }
```



# 深入宏方法

## 宏的行为
宏表现的像常量宏，还是像函数，决定了宏在定义选用的方式。

另外，根据用的场景的不同，使用宏时，后面是否可以跟`,`或 `;`等结束符，都会影响定义时的语法

### 宏方法定义惯用法之与do{} while(0)

我们经常需要宏方法表现的像调用普通函数那样，能够声明和使用新的栈变量，且能够在宏方法后面加`;`行结束符，那么与`do {} while(0)`的配合将是完美的！

在编译优化打开的情况下，`do {} while(0)`将被优化掉，且处理好新声明栈变量生命周期，整个`do {} while(0)`没有表现的像一个循环应该有的跳转逻辑，在运行期没有任何代价的！


### 宏方法参数来源
宏方法参数可以为符号、函数、其它宏、全局变量其它宏，以及调用方的参数列表。
特别地，调用方的参数列表必须为全信息，才能比较好地应对宏参数(除固定依赖外)的传递。


### 变参宏方法
在`C99`以后，可以使用`__VAR_AGRS__`语言关键词实现宏的变参传递

```c

#define MACRO(...) // 可变数量的参数

#define MACRO1(format_str_required_para, ...) printf(format_str_required_para, __VA_ARGS__)

#define MACRO(...) printf(__VA_ARGS__)

MACRO("Hello, world!\n");
MACRO1("str para: %s, int para: %d\n", "Hello, world!", 10);
```

#### 变参宏的参数设计
变参宏的参数，分为必选(Required)、可选(Optional)，需要将必选参数放置在参数列表的左侧

### 宏参数#和##特殊处理

+ 利用`#`可以将符号转变为字符串

```c
#define TOKEN_TO_STR(token) #token

int main(void)
{
  printf("%s\n", TOKEN_TO_STR(123));

  return 0;
}
```

+ 利用`##`可以将符号对前后作用的对象进行拼接

```c
// 利用部分喜闻乐见的关键词拼凑函数签名
#define ON(event, handler) \
    void event##_handler() { handler(); }

#define EMIT(event, ...) event##_handler(__VA_ARGS__)

void my_loaded_handler() {
    printf("Loaded\n");
}

ON(load, my_loaded_handler);

int main(void)
{
  EMIT(load);

  return 0;
}
```

> 利用这样的特性，建议宏参数中以`符号`进行传递，如果以字符串传递，将符号拼接为变量名或函数名时会收到限制

## 宏与宏

+ 宏的定义可以使用其他宏
```c
#define MACRO1() printf("I'm a macro being called ...\n")
  
#define  MACRO2() \
  printf("Will use other macro ...\n");\
  MACRO1()

int main(void)
{
  MACRO2();

  return 0;
}
```

+ 宏的参数定义可以传递宏

```c
#define MACRO1() printf("I'm a macro being called by passed arg ...\n")
  
#define  MACRO2(MACRO_ARG) \
  printf("Will use other macro by arg ...\n");\
  MACRO_ARG()

int main(void)
{
  MACRO2(MACRO1);

  return 0;
}
```


+ 宏给宏传递参数

```c
#define MACRO1(value) printf("I'm a macro being called by passed arg and with input arg: %d...\n", value)
  
#define  MACRO2(MACRO_ARG, arg_for_macro) \
  printf("Will use other macro by arg and passing arg to it ...\n");\
  MACRO_ARG(arg_for_macro)

int main(void)
{
  MACRO2(MACRO1, 100);

  return 0;
}
```

> \__VAR_ARGS__变参也可以实现传递

### 作为宏参数的宏的字符化

我们知道可以通过`#`对宏参数进行字符串，但如果宏参数是一个宏，不是一个符号，那么它的字符化需要特殊步骤。
原因是可以解释的，我们知道宏的展开是一级一级深入的，所以，利用再深一级的宏，再次做一次宏展开，那么就可以将作为宏参数的宏解析出其符号，再下一级处理中，就可以被字符化了！

```c
#define MACRO1 123

#define TOKEN_TO_STR(token) #token


#define EXPAN_MACRO_ARG_TO_STR(MACRO_ARG) TOKEN_TO_STR(MACRO_ARG)

int main(void)
{
  
  printf("MACRO1: %d\n", MACRO1);
  printf("MACRO1 to STR: %s\n", TOKEN_TO_STR(MACRO1));
  printf("MACRO1 to STR v2: %s\n", EXPAN_MACRO_ARG_TO_STR(MACRO1));

  return 0;
}
```

# 元编程例子

> 例子比较抽象，关键要发现代码中的模式

```c
typedef struct  A
{
   int a;
   int b;
   int c;
   int d;
} A;

// 定义出全信息出来列表
#define FOREACH_TEMPLATE_DECL \
  _(row1_required_para1, row1_required_para2, row2_optional_para1, row2_optional_para2)\
  _(row1_required_para1, row1_required_para2, row2_optional_para1, row2_optional_para2)\

#define _(required_para1, ...) {required_para1, 0, 0, 0},
  A g_atA_1[] = {

    FOREACH_TEMPLATE_DECL

  }
#undef _ 

#define _(required_para1, required_para2, ...) {required_para1, required_para2, 0, 0},
  A g_atA_2[] = {

    FOREACH_TEMPLATE_DECL

  }
#undef _

#define _(required_para1, required_para2, optional_para1, optional_para2) {required_para1, required_para2, optional_para1, optional_para2},
  A g_atA_2[] = {

    FOREACH_TEMPLATE_DECL

  }
#undef _ 

#define _(required_para1, required_para2, optional_para1, optional_para2)  required_para1(),
  union
  {
    FOREACH_TEMPLATE_DECL
  }
    
  
#undef 


#define _(required_para1, required_para2, optional_para1, optional_para2) void required_para1();
  
    FOREACH_TEMPLATE_DECL
  
#undef 
```

> 如果依赖其它固定的符号，可以传递更多的信息
> 宏模板定义全信息列表呈现为多行，且每行建议不加任何结束符，仅在定义临时匿名宏时根据使用场景的不同，提供必要的结束符









