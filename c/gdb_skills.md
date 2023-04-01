# 修改记录

版本号    |日期             | 更改理由  
----------|----------------|----------------
V0.8      | 2023-04-01     | MD化



# 总则

**仅总结一些常用且实用的GDB调试技巧，且避免记忆负担，聚焦影响比较大的20% !**

# 术语
术语                                       |说明                 | 
--------------------------------------------|-------------------------------------------|
PRE    |  表示是在进入GDB交互Shell前执行OS Shell命令|
POST   |  表示进入GDB交互Shell后可执行命令|
MULTI  |  跨行|
arg...  |  表示命令参数多个|




# 编译器

## 打开-O0和-g3开关

+ -O0保证代码行的一致性，避免优化所带来的的调试麻烦
+ -g3产生更详细的调试信息，例如，宏的展开

# 常用技巧

## 前置动作

命令                                         |类别   |说明             | 
--------------------------------------------|----------------------|----------------------|
gdb                                         |PRE| 启动gdb交互Shell        |
gdb /path/to/program                      |PRE| 指定调试的程序，启动交互Shell        |
gdb att pid                                  |PRE| 附加到当前运行的的程序 |


## 基本调试命令

命令                                         |类别   |说明             | 
--------------------------------------------|----------------------|----------------------|
set args  arg1...                                       |POST| 设定调试程序的参数        |
r                                       |POST| 运行调试程序        |
b                                       |POST| 设定断点命令，支持线程号设定和条件断点        |
d <break_point-num or break_points-rangle>                                       |POST| 删除所有断点或某一断点或某一范围断点        |
enable  break_point_num                                        |POST|        使能某一断点  |
disable break_point_num                                        |POST|       暂时关闭某一断点  |
ignore break_point_num count                                        |POST|       设定断点忽略命中次数；注意可以设置为零  |
c                                        |POST|       继续运行  |
s                                        |POST|       进入子过程  |
finish                                   |POST|       跳出子过程  |
until line_num                                       |POST|       运行到某一行号停止  |
bt                                        | POST | 查看当前线程的堆栈 |
kill                                       |POST| 杀死调试程序        |
quit                                       |POST| 退出GDB程序        |
att pid                                    |POST| 动态调试某程序        |
detach                                    |POST| 脱离调试程序        |
file /path/to/program                     |POST| 重新加载调试程序文件，以利于编译后在不关闭GDB的情况下继续调试        |
dir                                       |POST| 清空缓存，结合file命令使用，可以使得调试程序与代码行的对应更准确，特别是在不重新启动GDB时        |
p                                         |POST| 查看变量或某内存地址内存、寄存器等|
pt                                        |POST| 查看变量或宏定义的类型|


### 查看技巧

命令                                         |类别   |说明             | 
--------------------------------------------|----------------------|----------------------|
p /d operand                                       |POST| 以十进制查看，特别是查看某些枚举值的值        |
p /x operand                                       |POST| 以十六进制查看        |
p array@count                                      |POST| 连续查看数组多少个元素        |
p *arrayPtr@count                                      |POST| 连续查看指针数组多少个元素的内容        |
p sizeof(int)                                      |POST| 查看类型的sizeof        |
call strlen(szStrVar)                                      |POST| 调用当前上下文可以使用的函数进行显示        |
command    break_point_num                                   |POST| 设定断点命中后自动执行的动作，例如，p命令等        |


### 输入历史命令
命令                                         |类别   |说明             | 
--------------------------------------------|----------------------|----------------------|
show commands  [count]                         |POST| 显示最近输入命令        |
ctrl + r                                       |POST| 在历史输入命令中搜索匹配的，敲击enter重新输入       |
-                                              |MULTI| 要编辑匹配命令，请按左或右光标键，相当于选中后编辑       |
       |



# 高级技巧

## 内存断点和修改变量

命令                                         |类别   |说明             | 
--------------------------------------------|----------------------|----------------------|
watch  [var or addr]                                    |POST| 观察变量或内存地址的读、写操作        |
rwatch  [var or addr]                                    |POST| 观察变量或内存地址的读操作        |
set  [var or addr]=value                                    |POST| 设定变量或内存地址的值        |


## 执行OS Shell

命令                                         |类别   |说明             | 
--------------------------------------------|----------------------|----------------------|
shell  command                              |POST| 执行OS Shell命令，例如，重新make程序；获取进程号等        |
[!clear or shell clear]                      |POST| 清理GDB以往显示输出


## 自动加载调试脚本

命令                                         |类别   |说明             | 
-----------------------------------------------------|----------------------|----------------------|
vi ~/.gdbinit                                            |PRE | 设定用户级GDB调试环境的设定，例如，关闭确认的开关、自定义命令等       |
gdb --command=program.cmd.gdb -p $(pidof program)                             |PRE | 执行OS Shell命令，例如，重新make程序；获取进程号等        |
define custom_cmd                                        |POST | 初始化设定或在GDB交互式Shell中定义自定义常用命令        |

> 可以在`.gdbinit`文件中设定用户级全局设定
> 可以在`program.cmd.gdb`设定程序级设定，例如，参数、LD_LIBRARY_PATH环境变量等重复执行动作

## Coredump

命令                                       |类别 |说明                 | 
--------------------------------------------|-------------------------------------------|----------------------|
file /path/to/program /path/to/coredump*   | PRE | 在OS Shell下可以查看core文件与那个程序相关 |
gdb /path/to/program /path/to/coredump     | PRE | 查看某程序的coredump信息 |
core-file /path/to/program                 | POST |加载或切换core文件 |
generate-core-file                         | POST |对目标产生core文件 |


## 线程&堆栈

命令                                       |类别 |说明                 | 
--------------------------------------------|-------------------------------------------|----------------------|
pstack pid  | PRE | 可以在不启动GDB的情况下查看看程序的堆栈 |
thread apply all bt     | POST | 查程序所有线程的堆栈信息 |
i threads   | POST | 查看调试程序的相关进程 |
thread num   | POST | 切换线程 |
core-file /path/to/program                 | POST |加载或切换core文件 |
generate-core-file                         | POST |对目标产生core文件 |




