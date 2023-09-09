
# 下载源码
git clone git@github.com:smithAchang/ACE_TAO.git

# tests目录增加测试文件

## 先搜索是否存在已有测试文件
> 避免重复增加

## 新增Test文件后的特殊要求
可参照$ACE_ROOT/../tests目录按照已有格式增加测试源文件
特别地，ACE_START_TEST (ACE_TEXT ("$src_file_name"));
ACE_START_TEST起始的宏的参数，注意保持与源文件相同名字，linux下测试时要产生输出的日志名称能够对的上才能通过



## 修改mpc文件以利于生成测试工程
project(RAW Socket Test) : acetest {
  avoids += ace_for_tao
  exename = RAW_Socket_Test
  Source_Files {
    RAW_Socket_Test.cpp
  }
}

> 按照测试文件的目录顺序排序，可以在`run_test.lst`发现合适的位置

# windows下测试
## linux下生成 vcproj
$ACE_ROOT/bin/mwc.pl -type vs2019 -recurse -hierarchy -relative ACE_ROOT=$ACE_ROOT -relative TAO_ROOT=$ACE_ROOT/../TAO -workers 20 -name_modifier "*_vs2019"


## window利用git bash生成vcproj
/d/gitrepo/ACE_TAO/ACE/bin/mwc.pl -type vs2019 -recurse -hierarchy -relative ACE_ROOT=/d/gitrepo/ACE_TAO/ACE -relative TAO_ROOT=/d/gitrepo/ACE_TAO/TAO -workers 20 -name_modifier "*_vs2019"


### 暂时解决办法vcproj中修改$(ACE_ROOT)

 直接修改vcproj文件，直接修改$(ACE_ROOT)为..

## 先编译通过

# linux下测试
## 先编译通过

> + cd $ACE_ROOT/tests
> + 修改run_test.lst，按照模板增加自己的测试用例
> + export LD_LIBRARY_PATH=$ACE_ROOT/lib:$LD_LIBRARY_PATH
> + 运行./run_test.pl







