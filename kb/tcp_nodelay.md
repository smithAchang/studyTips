
# 结论先行
在`TCP`报文有时比较短，又对延迟比较敏感的控制信令场景，非常适合启用`TCP_NODELAY`套接字选项

# 现象

+ 发送者在`TCP`链路上连续发送两条请求，第一条请求立即发送出去了，而第二条要等到`TCP Ack`后才能发送出去

> `TCP`存在随路`Ack`确认和延迟`Ack`确认，对端在没有报文发送时，会出现延迟回复`Ack`确认


# 代码
```c
  int enable = 1;
  // 注意使用 第二个参数特别需要注意，需要设置为SOL_TCP
  setsockopt(fd, SOL_TCP, TCP_NODELAY, &enable, sizeof(enable));
```

> 笔者曾出现设置第二个参数`SOL_SOCKET`为，但不生效的情况

# 后记

后来回想了下，setsockopt第二个选项，体现为套接字的工作层次，与网络协议栈的层次有一定的对应关系。所以，对于`TCP_NODELAY`应该设置在传输层上！