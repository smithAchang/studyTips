#!/bin/bash
#

echo "Self pid: $$, testing signal process character ..."
# 阻塞的伙伴
/bin/bash -c "sleep 1000" &
# 用户态无限循环的伙伴
/bin/bash -c "x=0;while true; do let x=x+1; done" &

# 打印后台进程列表
jobs -l

# 两段打印分开
sleep 1
echo "Send signal TERM to background jobs ..."
kill %1 %2

echo "Will sleep to see after having send TERM signal ..."
k=0
while true ; do
  sleep 2
  echo "Print jobs loop count: $k; The expected job list should be empty! ..."
  
  jobs -l

  let k=k+1
  if [ $k -eq 10 ]; then
     break
  fi
done

echo "Test is over"
