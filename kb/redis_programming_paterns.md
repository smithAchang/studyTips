

# 缘由
以前只是接触过`redis`，只有最近才比较深入研究了下，觉得有几个重要的概念可以积累出来，以利于帮助理解`redis` :)

本文仅简述重点概念，和列举相关参考文档链接，但参见文档多来自`redis`官网，足见其权威，需要深入研究的同学，可以自便。

# 特点
## 字符串二进制安全
`redis`的**keystore**是基于`key:value`的。
而`value`被对待成特别的*字符串*结构体进行处理，但`redis`中的字符串概念显著区别于`C`语言字符串，它是二进制安全的，也就是说可以存储任何二进制的串！

> 在`C`语言中，普通的字符串如果遇到字符'\0'或二进制的单字节零值，均会被认为是字符串的结尾
> 
> 但在，`redis`中是基于`memcpy | memcmp ..`等`mem*`的API接口的操作，带有明确的长度的参数，进行`redis`系统内的字符串操作，所以，可以保证字符串的二进制安全。

### 如何插入二进制数据

+ `redis-cli`和`lua script`
> 如果要存储二进制数据，则需要将要存储的二进制数据编码为**hex**或**base64**字符串，需要一个编解码的过程，建议使用**base64**字符串
```shell
set key "{\x45\x12\x32\x13}"
# 查看长度，长度并非4个字节长度，体现为编码字符串的长度
# 使用‘hex’表示，不如直接使用base64编码
strlen key
```
```c++
redis.pcall('set', 'key', base64Str)
```

+ `hiredis`客户端库
> 用`hiredis`库可以使用`%b`格式化符，特别地传入首地址指针和长度参数，就可以将二进制流存储到`redis`
```c++
uint16_t u16Value = 0x1234;
redisCommand(context, "SET %s %b", "Hello", &u16Value, sizeof(u16Value))
```
```shell
# 查看长度，长度为2个字节，体现为原始类型长度
strlen "Hello"
```

>> 用`hiredis`库可以真正支持二进制写入

## lua 脚本
+ 高效
> 因为`lua script`在`redis`服务器端执行，可以有效避免那些使用`redis-cli`需要一次往返，才能够实施的后续动作
+ 原子粒度
> `lua script`脚本提供了多步操作很好的原子性操作粒度，而且往往比事务语法更快、更简单，推荐使用

> [transactions](https://redis.io/docs/interact/transactions/)

### lua脚本调试
```shell
# 注意key和参数arg之间的空格和逗号
./redis-cli --ldb --eval /tmp/script.lua mykey somekey , arg1 arg2
```
> 快捷操作上十分类似`gdb`

> [lua-debugging](https://redis.io/docs/interact/programmability/lua-debugging/)

## 批量处理(Bulk loading)

*批量处理，可以有效减少网络IO的损耗，强烈建议使用*

+ `redis-cli` **pipelining**
```shell

echo "
SET Key0 Value0
SET Key1 Value1
...
SET KeyN ValueN
" > data.txt

cat data.txt | redis-cli --pipe

```
> [pipelining](https://redis.io/docs/manual/pipelining/)

+ `hiredis`客户端库

```c
// hiredis api
redisReply *reply;
redisAppendCommand(context,"SET foo bar");
redisAppendCommand(context,"GET foo");
redisGetReply(context,&reply); // Return of SET command
freeReplyObject(reply);
redisGetReply(context,&reply); // Return of GET command
freeReplyObject(reply);

```

> [bulk-loading](https://redis.io/docs/manual/patterns/bulk-loading/)

## 分布式锁(Distributed Locks with Redis)
对于比较简单的分布式锁使用场景，利用如下语法格式的分布式互斥锁，已经够用。但比较复杂的分布式、高可用场景，可以阅读官网文档建议的部署方式。

```shell
SET resource_name my_random_value NX PX 30000
```

>  + 需要着重指出的是my_random_value是带有唯一性的值，最好满足随机性，如果非自身，则用lua脚本判断不能**del**
>  + NX指定覆盖条件
>  + PX设定超时毫秒数

> [Distributed Locks with Redis](https://redis.io/docs/manual/patterns/distributed-locks/)

## 索引用法(Secondary indexing)
`redis`具有的丰富的数据结构，例如，`set、sorted set or list`。

使用他们可以建立一些有用的索引集合，以利于快速查阅相关**keys**值，避免`SCAN`命令进行全库扫描！

> [Secondary indexing](https://redis.io/docs/manual/patterns/indexes/)

# 参考
[Redis programming patterns](https://redis.io/docs/manual/patterns/)
