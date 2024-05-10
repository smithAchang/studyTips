#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <stdint.h>

typedef struct tagTRet
{
  int j;
  int k;
  int l;
  int h;
} TRet;


TRet f(unsigned int size)
{
  int a[size];
  int b;
  int c;
  TRet rc = {};

  a[0]        = 1;
  a[1]        = 10;
  a[size - 1] = 100;

  b = 2;
  c = 3;

  return rc;
}

int main(int argc, char *argv[])
{
  printf("Hello world !\n");

  TRet rc = f(8);

  printf("Exit from program !\n");
  return 0;
}
