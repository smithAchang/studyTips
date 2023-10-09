# 源起
以前就知道套接字有可能出现部分发送成功的可能，直到近段时间一个典型的使用场景触发了明确的此问题，才予以重视，比较深入地考虑解决这个问题的方案！

# 分析
因为TCP的流式特征，如果出现部分发送成功，则剩余的部分数据也应该被发送出去，以利于对端正确处理消息。

如果抛弃了剩余部分，继续发送新的发送请求，则对端有可能无法正确处理，特别是解码带有边界特征的命令消息时，不能随便出现流中的间隙！

# 正确解决之道
+ 缓存剩余部分数据，在剩余部分未发送出去的情况下，明确拒绝下次发送请求，或设计一定的缓冲新发送请求的能力
+ 侦测TCP套接字的可写时机，实时发送剩余部分数据

> 注意，在TCP套接字重新可写，能够被write后，发送剩余部分，依然可能出现剩余

## 有局限性的对端缓解之道
在对于TCP流式传输的数据，特别在含有消息边界的业务中，TCP对端尝试多次后，依然无法解码数据，则可以采用抛弃已缓存读取数据的策略。

因为TCP命令消息输入不可能很频繁，在间歇报文信令间，抛弃脏数据，很有可能在后期处理就可以恢复正常 :)

# 多说几句
简单的网络套接字编程，在部分发送成功的处理上并不简单，比较耗代码！

# 参考
+ [man 2 write](https://man7.org/linux/man-pages/man2/write.2.html)
> Such partial writes can occur for various reasons; for example, because there was insufficient space on the disk device
> to write all of the requested bytes, or because a blocked write() to a socket, pipe, or similar was interrupted by a signal handler
> after it had transferred some, but before it had transferred all of the requested bytes.  In the event of a partial write, the
> caller can make another write() call to transfer the remaining bytes. The subsequent call will either transfer further bytes or
> may result in an error (e.g., if the disk is now full)

## 重点几个错误码可能部分发送成功的场景
+ EAGAIN
+ EWOULDBLOCK
+ EINTR


---
# ACE框架下的解决之道
## 仅ACE_Message_Block组成的链表 
利用ACE_Message_Block自身的链表能力，组成弹性的发送请求队列，然后从队头逐个处理发送请求，遇到部分发送成功，则修改当前发送`ACE_Message_Block`读指针rd_ptr(hasSendNum)，并不从队列中删除。

## ACE_Message_Queue
使用`ACE_Message_Queue`，依然可以形成弹性的发送请求队列，而且具备同步策略定制能力。需要在发送时，使用`peek_dequeue_head`接口，获取队头引用，但并不从链表中删除，发送完整成功后，再调用`dequeue_head`删除队头。

# 结束语
> 解决思路的关键是保留宏观上的TCP流式数据特点，逐次发送，直至成功，或最终的失败，释放连接，重新建链！
