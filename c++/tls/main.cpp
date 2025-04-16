#include <iostream>
#include <thread>

class MyClass {
public:
    // 声明一个线程局部存储的成员变量
    thread_local static int threadLocalVar;
    thread_local static void* threadLocalPointer;

    void setThreadLocalVar(int value) {
        threadLocalVar = value;
    }

    int getThreadLocalVar() const {
        return threadLocalVar;
    }
};

// 定义并初始化线程局部存储的静态成员变量
thread_local int MyClass::threadLocalVar;
thread_local void*  MyClass::threadLocalPointer;

void threadFunction(const std::string& threadName) {
    MyClass obj;
    obj.setThreadLocalVar(std::hash<std::string>{}(threadName)); // 使用线程名的哈希值作为变量值
    std::cout << std::endl;
    std::cout << "Thread: " << threadName << ", threadLocalVar: " << obj.getThreadLocalVar() << std::endl;
    std::cout << "Thread: " << threadName << ", threadLocalPointer: " << obj.threadLocalPointer << std::endl;
}

int main() {
    std::thread t1(threadFunction, "Thread1");
    std::thread t2(threadFunction, "Thread2");

    t1.join();
    t2.join();

    return 0;
}

