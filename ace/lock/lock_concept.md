近来遇到一个问题，需要在已一个C/C++混编的DB模块上可以让其支持可变无锁访问或有锁访问。

原来该模块已通过条件编译，可以通过重编达到有锁或无锁的方式， 但是，不够自由，能否在同一个版本中，通过参数定制这种有锁或无锁方式？

我们知道，在ACE的框架中提供了ACE_MT_SYNCH和ACE_NULL_SYNCH两个模板相关的策略类，在编译或创建的时间来达到定制无锁和有锁的使用方式。当然，在我们这个DB模块中也可以使用这种套路，但囿于固执的编程规范，自身要避免C++模板实现，虽然可以使用已有的模板类，所以，模板解决之路并不通畅。

在非模板化的道路上，看来只能通过创建过程的参数，而非类模板参数来实现这种锁使用的可变性。对于这样的限定场景，如果使用设计模式的套路的话，无疑是**工厂方法**的用武之地。但，当时想改造量尽量的小，就放弃了，选择了将就的办法，而且此将就的实施，更是引出了自己对于变量与不变量的深入思考。


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

首先，面对的是写死了类型的成员不变量，需要为有锁和无锁找到共同的基座类型，而且为了适应有锁和无锁的变化，要用指针变量，增加弹性。
再次验证在C语言编程世界的名言，可以用指针增强弹性！

再者，面对的是使用锁的C++ RAII的惯用法，如果`if(){ useLock } else {withoutLock}`方式进行改造代码，是非常笨拙和丑陋的，但可以通过第一步锁策略指针变量化，继续使用C++ RAII方式，整体上保证代码变动比较小。

```
 CommonLockBaseType commonLockPtr;
 if(useLock)
 {
    commonLockPtr = new LockType();
 }
 else
 {
    commonLockPtr = new WithoutLockType();
 }

 ...
 //raii
 ACE_Read_Guard<CommonLockBaseType> readGuard(*commonLockPtr);

 ...
 //raii
 ACE_Write_Guard<CommonLockBaseType> writeGuard(*commonLockPtr);

```

具体在ACE应用框架的下，有锁和无锁改造方案如下:

```

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
 //raii
 ACE_Read_Guard<CommonLockBaseType> readGuard(*commonLockPtr);

 ...
 //raii
 ACE_Write_Guard<CommonLockBaseType> writeGuard(*commonLockPtr);

```

# 结束语
+ 为不同的东西找到共同的基座，进行变量化，就可以自适应两种不同的情况
+ 模板相对于虚函数弹性空间更大，而且可以避免虚函数损耗

## 为ACE说两句
+ ACE的应用框架利用`ACE_Lock`的纯虚函数锁策略类基座和ACE_Lock_Adapter<ACE_LOCKING_MECHANISM>适配器类实现在创建期有锁使用和无锁使用的弹性
+ 如果直接使用ACE_MT_SYNCH和ACE_NULL_SYNCH的类似的模板技术，正如ACE在`ACE_Lock`的介绍中，可以避免虚函数所带来性能损耗和某些的不变

## 锁策略的生效的时期明显区别
+ 编译期能够确定
+ 运行创建期能够确定

> 编译期能够确定的材料，确定了编码的砖石