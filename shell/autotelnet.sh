#!/bin/bash
#

telnetServer=127.0.0.1
telnetPort=23
preSteps=("telnetuser" "888888")
postSteps=("exit")
steps=()

function readOutputToEOF()
{

  while IFS= read -r line
  do
    if [ -z "$line" ]; then
      continue
    fi

    echo "$line"
  done <&3
}

function execSteps()
{
  for step in "${steps[@]}"; do
    echo "$step" >&3
  done

  readOutputToEOF
}

# Main
curl -m 1 http://$telnetServer:$telnetPort &> /dev/null
if [ $? -eq 7 ]; then
  echo "Can't connect to server!"
  exit 1
fi

# Open the telnet connection
exec 3<> /dev/tcp/$telnetServer/$telnetPort

if [ $? -ne 0 ]; then
  echo "Can't connect to server(has a long timeout)!"
  exit 1
fi

## Entry
case "$1" in
  "show")
   steps=("${preSteps[@]}")

   steps+=("ls -l")
   
   steps+=("${postSteps[@]}")
   execSteps
  ;;
  *)
  echo "operation type error, please input operation type in (show)!"
  ;;
esac
