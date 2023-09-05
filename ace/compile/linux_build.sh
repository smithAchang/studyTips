#!/bin/bash
#
# This simple script is used to gather compile time metrics.  You can use
# it with make like this:
#
#  $ make CXX=g++_metric.sh
#

curDir=$(pwd)

if [ $# -gt 0 ]; then
   # get the full path
   pushd $1
   compileDir=$(pwd)
   popd
else
   compileDir=$curDir
fi

if [ ! -d $compileDir/ace ]; then
   echo "You should exec this script in ACE_ROOT dir or set the dir as input para !"
   exit 1
fi

if [ ! -d $compileDir/MPC ]; then
 echo "You must have MPC tool. Please git clone https://github.com/DOCGroup/MPC.git MPC"
 exit 1
fi

export ACE_ROOT=$compileDir
echo "ACE_ROOT: $ACE_ROOT"

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

export ACE_ROOT=$compileDir
export LD_LIBRARY_PATH=$ACE_ROOT/lib:$LD_LIBRARY_PATH

# parallel compile
make -j8

popd
