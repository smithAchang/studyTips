#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>


int main(int argc, char *argv[])
{
  printf("Hello world test bit field threshold!\n");

  struct {
    union
    {
      uint32_t seq : 20;
      uint32_t pt : 12;
      uint32_t guard;
    };
  } a = {};

  const int SLEEP_SECONDS    = 8;
  const uint32_t BIT_SEQ_MAX = (1 << 20) - 1;
  uint8_t bNeetPrintNext     = 0;


  a.seq = BIT_SEQ_MAX + 2;
  a.pt  = 3;

  printf("Seq value beyond max one: %u ; middle pt value: %u !\n", a.seq, a.pt);

  sleep(SLEEP_SECONDS);

  for (uint32_t i = 0; i < UINT_MAX; ++i)
  {
    ++a.seq;

    if (bNeetPrintNext == 1)
    {
      printf("Seq value next value : 0x%x,  pt value: %u ; guard: 0x%x !\n", a.seq, a.pt, a.guard);
      bNeetPrintNext = 0;
    }

    if (a.seq >= BIT_SEQ_MAX)
    {
      printf("Seq value reach max : 0x%x !\n", a.seq);
      bNeetPrintNext = 1;
    }

    if (a.seq > BIT_SEQ_MAX)
    {
      printf("Seq exceed the threshold value : 0x%x !\n", a.seq);
      break;
    }
  }

  printf("\nSeq value after loop: %u, pt value: %u ; guard: 0x%x !\n\n", a.seq, a.pt, a.guard);
  sleep(SLEEP_SECONDS);

  uint32_t BIT_PT_MAX = (1 << 12) - 1;

  for (uint16_t i = 0; i < USHRT_MAX; ++i)
  {
    ++a.pt;

    if (bNeetPrintNext == 1)
    {
      printf("Pt value next value : 0x%x,  pt value: %u ; guard: 0x%x !\n", a.seq, a.pt, a.guard);
      bNeetPrintNext = 0;
    }

    if (a.pt >= BIT_PT_MAX)
    {
      printf("Pt value reach max : 0x%x !\n", a.pt);
      bNeetPrintNext = 1;
    }

    if (a.pt > BIT_PT_MAX)
    {
      printf("Pt exceed the threshold value : 0x%x !\n", a.pt);
      break;
    }
  }

  printf("\nSeq value after loop: %u, pt value: %u ; guard: 0x%x !\n\n", a.seq, a.pt, a.guard);


  printf("Exit from program !\n");
  return 0;
}
