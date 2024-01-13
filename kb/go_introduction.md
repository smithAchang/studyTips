
# 引子
以前就了解`Go`语言，因为其天生为并发、并行而生，且在语言层面就进行了内秉设计。

总想对比于我们自研的分布式并发、并行平台，以利于得到一些新认识 ：）

# Go官网资料
在`Go`的官网资料提供了很好的资料和知识库

+ 初学者园地[Tour](https://go.dev/tour/moretypes/15)
+ 必读必学的建议编程规范[effective_go](https://go.dev/doc/effective_go)
+ 博客，例如，介绍其语言设计思想的[declaration-syntax](https://go.dev/blog/declaration-syntax)。
+ 博客，例如，介绍依赖通信进行共享[Share Memory By Communicating](https://go.dev/blog/codelab-share)
## 并发并行的CAP(Communicating Sequential Processes)基础理论
+ [Brief](http://www.usingcsp.com/)
+ [Tony Hoare's pdf](http://www.usingcsp.com/cspbook.pdf)

> 总的来说，**CAP**建议通过管道进行共享信息，以利用并发、并行协作，并建议提供成为语言基础构件，以便于替换共享内存同步原语，例如，**mutex**、**semphore**等
## 并发与并行的关系
这两个概念紧密相关，但是，又有区分。

我们可以设想在多进程使用场景，并发与并行的概念就比较好区分。

在一个`CPU`的情况下，多进程仅能并发、但并不能并行。

如果非要说到并行，只是因为`CPU`运算速度够快，并行概念仅是粗粒度，或者说比较高级层面呈现的假象！

对于底层`CPU`层面是无并行的！

但，在多核`CPU`的情况下，多进程既可以做到并发、又可以做到一定程度的并行。

> 如果熟悉操作系统概念的同学，用进程调度的术语来讲，进程并发就是意味着多个进程都进入了`就绪`态。

## 鼓励CSP编程的口号
借用`Go`语言官网上总结的口号
> Go encourages a different approach in which shared values are passed around on channels and, in fact, never actively shared by separate 
> threads of execution. Only one goroutine has access to the value at any given time. Data races cannot occur, by design. To encourage this 
> way of thinking we have reduced it to a slogan:
> 
> Do not communicate by sharing memory; instead, share memory by communicating.

---

~~不成熟的翻译~~ 

> Go 鼓励使用一种不同方式，通过通道来传递共享值，以永久避免不同的线程进行共享竞争。
>  在任何时间，仅有一个协程可以访问(读、写)此值，所以，数据之间的竞争，从设计上不能发生。
> 为了简化此种思考方式的转变，我们概括它成为一句口号：
> > 不要用共享内存进行协作，但是，共享内存通过生产者和消费者的通信进行


## `Go`的优势
`Go`语言根据**CSP**并发、并行理论，在语言层面提供了**goroutine**和**channel**作为基础构件，使得在其它语言必须直面线程进行编程模式，可以有另外一条轻便的路径选择！

因为，多线程编程是困难的，易错的！

我觉得选择`Go`语言来解决项目中并发、并行问题，可以有以下优势：
+ 降低了解决并发、并行问题的难度，提供了基础构件作为代码粒度划分的利器

+ + **goroutine**在多核间的分布调度问题，无需开发者主动介入。由`Go`运行时高效保证，避免忙的忙死、闲的闲死，并可弹性伸缩
+ + 有效避免了多线程编程的线程管理、线程间死锁、线程忙闲不均等问复杂问题

+ 提供了现代编程语言具有的特性，例如，模块、包机制、闭包、函数编程、接口、泛型、类型自动推导等，方便高效编码
+ 跨平台能力
## `Go`语言`goroutine`编程建议

对于`CPU`执行的任务，我们知道一般分为两种，一种为`CPU密集型任务`，另一种是`IO密集型任务`。

对于`IO密集型任务`，如果采用异步策略后，实际上也可以看做近似的`CPU密集型任务`，仅有`Reactor`，或`Proactor`组件存在`IO密集型任务`的工作特点。

**`goroutine`编程建议：**
+ `goroutine`仅做`CPU`密集型任务，保证任务高速、高效
+ `goroutine`仅访问自身、排它资源；需要共享信息，通过**channel**通信进行协作

> 虽然`Go`语言同时提供了同步包，以用于减少通过**channel**机制的overhead，
> 但，我觉得有点鸡肋！
>  **原因如下：**
> + 其破坏了语言并发、并行模型，
> + 存在程序员过早优化，或过度优化的嫌疑，而非拥有充足证据后，才进行的懒惰式优化。

> > 猜测来看，同步包会带来`Go`运行时为精细化**goroutine**分布调度，而引入的新overhead
# 回到源头
> 对比于自研的平台系统，自研平台预先确定了**JOB**的并行的分布模型，并不具备弹性管理能力，但核心上并发、并行思想接近于**CSP**，利用**JOB**间消息通信进行。

>对于领域内非通用应用，**自研平台够用，且实用！**


