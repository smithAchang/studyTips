# 缘由
以前总觉得`sed`的语法过于难懂，用的时间都不一定会想起来怎么用。

每次都需要先查找帮助，然后才能应用，但随后又忘 :(

直到今天通过`man sed`链接到`sed`官网上对`sed`的介绍，才明白它竟然是如此的简单明了，和逻辑自然！

# 提纲挈领的sed命令语法结构
```shell
# sed commands follow this syntax:

sed [options] [addr]X[flags]

```
> X is a single-letter sed command. 
>> X 是单字符操作命令，例如`s command`

> [addr] is an optional line address. If [addr] is specified, the command X will be executed only on the matched lines. 
>> 行地址可选。如果指定了，则sed仅工作做匹配的行上

> [addr] can be a single line number, a regular expression, or a range of lines (see sed addresses). 
>> 地址信息可以是单一行号、单正则表达式，或由它们组成的地址范围

> Additional [flags] are used for some sed commands.
>> 附加的标志信息可以对一些`sed`命令进行控制

*更详细的介绍，需要看官网介绍*
## 语法概念介绍

`sed`区别于`grep`搜索命令工具，`sed`只所以被称之为**流编辑器**，就在于`sed`除查找外的编辑动作，相对于`grep`还提供了流的增、删、改的功能。

而这方面，`grep`命令就显得无能为力了。

可以想象，一般我们对于文档进行编辑，遇到的首要问题是关于行的定位。

文档编辑要针对那一行，或那一段进行处理，所以，`sed`工具语法开始就首先让你提供关于**地址**的信息，其实是非常自然的

> 在语法中**地址**虽然可以忽略，则当地址信息省略后，实则命令是针对于全文档的处理。

+ 后续通过各种具体例子，会看到`sed`的命令语法结构以一贯之

**所以，`sed`命令是非常容易理解的 :)**

## sed命令语法特殊说明

> 命令部分经常使用单引号，而单引号符合`bash`对单引号引用内容的限定
> > 单引号用于保留字符的字面含义，各种特殊字符在单引号里面，都会变为普通字符，比如星号*、美元符号$、反斜杠\等

> 当然也可以用双引号，以实现变量替换
# sed选项特别介绍
+ -n
> 默认情况下，将打印每一行的执行结果，以利于重定向处理结果到结果文件中。开启`-n`选项后，结合`p`命令字可以有选择地输出行

```shell
# 不仅全部输出了，控制打印的特殊行也输出了，所以，显示了两次
seq 5 | sed '1,2p'

# 仅有第一行和第二行输出
seq 5 | sed -n '1,2p'
```

+ -i
> 选项开启后将原地编辑输入文件，并不将结果输出到标准输出

# 结束语
正常的话，如果你了解了，前面`sed`命令概念介绍，以及比较熟悉`sed`命令，看完前面基本上可以做到心领神会了。

后面只需要更专注编辑处理的逻辑概念，然后，再去拼凑适合的`sed`命令即可。

对于附录部分，可以有选择地阅读，以增加对于`sed`命令的了解。

后期会随着文档的更新，不断积累`sed`一些常用命令。

# 附录
## sed常用命令单字母
+ s
> 替换
+ d
> 删除
+ a
> 追加
+ i
> 前插
+ p
> 打印



## 地址分类
+ 数字
+ 正则表达式
+ 地址范围

> 特别地，在地址与命令字之间加入`!`，则可以实现地址反选

## 具体例子介绍
### 删除首行
```shell
seq 5 | sed  '1d'
```

> 数字行号加删除命令

### 删除最后一行
```shell
seq 5 | sed  '$d'
```

> 特殊行号符号加删除命令

### 组合命令

```shell
seq 5 | sed  '1d; $d'
```

### 截取首部行

```shell
seq 5 | sed  '2q'
```

### 地址范围反选
```shell
seq 5 | sed  '1!d'
```

> 利用分号进行命令分割。其它`-e`方式不够简洁

## s命令介绍
 本身`sed`命令地址部分就可以有多种变化，特别地，可以与正则表达式进行结合，所以，经常看到比较复杂的`sed`命令都源于此种地址的的变化，以及加上`s command`命令本身的一些负责语法！

### s命令语法
> **s/regexp/replacement/flags**

### 例子
 ```shell
 echo "hello! apple is red
 hello! banana is yellow
 hello! watermelon is green" > input.txt

 sed '/apple/s/hello/world/' input.txt
 ```

> 通过正则表达式地址定位方式，对于含有apple的行，进行搜索hello，如果匹配则替换为world
> 此例也非常典型的说明了`sed`语法结构是一以贯之的，`s comand`很好地符合了语法结构。

利用`sed`语法结构，可以很清晰地解析任何`sed`操作！


# 参考
+ [sed官网帮助](https://www.gnu.org/software/sed/manual/sed.html)
+ + [sed命令选项](https://www.gnu.org/software/sed/manual/sed.html#Command_002dLine-Options)
+ + [sed address官网章节](https://www.gnu.org/software/sed/manual/sed.html#Addresses-overview)
+ + [sed 如何工作的](https://www.gnu.org/software/sed/manual/sed.html#Execution-Cycle)
+ + [sed s command](https://www.gnu.org/software/sed/manual/sed.html#The-_0022s_0022-Command)
+ [wangdoc shell 单引号介绍](https://wangdoc.com/bash/quotation#%E5%8D%95%E5%BC%95%E5%8F%B7)