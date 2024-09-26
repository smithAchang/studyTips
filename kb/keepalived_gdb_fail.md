`keepalived`程序竟然不能正确被`gdb`正常调试


# 现象
通过`gdb att $keepalived_pid`发起对当前运行`keepalived`的调试，在放行`keepalived`继续执行后，想通过`Ctrl+C`中断执行，想观察下内部状态，
但是，在终端输入`Ctrl+C`后，`keepalived`被调试进程退出，`gdb`无法进行正常的调试交互！

# 结论先行
+ `gdb`与`inferior process`之间通过`ptrace`系统调用，以及对`signal`信号的拦截、传递进行调试过程
+ 由终端触发的`signal`会首先被`Kernel`路由到`inferior process`，然后再被`gdb`截获
+ `Linux Kernel`针对被调试进程信号存在特别的路由机制，会导致某些应用使用了特别信号捕获机制，例如，`sigwait or signalfd`，并不能被`gdb`正常调试



# 缘由
以前就[了解到](https://bugzilla.kernel.org/show_bug.cgi?id=9039#c8 )如果被调试程序使用到类似`sigwait`特殊的信号捕捉机制，将会导致`gdb`调试遇到麻烦，特别是无法正常使用`Ctrl+C`进行中断暂停被调试程序。


最近遇到`keepalived`一些问题， 就想通过调试手段，看看它的运行时逻辑，就遭遇了与描述现象一样的调试失败。先搜索了下`keepalived`源码，并无`sigwait`的使用过程，因为以前看那篇介绍比较粗，不知道`signalfd`使用方式，同样会遭遇类似问题。
而且，当时并没有怀疑，作为比较出名的开源软件，竟然默认不支持调试。

为了分析这个问题，在挖掘过程中，就用不同的`keepalived`版本、不同的`linux`部署环境、不同的`gdb`版本，来验证这个问题，结果发现现象出奇的一致！

最终，懒得继续研究下去，就向`keepalived`社区提出了[Keeppalived不能被调试的issue](https://github.com/acassen/keepalived/issues/2430)。

## 解决方案

> `keepalived`社区称之为`Linux Kernel signal bug`，但`Linux`社区保持这样的信号处理特性，有其原因。最终，`keepalived`社区用新增启动参数项`--ignore-sigint`来解决这个问题。


# 参考
+ [--ignore-sigint option](https://github.com/acassen/keepalived/commit/7e4c75fb68c9e1c70a31ea332e8dc7ac2baa5c3d)
+ [Linux信号路由特殊机制导致GDB调试失败](https://bugzilla.kernel.org/show_bug.cgi?id=9039#c8 )
+ [E文调试的信号传递过程](https://undo.io/resources/gdb-watchpoint/how-work-signals-gdb)
+ [GDB 信号处理相关命令](https://ftp.gnu.org/old-gnu/Manuals/gdb/html_node/gdb_38.html)

