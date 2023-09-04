
# 源代码
git clone git@github.com:DOCGroup/ACE_TAO.git

# 编译
[Building and Installing ACE](https://www.dre.vanderbilt.edu/~schmidt/DOC_ROOT/ACE/ACE-INSTALL.html)


## 自动化编译脚本
[Build Script](https://github.com/smithAchang/studyTips/tree/master/ace/compile/linux_build.sh)



# 如何作自测

Please follow these directions when adding a new test to
$ACE_ROOT/tests:

1.  Use appropriate ACE_START_TEST and ACE_END_TEST macros in main ()

2.  Add new project entry to the tests.mpc file.

3.  Add test to run_test.lst.



cd $ACE_ROOT/tests
./run_test.pl

