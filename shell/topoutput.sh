#!/bin/sh

# pids group output firstly
top -b -n 1 -o %MEM -p 1,2 | sed '1,5d' |awk '{print $1,$5,$6,$7,$10,$12}' | column -t

# print the free memory
echo -e "\n-----------------------------\n"
free

# print the whole. The more older the pid is the longer time it has exist
echo -e "\n-----------------------------\n"
top -b -n 1 -o -PID | sed '1,5d' |awk '{print $1,$5,$6,$7,$10,$12}' | column -t


