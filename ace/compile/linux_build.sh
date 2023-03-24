#!/bin/sh
#
# This simple script is used to gather compile time metrics.  You can use
# it with make like this:
#
#  $ make CXX=g++_metric.sh
#

export ACE_ROOT=$(pwd)/..

if [ ! -d $ACE_ROOT/ace ]; then
   echo "You should exec this script in ACE_ROOT/bin dir !"
   exit 1
fi


if [ ! -d $ACE_ROOT/MPC ]; then
 echo "You must have MPC tool. Please git clone https://github.com/DOCGroup/MPC.git MPC"
 exit 1
fi

export LD_LIBRARY_PATH=$ACE_ROOT/lib:$LD_LIBRARY_PATH

echo "#ifndef ACE_HAS_IPV6
#define ACE_HAS_IPV6
#endif

#include <ace/config-linux.h>
" > $ACE_ROOT/ace/config.h


echo 'include $(ACE_ROOT)/include/makeinclude/platform_linux.GNU' > $ACE_ROOT/include/makeinclude/platform_macros.GNU

cd $ACE_ROOT

$ACE_ROOT/bin/mwc.pl -type gnuace $ACE_ROOT/ace/ace.mwc
$ACE_ROOT/bin/mwc.pl -type gnuace $ACE_ROOT/tests/tests.mwc

cd $ACE_ROOT

# parallel compile
make -j8
