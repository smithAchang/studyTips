
# 结论先行

+ `Linux TCP`套接字默认缓冲区策略已很大程度上可以满足要求
+ `TCP`套接字并不适合主动设置套接字缓冲区，除非证明有必要!

> 因为`TCP`为可靠传输，并具有流量控制、拥塞控制等功能，所以，主动地单项提高套接字收发缓冲区，可能效果并不会太好

# 证据

+ `Redis`服务的配置文件不存在对于`TCP`套接字缓冲区的设置
> `Redis`作为知名服务程序，并严重依赖`TCP`套接字进行工作，不主动设置`TCP`套接字缓冲区有其原因

+ `sysctl -a -r '[wr]mem'`命令显示套接字内核参数，仅具有`IPv4`的遗留`TCP`控制项，但更外现代的`IPv6`此类控制项并没有对应提供
> 反推此类设置可能并不合适

+  测试`Demo`程序显示默认情况下`TCP`套接字缓冲区并不太小

+ 根据官网介绍`Linux`协议栈会根据主机情况进行智能确定

> 相关测试均在`Linux`环境中验证

## setsockopt

`setsockopt`接口对于`TCP`套接字的缓冲区能够起到作用，并受系统内核参数`net.core.[wr]mem_default`和`net.core.[wr]mem_max`的影响。

**但如上所述，它并不建议使用！**


# 参考
+ [IPv4内核参数 tcp_rmem等](https://www.kernel.org/doc/html/latest/networking/ip-sysctl.html)
+ [net相关内核参数](https://www.kernel.org/doc/html/latest/admin-guide/sysctl/net.html)

