# fuzz found trailing whitespace check
export ACE_ROOT=/path/to/aceRoot

cd $ACE_ROOT
bin/fuzz.pl

> 避免PR被自动流程拒绝


~/cppcheck/cppcheck-2.10/cppcheck --enable=all --check-config /home/gui/ACE_TAO/ACE/ace/RAW_Socket.cpp

~/cppcheck/cppcheck-2.10/cppcheck --enable=all --check-config --includes-file=/home/gui/ACE_TAO/ACE /home/gui/ACE_TAO/ACE/ace/RAW_Socket.cpp

 ~/cppcheck/cppcheck-2.10/cppcheck --enable=all --check-config --includes-file=/home/gui/ACE_TAO/ACE   --includes-file=/usr/include  /home/gui/ACE_TAO/ACE/ace/ICMP_Socket.cpp

# 借用这个同样存在问题
  ~/cppcheck/cppcheck-2.10/cppcheck --enable=all --check-config --includes-file=/home/gui/ACE_TAO/ACE   --includes-file=/usr/include  /home/gui/ACE_TAO/ACE/ace/SOCK_Dgram.cpp