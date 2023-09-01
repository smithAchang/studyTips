
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


## fuzz found trailing whitespace check

cd $ACE_ROOT
bin/fuzz.pl

> 可以避免简单多余空格的修改PR，被自动流程拒绝

## vc proj
$ACE_ROOT/bin/mwc.pl -type vs2019 -recurse -hierarchy -relative ACE_ROOT=/home/gui/ACE_TAO/ACE -relative TAO_ROOT=/home/gui/ACE_TAO/TAO -workers 20 -name_modifier "*_vs2019"


## window
/d/gitrepo/ACE_TAO/ACE/bin/mwc.pl -type vs2019 -recurse -hierarchy -relative ACE_ROOT=/d/gitrepo/ACE_TAO/ACE -relative TAO_ROOT=/d/gitrepo/ACE_TAO/TAO -workers 20 -name_modifier "*_vs2019"

