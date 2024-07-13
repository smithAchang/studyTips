/*

 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>

typedef struct A {
    uint8_t u8;
    uint64_t u64;
} A;

/*Packed 编译指令*/
#pragma pack(push, 1)

typedef struct B {
    uint8_t u8;
    uint64_t u64;
} B;

#pragma pack(pop)

typedef struct C {
    uint8_t u8;
    uint64_t u64;
} C;

double getDiff(const struct timeval *start, const struct timeval *end);
double getDiff(const struct timeval *start, const struct timeval *end)
{
    #define USEC_UNIT (1000000)
    unsigned long diff = (end->tv_sec - start->tv_sec) * USEC_UNIT + (end->tv_usec - start->tv_usec);
    return (double)diff/USEC_UNIT;
}


int main(int argc, char **argv)
{
  struct timeval start, end;
  double smalldiff, bigdiff;
  printf("Sizeof(A): %2zu in default\nSizeof(B): %2zu with pack(1)\nSizeof(C): %2zu afer pack setting switch\n", sizeof(A), sizeof(B), sizeof(C));

  A a = {};
  B b = {};
  
  printf("\nThe asm code framewrok is the same, but the consumed time is different.\n");
  printf("Access A member: %lu to see its asm code\n", a.u64);
  printf("Access B member: %lu to see its asm code\n", b.u64);
  
  const unsigned int count = 100000000;
  printf("\nAccess struct member in default pack setting\n");
  
  unsigned int i = count;
  gettimeofday(&start, NULL);
  while(i--)
  {
      a.u64 += a.u8;
      ++a.u8;
  }
  gettimeofday(&end, NULL);
  smalldiff = getDiff(&start, &end);
  printf("End time: %ld.%ld, diff: %6lf in pack(default) setting\n", end.tv_sec, end.tv_usec, getDiff(&start, &end));
  
  i = count;
  gettimeofday(&start, NULL);
  while(i--)
  {
      b.u64 += b.u8;
      ++b.u8;
  }
  gettimeofday(&end, NULL);
  bigdiff = getDiff(&start, &end);
  printf("End time: %ld.%ld, diff: %6lf in pack(1) setting\n", end.tv_sec, end.tv_usec, getDiff(&start, &end));
  printf("Diff Percent: %lf%%\n", (bigdiff - smalldiff)*100/bigdiff);
  
  return 0;
}
