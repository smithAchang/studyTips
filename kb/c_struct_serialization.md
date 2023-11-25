
# 场景
在一些场合下，我们需要对`C`语言的结构体进行持久化，或序列化，以利于交换信息。

# 需要考虑的问题
+ 存储和传输效率
+ 字节序问题
+ 升级兼容性问题

# 

先查看最简单的，然后再分析组合和递归的形态。

# 方案



+ 直接用结构体的内存进行序列化的简易方案

 优点      |缺点             |
----------|----------------|----------------
存储效率高 | 存在字节序问题  |
-         | 存在升级兼容问题|

+ TLV
优点      |缺点             | 说明
-----------|----------------|----------------
存储效率较高   | -  |
无字节序问题   | -|  需要约定一种标准的字节序
无升级兼容问题 | -|  注意对于缺失值的处理

> 注意点：
> + 在一级解码区域内`T`值不应该出现二义性
> + `T`值的定义更接近语义，即使实际存储类型两者都是相同的原始类型



+ NameTLV
> 在**TLV**基础上的变种，可以按照基本类型，仅定义少量的`T`值，用多携带的名称字段作为定位编解码字段的手段

# 按照配置自动编解码
> 如果按照不重复的`T`值定义，可以按照结构体的偏移实现自动编解码

# 例子

```c
#define STRUCT_MAX_FILED ((uint16_t)256)

typedef struct  tagT_StructFieldDesc
{
   unsigned int type;
   unsigned int offset;
} T_StructFieldTLVDesc;

typedef struct  tagT_StructFieldDesc
{
   unsigned int num;
   T_StructFieldTLVDesc fields[STRUCT_MAX_FILED];
} T_StructFieldTLVDesc;
```