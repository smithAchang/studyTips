#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <stdint.h>


int main(int argc, char *argv[])
{
  printf("Hello world !\n" "add suffix\n" "123\n");

        #define CASE_NAME "Show Usage\n"

  printf("Test String Macro Expand 1: no suffix string\n"CASE_NAME);
  printf("Test String Macro Expand 2: has suffix string, no need space to split\n"CASE_NAME
         "has suffix string when Macro Expand\n");

  printf("Exit from program !\n" CASE_NAME);
  return 0;
}
