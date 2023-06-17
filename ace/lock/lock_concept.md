# 前言

近来遇到一个问题，需要在已一个C/C++混编的DB模块上，可以让其在运行期支持可变**有锁、或无锁访问**的策略。

原来该模块已通过条件编译，可以通过重新编译达到有锁或无锁方式的切换， 但是，不够自由！那么能否在同一个版本中，通过运行期参数定制有锁或无锁的使用策略呢？

# 手中的办法

如果通过参数定制，在C++体系里常见的就是通过模板参数，或类的构造函数参数、甚至创建方法参数(可以避免某些类构造的失败，但是没有返回值的异常情况)进行实现。

我们知道，在ACE的框架中提供了ACE_MT_SYNCH和ACE_NULL_SYNCH两个模板相关的策略类，来支撑模板类定制无锁和有锁的使用方式。当然，在我们这个DB模块中也可以使用这种套路，但囿于陈旧、固执的编程规范，改造要避免C++模板实现，虽然可以使用ACE框架已有的模板类，所以，模板类参数解决之路并不通畅。

在非模板化的道路上，看来只能通过创建过程的参数进行变通。

对于限定的改造场景，如果使用设计模式的套路的话，无疑是**工厂方法**的用武之地。但，当时想改造量尽量的小和避免恐慌，就放弃了，选择了将就的办法。而此将就实现之法，却能从更细节的方式体现有锁和无锁切换的过程，更是引出了自己对于变量与不变量的深入思考！


# 已有DB代码

```
//已有DB模块示意

class Table
{
  public:
  read*(...);
  write*(...);
  
  private:
  
  ...

  ACE_RW_Thread_Mutex m_RW_Mutex;
}

Table::read*(...)
{
  //raii
  ACE_Read_Guard<ACE_RW_Thread_Mutex> readGuard(m_RW_Mutex);

  ...

}

Table::write*(...)
{
  //raii
  ACE_Write_Guard<ACE_RW_Thread_Mutex> writeGuard(m_RW_Mutex);

  ...

}

```

# 改造之路

## 类成员改造

首先，面对的是写死了读写锁类型的成员不变量。

特别需要点明，对于类成员，如果成员是类实例，则在编译期就确定下来，是无法在运行期根据参数进行变动，所以，必须适当变量化，而且要为有锁和无锁，找到共同的抽象基座类型。

为了适应有锁和无锁的变化，要用指针变量进行动态装载，这样的改造方法，更是再次验证在C语言编程世界的名言，用指针或中间层，可以增强程序弹性！


## 锁使用改造
再者，面对的是使用锁的C++ RAII的惯用法，如果`if(){ useLock } else {withoutLock}`方式进行改造代码，是非常笨拙和丑陋的，但可以通过第一步锁策略指针变量化，继续使用C++ RAII方式，整体上还可以保证代码变动比较小。

```
 //类成员变量
 CommonLockBaseType *commonLockPtr;

 //类创建期根据参数确定变量值
 if(useLock)
 {
    commonLockPtr = new LockType();
 }
 else
 {
    commonLockPtr = new WithoutLockType();
 }

```

```
Table::read*(...)
{
  //raii
  ACE_Read_Guard<CommonLockBaseType> readGuard(*commonLockPtr);

  ...

}

Table::write*(...)
{
  //raii
  ACE_Write_Guard<CommonLockBaseType> writeGuard(*commonLockPtr);

  ...

}

```

# 在ACE应用框架的下，有锁和无锁改造方案

```
 ...

 ACE_Lock_Adapter<ACE_Lock> *commonLockPtr;

 if(useLock)
 {
    commonLockPtr = new ACE_Lock_Adapter<ACE_RW_Thread_Mutex>();
 }
 else
 {
    commonLockPtr = new ACE_Lock_Adapter<ACE_Null_Mutex>();
 }

 ...

 Table::read*(...)
 {
  //raii
  ACE_Read_Guard<CommonLockBaseType> readGuard(*commonLockPtr);

  ...

 }

 Table::write*(...)
 {
  //raii
  ACE_Write_Guard<CommonLockBaseType> writeGuard(*commonLockPtr);

  ...

 }
```

# 结束语

+ 为不同的东西找到共同的基座，进行变量化，就可以自适应两种不同的情况
+ 模板相对于虚函数弹性空间更大，而且可以避免虚函数损耗

## 为ACE说两句

+ ACE的应用框架利用`ACE_Lock`的纯虚函数锁策略类基座和ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>适配器类实现在创建期有锁使用和无锁使用的弹性
  > 如果亲自动手实现，只是简单的模仿，而且正确性一下子也难以保证，不如，用成熟的框架类。而这些ACE是深思熟虑的！

+ 如果直接使用ACE_MT_SYNCH和ACE_NULL_SYNCH的类似的模板技术，正如ACE在`ACE_Lock`的介绍中，可以避免虚函数所带来性能损耗和某些的不便

## 锁策略生效时期的明显区别

+ 编译期能够确定
+ 运行创建期能够确定

> 编译期能够确定的材料，确定了编码的砖石