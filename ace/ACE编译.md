
[Building and Installing ACE](https://www.dre.vanderbilt.edu/~schmidt/DOC_ROOT/ACE/ACE-INSTALL.html)

git clone git@github.com:DOCGroup/ACE_TAO.git

export ACE_ROOT=/path/to/ACE_wrappers
export LD_LIBRARY_PATH=$ACE_ROOT/lib:$LD_LIBRARY_PATH



echo "ace/config-linux.h" > $ACE_ROOT/ace/config.h
echo "include $(ACE_ROOT)/include/makeinclude/platform_linux.GNU" > $ACE_ROOT/include/makeinclude/platform_macros.GNU

cd $ACE_ROOT

if [ ! -d MPC ]; then
 git clone https://github.com/DOCGroup/MPC.git MPC
fi

# 增加新文件，修改工程配置
if [ add some new file ]; then
  modify the $ACE_ROOT/ace/ace.mpc
fi

$ACE_ROOT/bin/mwc.pl -type gnuace ACE.mwc

cd $ACE_ROOT

make

