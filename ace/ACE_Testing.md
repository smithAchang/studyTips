
#
git clone git@github.com:smithAchang/ACE_TAO.git

# tests目录增加测试文件


先搜索是否存在已有测试文件

$ACE_ROOT/../tests目录按照已有格式增加测试源文件




# 修改mpc文件以利于生成测试工程
project(RAW Socket Test) : acetest {
  avoids += ace_for_tao
  exename = RAW_Socket_Test
  Source_Files {
    RAW_Socket_Test.cpp
  }
}


# windows下测试
## linux下生成 vc proj
$ACE_ROOT/bin/mwc.pl -type vs2019 -recurse -hierarchy -relative ACE_ROOT=$ACE_ROOT -relative TAO_ROOT=$ACE_ROOT/../TAO -workers 20 -name_modifier "*_vs2019"


## window利用git bash生成vc proj
/d/gitrepo/ACE_TAO/ACE/bin/mwc.pl -type vs2019 -recurse -hierarchy -relative ACE_ROOT=/d/gitrepo/ACE_TAO/ACE -relative TAO_ROOT=/d/gitrepo/ACE_TAO/TAO -workers 20 -name_modifier "*_vs2019"


## 在vcproj中修改$(ACE_ROOT)

暂时解决办法，直接修改

# linux下测试
## 先编译通过

# windows下测试
## 先编译通过