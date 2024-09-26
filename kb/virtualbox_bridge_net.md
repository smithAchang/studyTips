# VirtualBox桥接网络之怪事

+ 在主机上用`ip -d link`命令竟然发现不了任何`VirtualBox`创建的网桥或网卡

# 由已知推测未知
+ [看官网介绍，桥接网络使用filter技术](https://docs.oracle.com/en/virtualization/virtualbox/6.0/user/network_bridged.html)
+ tcpdump工具

## tcpdump发现
根据以前的经验，了解到`tcpdump`通过设置网卡的混杂模式实现网卡抓包，它的混杂模式是如何实现的呢？

通过启动和停止`tcpdump`，分别观察`ip -d link`命令的输出发现，与`ip link set dev device promisc on`设置方式不同。
`tcpdump`通过其它路径，也可以设置网卡的混杂模式，主要体现在`ip -d link`输出行中的`promiscuity *`信息上。


```c
Updating the device flags via ifconfig DEV promisc or ip link set dev DEV promisc on is not the only way to set a device in promiscuous mode: 

another way is via the packet(7) setsockopt() interface:

setsockopt(sock, SOL_SOCKET, PACKET_ADD_MEMBERSHIP, {.mr_type = PACKET_MR_PROMISC})`
That is what tcpdump (and other network capturing and filtering tools) are using.
```

## 反复启停VirtualBox管理虚机
发现在没有任何`VirtualBox`虚机启动的情况下，`ip -d link`命令显示`promiscuity 0`，如果存在虚机启动，则`promiscuity`显示不为零。

# 推论
结合前面已知的信息，VirtualBox桥接网络典型地使用了`Net Filter Driver`技术，并没有创建新的网桥或网卡。

所以，`VirtualBox`的桥接网络，与`KVM`虚机桥接网络使用`macvtap`网卡技术不同，所以，某些表现上会存在差异，例如，`keepalived useVMAC mode`



