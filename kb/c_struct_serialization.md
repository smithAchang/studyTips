
# 场景
在一些场合下，我们需要对`C`语言的结构体进行持久化，或序列化，以利于交换信息。

# 需要考虑的问题
+ 存储和传输效率
+ 字节序问题
+ 升级兼容性问题

# 方法论

先查看最简单的，然后再分析复杂的组合和递归的形态。

# 方案

+ 直接用结构体的内存进行序列化的简易方案

 优点      |缺点             |
----------|----------------|----------------
存储效率高 | 存在字节序问题  |
-         | 存在升级兼容问题|

+ TLV
优点          |缺点             | 说明
--------------|----------------|----------------
存储效率较高   | -              |
无字节序问题   | -              | 需要约定一种标准的字节序
无升级兼容问题 | -              | 注意对于缺失值的处理

> 注意点：
> + 在同级解码区域内`T`值不应该出现二义性
> + `T`值的定义应更接近语义，即使实际存储类型两者都是相同的原始类型

+ NameTLV
> 在**TLV**基础上的变种，可以按照基本类型，仅定义少量的`T`值，用多携带的名称字段作为定位编解码字段的信息

# 按照配置自动编解码

# 例子

```c
#define STRUCT_MAX_FILED ((uint16_t)256)
#define STRUCT_FIELD_NAME_MAX_LEN ((uint16_t)64)

//
typedef struct  tagT_StructFieldDesc
{
   unsigned int type;
   unsigned int length;
   unsigned int offset;
} T_StructFieldTLVDesc;

//可以定义少量的type类型，其它用结构体成员名称区分
typedef struct  tagT_StructFieldDescWithName
{
   char  szFieldName[STRUCT_FIELD_NAME_MAX_LEN + 1];
   unsigned int type;
   unsigned int length;
   unsigned int offset;
} T_StructFieldDescWithName;

typedef struct  tagT_StructDesc
{
   unsigned int num;
   T_StructFieldTLVDesc fields[STRUCT_MAX_FILED];
} T_StructDesc;

int encode(const void* ptObj, const T_StructDesc* ptDesc, unsigned char* out, unsigned int max, unsigned int* len)
{
   *len = 0;
   for(unsigned int i = 0; i < ptDesc->num && (*len) < max; ++i)
   {
      const T_StructFieldTLVDesc* ptTLVDesc = &ptDesc->fields[i];
      //Name TLV
      snprintf(out[(*len)], sizeof(ptTLVDesc->szFieldName), "%s", ptTLVDesc->szFieldName);
      *len += strlen(ptTLVDesc->szFieldName) + 1;
      out[(*len)]  =  ptTLVDesc->type;
      *len += 1;

      *(uint16_t*)&out[(*len)]  =  ptTLVDesc->length;
      *len += sizeof(uint16_t);

      memcpy(out + (*len), (unsigned char*)ptObj + ptTLVDesc->offset, ptTLVDesc->length);
      *len += ptTLVDesc->offset
   }
}

int decode(void* ptObj, const T_StructDesc* ptDesc, unsigned char* in, unsigned int max)
{
   for(unsigned int offset = 0; offset < max; ++offset)
   {
      for(unsigned int i = 0, len = 0; i < ptDesc->num && len < max; ++i)
      {
      const T_StructFieldTLVDesc* ptTLVDesc = &ptDesc->fields[i];
      //Name TLV
      if(strcmp(ptTLVDesc->szFieldName, &in[offset]) == 0)
      {
         
      }
      in[len], sizeof(ptTLVDesc->szFieldName), "%s", ptTLVDesc->szFieldName);
      *len += strlen(ptTLVDesc->szFieldName) + 1;
      out[(*len)]  =  ptTLVDesc->type;
      *len += 1;

      *(uint16_t*)&out[(*len)]  =  ptTLVDesc->length;
      *len += sizeof(uint16_t);

      memcpy(out + (*len), (unsigned char*)ptObj + ptTLVDesc->offset, ptTLVDesc->length);
      *len += ptTLVDesc->offset
      }

   }
}


```