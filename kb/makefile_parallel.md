
# 由来
写了`后知后觉Makefile`后，觉得有必要对现存的`Make`实现进行下梳理。

为了保证梳理在一个新的认知水平上，在梳理前对以前模糊的概念，通过阅读官网的手册进行了一些必要的理论脑补，使得修补技术，更符合`Make`的原笔原意 :)


# 并行Make武器库

列举了修补使用到的技术要素

## 并行选项
+ -j[jobs] 
+ -l [load]

> 建议编译环境独立使用的时间，选择`-j`选项进行充分并行；编译环境共享使用的时间，需要考虑负载因素，建议使用`-l`选项

## 条件Make
+ `Target`依赖条件选择
+ `Target`执行`Recipes`指令进行条件执行
+ `immediate ?= deferred`变量条件赋值，选择性地赋值，以利于与集成Make环境协调

```Makefile
# 条件依赖
ifndef somecondition
    all: sometarget   
endif

# 指令条件执行
all:
ifndef somecondition
   @echo This statement will be excuted when var is not setted
endif

# 共享变量条件赋值
sharedvar ?= default

```

> 适用场景：
> + 使用条件语句使得某些依赖和某些执行指令，在集成编译的时间并不执行
> + 在某些编译条件下，例如，独立组件编译的时间，需要进行执行和变量设定

##  更准确的规则
```makefile
# 通用处理
%.o: %.c
	...

# 对于特殊编译设置的文件，可以撰写更精确匹配的规则
special.o: special.c specail.h
	...
```
## Sub-make要求

要求能够在独立的编译目录中进行编译，保证可以充分并行
## Sub-make共享动作
对于各`Sub-make`共享的动作，可以在`Parent-make`中进行先期执行，使得`Sub-make`可以充分并行起来，以避免后期重复执行，和竞争资源

## export变量传递

使得与`Sub-make`过程共享集成编译环境信息，例如，某些编译`warning`转移`ERROR`选项

## 特别地MAKEFLAGS变量

通过内置`MAKEFLAGS`变量可以实现与`Sub-make`间共享参数选项，避免再次重复手写

## 构建目标正确依赖关系

>**方法论**： 选择自顶向下的构建方法，可以将所有`Target`列举在第一个层次，然后逐渐将被依赖的`Target`下沉，直至无任何依赖。

>建议将无任何依赖的`Target`集中在Makefile文件的底部，作为`Make`构建的基石

```Makefile
.PHONY: all app lib clean

all: app
app: lib
lib: clean

# 最下面的目标无任何依赖
clean:
    @echo $@ begin to run
    @echo $@ has done
```

# 参考
+ [GNU Make Manual](https://www.gnu.org/software/make/manual/make.htm)
+ [GNU Make Parallel](https://www.gnu.org/software/make/manual/make.html#Parallel)
+ [通过变量与子Make通信](https://www.gnu.org/software/make/manual/make.html#Variables_002fRecursion)
+ [通过选项与子Make通信](https://www.gnu.org/software/make/manual/make.html#Options_002fRecursion)
+ [约定俗成的一些Target](https://www.gnu.org/software/make/manual/make.html#Standard-Targets)