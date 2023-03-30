#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define foreach_timerid_field() \
_(u32StartupID, "startup timer")\
_(u32SetupID,   "setup timer")\
_(u32DestroyID,   "destroy timer")\

struct A
{
  unsigned int type;
  unsigned int len;

  #define _(sym, desc) uint32_t sym;
    foreach_timerid_field()
  #undef _

  unsigned char data[0];
};

struct B
{
  struct A head;
  int i;
  int j;
  int k;
};

struct C
{
  struct A head;
  int h;
  int m;
  int n;
};

struct Msgs
{
  union
  {
    struct A a;
    struct B b;
    struct C c;
  };
};


int main(void)
{

  unsigned char encodeBuf[2048];
  struct A *a = (struct A *)encodeBuf;

  #define _(sym, desc) a->sym = -1;
    foreach_timerid_field()
  #undef _
  
  struct A aa = {};

  #define _(sym, desc) aa.sym = -1;
    foreach_timerid_field()
  #undef _

  #define ptrUsing() (&aa)->
  #define _(sym, desc) sym = -1;
    ptrUsing()foreach_timerid_field()
  #undef _
  #undef ptrUsing

  printf("\nsizeof(A)=%zu, sizeof(A.data)=%zu", sizeof(*a), sizeof(a->data));

  char src[] = "Hello World!";
  unsigned int copyLen = sizeof(src);

  memcpy(a->data, src, copyLen);

  struct B *b = (struct B *)encodeBuf;
  printf("\nsizeof(B.head)=%zu, sizeof(B)=%zu", sizeof(b->head), sizeof(*b));
  memcpy(b->head.data, src, copyLen);

  printf("\n");
  return 0;
}
