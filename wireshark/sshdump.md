# 引言
以前在不了解`wireshark`可以远程抓包的时间，经常通过`tcpdump`在远程`linux`主机将抓包文件保存下来后，然后拖拽入`windows`中再打开分析查看。
过程比较繁琐，也不够实时。
但实际上，`wireshark`可以通过自身组件，或其它工具的辅助支持支持远程抓包。

# 两种简单的方法
+ 通过`wireshark sshdump`组件支持`ssh`远程登录`linux`主机后进行实时在线抓包
> + 此组件默认不安装，需要在安装`Tools`组件中进行特殊勾选
> + 默认过滤条件会抛弃掉非相关报文 ，比较智能

+ 借助`MobaXterm`的本地客户端工具，通过远程命令和管道操作进行远程抓包
```shell
cd /drives/c/"Program Files"/Wireshark
ssh root@192.168.8.146 'tcpdump -i enp0s3 ! host 192.168.9.37 -s 0 -l -w -' | ./wireshark -k -i -
```

> 手工步骤过多，但，已算是较为简单的方法


+ ~~其它借助`ncat`更复杂的远程操作方法不在赘述~~


# 如何配置sshdump组件
+ 已安装wireshark
> 通过`Wireshark*.exe`程序重新安装wireshark，在`Tools`组件选择中特别勾选

+ 未安装wireshark
> 下载新的`Wireshark*.exe`程序安装wireshark，在`Tools`组件选择中特别勾选

见图


# 如何配置SSH Remote Capture

# 成功安装sshdump后的界面
界面上会出现`SSH remote capture`的选择项，见图：

# 配置ssh登录参数
点击`SSH remote capture`标签的前的图标，可以对远程抓包的参数进行配置

## Server端口配置
包括**IP和Port**，比较简单不赘述

## 认证配置
配置用户名和密码，建议通过非对称加密的公钥和私钥配置，进行免密登录

### ssh免密登录配置
通过较为自动化`ssh-copy-id`命令进行操作，减少手工出错概率，见链接[最简SSH免密配置方法](https://blog.csdn.net/jkler_doyourself/article/details/135879408)

## Capture抓包配置
主要配置抓包网口，可以使用**any**关键词代表所有网卡，包括`lo`环回接口


