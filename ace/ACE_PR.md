
# 编译通过
+ linux
> [Linux Build Script](https://github.com/smithAchang/studyTips/tree/master/ace/compile/linux_build.sh)

+ windows
> 在linux下利用脚本生成windows的工程文件$ACE_ROOT/bin/mwc.pl -type vs2019 -recurse -hierarchy -relative ACE_ROOT=$ACE_ROOT -relative TAO_ROOT=$ACE_ROOT/../TAO -workers 20 -name_modifier "*_vs2019"


# 社区规范
+ 删除回车符前的多余空格，可以用fuzz.pl检查，以避免PR被自动流程拒绝
>
> //fuzz found trailing whitespace check
> `export ACE_ROOT=/path/to/aceRoot; cd $ACE_ROOT; bin/fuzz.pl`
 
+ 删除多于两行的多余空行
+ `2 space`缩进对齐，除非特别的多参数函数调用
+ 其他符合已有代码的格式，可以进行参考
+ 编程禁止使用C语言类型强制指针转换
+ 类成员变量加`this`指针，可以用来区分变量与成员field


# 工具检查
~/cppcheck/cppcheck-2.10/cppcheck --enable=all --check-config /home/gui/ACE_TAO/ACE/ace/RAW_Socket.cpp

~/cppcheck/cppcheck-2.10/cppcheck --enable=all --check-config --includes-file=/home/gui/ACE_TAO/ACE /home/gui/ACE_TAO/ACE/ace/RAW_Socket.cpp

 ~/cppcheck/cppcheck-2.10/cppcheck --enable=all --check-config --includes-file=/home/gui/ACE_TAO/ACE   --includes-file=/usr/include  /home/gui/ACE_TAO/ACE/ace/ICMP_Socket.cpp

## 已有代码同样存在问题
  ~/cppcheck/cppcheck-2.10/cppcheck --enable=all --check-config --includes-file=/home/gui/ACE_TAO/ACE   --includes-file=/usr/include  /home/gui/ACE_TAO/ACE/ace/SOCK_Dgram.cpp


