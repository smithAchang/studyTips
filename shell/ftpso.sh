#!/bin/bash
#

funciton ftpso()
{
  if [ -z "$1" ]; then
	srcDir=$(pwd)
  else
    srcDir=$1
  fi
  
  if [ -d $srcDir/childDir ]; then
      srcFileDir=$srcDir/childDir
  fi
  
  # find the target file
  srcFile=$(ls $srcFileDir/someFile)
  dstDir="$(date +%Y%m%d_%H_%M_%S)"
  ftp -ivn 172.16.254.218 << EOF
	  user ftpUser plainPassword
	  bin
	  cd /someDstDir
	  mkdir $dstDir
	  cd $dstDir
	  put $srcFile ${srcFile##/*/}
	  bye
EOF

}