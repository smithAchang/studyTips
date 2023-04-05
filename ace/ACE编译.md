
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


if [ add some new file ]; then
  modify the $ACE_ROOT/ace/ace.mpc
fi

$ACE_ROOT/bin/mwc.pl -type gnuace ACE.mwc

cd $ACE_ROOT

make

# fuzz found trailing whitespace check

cd $ACE_ROOT
bin/fuzz.pl

> 避免PR被自动流程拒绝

# vc proj
$ACE_ROOT/bin/mwc.pl -type vs2019 -recurse -hierarchy -relative ACE_ROOT=/home/gui/ACE_TAO/ACE -relative TAO_ROOT=/home/gui/ACE_TAO/TAO -workers 20 -name_modifier "*_vs2019"


# window
/d/gitrepo/ACE_TAO/ACE/bin/mwc.pl -type vs2019 -recurse -hierarchy -relative ACE_ROOT=/d/gitrepo/ACE_TAO/ACE -relative TAO_ROOT=/d/gitrepo/ACE_TAO/TAO -workers 20 -name_modifier "*_vs2019"
