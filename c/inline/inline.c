#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "inline.h"

inline int foo(int t)
{
   return ++t;
}

int usefoo(int c)
{
  return c + foo(c);
}
