在`Unix\Linux`体系中，一个进程通常自带有标准输入、标准输出、标准错误输出等三个文件描述符。

如果从对称的观点来看，它确实有点奇怪，但它背后隐藏了什么样的知识和道理呢？

以前谈到现在计算机背后的理论结构是图灵机，而图灵机模型仅需要左右两边的纸带，其实可以直接对应输入和输出。

我们知道，一个程序在运行期不携带日志系统是无法想象的，但是，通常程序又有自己的输出数据，而日志信息是明显有区别于这些输出数据。

如果能够一边观察运行期行为，例如，输出日志进行分析，一边输出期望数据也是很方便的。这个在`Unix\Linux`体系中典型程序`ssh`中，明显区分了标准错误输出和标准输出的不同。

另外，特别在`Shell`编程中，经常通过管道或者IO重定向拼接命令，如果命令运行过程中，出现某些提示或错误信息，可以能够在控制台或终端及时展现将是非常方便的。

通过阅读一些网络E文，感觉`Unix\Linux`体系特别地具有标准输出错误文件描述符，可能来自传统实践，和某种编程上的便利。

我们通常可以看到，在标准输出也是日志信息的情况下，在日志收集的时间，采用`1>log.txt 2>&1 or &>log.txt`的命令语法，将标准输出和错误输出都指向相同的目的。

---

在一系列图灵机中，有一类的图灵机具有普适的特点，我们称其为**普适图灵机**。
这类图灵机的核心就是可以将其它图灵机的控制转移函数作为自己的输入，然后去模拟那个图灵机的行为。
如果从学习的观点来看，这其实就相当于学习过程，增强了个人能力。
如果从递归的观点来看，这相当于可以展现一种无穷套娃的能力。
在可计算性理论模型上，图灵机理论与递归理论被认为是等价的计算模型。

可计算模型界定的问题是什么是可计算，而什么是不可计算。可计算模型中计算的涵义，与我们通常认为的计算有细微的区分，但都包括我们通常的认为的计算。
可认为可计算模型中的计算，是计算的尽头


---
某些特殊命令，例如，`cat`和`md5sum`只能从标准输入传入字符串参数，则需要`here document`和`here string`技术。
```bash
cat << _EOF_
<html>
...
</html>
_EOF_

cat <<< 'Hello world'
md5sum <<< 'aaa'

```

---
如果命令对于标准输入和标准输出有对应处理，则可以非常方便地利用管道进行组合命令。特别地，`tar`命令通过'-'符号来约定指明。
可以看到'-'符号并不专门指代标准输入或标准输出，而是命令的一种处理约定。
> If you use ‘-’ as an archive-name, tar reads the archive from standard input (when listing or extracting files),
> or writes it to standard output (when creating an archive). 
> If you use ‘-’ as an archive-name when modifying an archive, tar reads the original archive from its standard input and writes the entire new archive to its standard output.


# 参考
+ [Tar标准输入和输出的处理](https://www.gnu.org/software/tar/manual/tar.html#file)

