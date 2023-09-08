#!/bin/bash
#
# This simple script is used to build ACE
# Date: 2023-09-06
#

curDir=$(pwd)

if [ $# -gt 0 ]; then
   # support relative path
   pushd $1
   compileDir=$(pwd)
else
   compileDir=$curDir
fi

if [ ! -d $compileDir/ace ]; then
   echo "You should exec this script in ACE_ROOT dir or set the ACE_ROOT dir as input para !"
   exit 1
fi

if [ ! -d $compileDir/MPC ]; then
 echo "You must have MPC tool. Please git clone https://github.com/DOCGroup/MPC.git repo"
 exit 1
fi

export ACE_ROOT=$compileDir
echo  "Using ACE_ROOT: $ACE_ROOT"

if [ ! -d $curDir/ace ]; then
 # change work dir
 pushd $ACE_ROOT
fi

pushd bin
$ACE_ROOT/bin/mwc.pl -type gnuace $ACE_ROOT/ACE.mwc
popd

echo "
#ifndef ACE_HAS_IPV6
   #define ACE_HAS_IPV6
#endif

#include <ace/config-linux.h>
" > $ACE_ROOT/ace/config.h

echo 'include $(ACE_ROOT)/include/makeinclude/platform_linux.GNU' > $ACE_ROOT/include/makeinclude/platform_macros.GNU

export LD_LIBRARY_PATH=$ACE_ROOT/lib:$LD_LIBRARY_PATH

# parallel compile
make -j8

# if dir stack is empty, oper is noop
popd
