
# 导言 
很多人在`Linux`中用信号让程序退出，总是恨不得直接用`kill -9 pid`，而实际来看，没有那个必要，也并不优雅，可以用`TERM`信号！
# 信号处理的特点
+ 硬件中断，例如，除零、内存缺页、越界等指令级不可继续异常，在下条指令执行前必须被中断的处理
+ 软中断，OS调度级处理，在内核态到用户态切换的时机


## man文档的误导

>    Execution of signal handlers
>       Whenever there is a `**transition**` from **kernel-mode to user-mode**
>       execution (e.g., on return from a system call or scheduling of a
>       thread onto the CPU), the kernel checks whether there is a
>       pending unblocked signal for which the process has established a
>       signal handler

---

> 每当存在一个内核态到用户态执行的转变，例如，系统调用、重新调度得以在CPU上执行，内核会检查是否存在未处理的异常

注意， 非逐字翻译，但文本描述中转换时机举例涵盖场景**不够全面**，存在某些误导！

对于那些阻塞的线程，以及在用户态无限循环的线程，是否信号处理就无法进行了么？

经过验证，`Linux`对于阻塞操作进行了特殊对待，见章节`Interruption of system calls and library functions by signal handlers`。

对于用户空间的无限循环程序，在微观的操作系统视角来看，并不存在一直占用CPU，所以，总会从用户态转换到内核态的就绪，然后再从就绪态获得下一次CPU执行机会，就满足了文档介绍的转换时机。
如果用户态无限循环程序，配合以全局易失的退出标记量，则程序退出问题，更是可以比较优雅地解决！


# 优雅推出方式模板
```c
   // 以C语言为例
   volatile char g_bExit;
   
   // 信号处理函数，在默认信号处理不能满足要求时使用
   void signal_handler(int signum)
   {
		if (signum == SIGTERM && g_bExit == 0) {
			g_bExit = 1;
			/*向伙伴发送停止信号*/
			for_each_managed_thread()
			{
				pthread_kill(eachThreadId, SIGTERM);
			}
			
			/*主线程用非信号方式退出*/
			wakeupMainThread();
		}
   }
   
   //main thread
   while(unlikely(g_bExit))
   {
     ...
   }
   // 等待伙伴退出
   pthread_join(somePartnerThreadId);
```

# 验证信号处理脚本
用`shell`脚本程序，也可以很好地完成对信号处理的验证，而不用写`C`语言程序，何乐而不为？

```bash
echo "Self pid: $$, testing signal process character ..."
# 阻塞的伙伴
/bin/bash -c "sleep 1000" &
# 用户态无限循环的伙伴
/bin/bash -c "x=0;while true; do let x=x+1; done" &

# 打印后台进程列表
jobs -l

# 两段打印分开
sleep 1
echo "Send signal TERM to background jobs ..."
kill %1 %2

echo "Will sleep to see after having send TERM signal ..."
k=0
while true ; do
  sleep 2
  echo "Print jobs loop count: $k; The expected job list should be empty! ..."
  
  jobs -l

  let k=k+1
  if [ $k -eq 10 ]; then
     break
  fi
done

echo "Test is over"
```

# 欢迎指正
验证过程，如果不足之处，欢迎指正，谢谢 :)
# 参考
+ [man 7 signal](https://www.man7.org/linux/man-pages/man7/signal.7.html)

