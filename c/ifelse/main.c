#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

/* IPPROTO_SCTP SHOULD be defined in
 * /usr/include/linux/in.h but probably isn't.
 * It is an enum element, not a #define, so we can't easily check.
 */
#define SHOULD_IPPROTO_SCTP 132

int main(void)
{
  char header[32], data[128];
  struct iovec iovecs[2] = {};

  iovecs[0].iov_base = header;
  iovecs[0].iov_len  = sizeof(header);

  char  *pcData  = data;
  size_t dataLen =  sizeof(data);

  printf(
    "There only has must branch code but maybe with an additional assignment!assignment will access memroy,but jump only change register!\n");
  int iov_num = 1;

  if (pcData != NULL)
  {
    iovecs[1].iov_base = pcData;
    iovecs[1].iov_len  = dataLen;
    iov_num            = 2;

    /*no jump cpu instruction*/
  }

  printf("There has no additional assignment but with an specail jump cpu code!\n");
  int iov_num2;

  if (pcData != NULL)
  {
    iovecs[1].iov_base = header;
    iovecs[1].iov_len  = dataLen;
    iov_num2           = 2;

    /*jump cpu instruction*/
  }
  else
  {
    iov_num2 = 1;
  }

  return 0;
}
