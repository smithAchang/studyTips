#!/bin/bash

addr="$2"

name=${addr%+*}
offset=${addr#*+}

base=$(objdump -D $1 | grep "<$name>" | awk '{print $1}')

if [ -z "$base" ]; then
  echo "can't get the base addr of $name"
  exit
fi

base="0x$base"

addr2line -e "$1" $(printf '%x' $((base + offset)))


