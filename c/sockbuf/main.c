#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>


int main(void)
{
  char szErrno[256];

  printf("\nHello World!");

  int tSock = socket(PF_INET, SOCK_DGRAM, 0);

  if (tSock < 0)
  {
    strerror_r(errno, szErrno, sizeof(szErrno));
    printf("\ncreate sock errno: %d -> %s", errno, szErrno);
    return -1;
  }
  printf("\ncreate sock successfully, got sock fd: %d", tSock);


  struct sockaddr_in tSockAddr = {};
  tSockAddr.sin_family = AF_INET;

  // tSockAddr.sin_addr.s_addr = 0;
  tSockAddr.sin_port = htons(12701);

  int rc = bind(tSock, (struct sockaddr *)&tSockAddr, sizeof(tSockAddr));

  if (rc < 0)
  {
    strerror_r(errno, szErrno, sizeof(szErrno));
    printf("\nbind errno: %d -> %s", errno, szErrno);
    return -1;
  }
  printf("\nbind successfully !");

  unsigned int bufSize = 1024 * 1024 * 1024;
  rc =  setsockopt(tSock, SOL_SOCKET, SO_RCVBUF, (const void *)&bufSize, sizeof(bufSize));
  printf("\nsetsockopt rc: %d", rc);

  unsigned int optval = 0;

  /*optlen must be as in & out parameter*/
  socklen_t optlen;
  rc = getsockopt(tSock, SOL_SOCKET, SO_RCVBUF, (void *)&optval, &optlen);
  printf("\ngetsockopt without providing optlen value. rc: %d, has set bufsize: %u, optlen: %u", rc, optval, optlen);

  // compare at CentOS7 env
  optlen = sizeof(optval);
  rc     = getsockopt(tSock, SOL_SOCKET, SO_RCVBUF, (void *)&optval, &optlen);
  printf("\ncompare getsockopt rc: %d, has set bufsize: %u, optlen: %u", rc, optval, optlen);

  printf("\n");
  close(tSock);

  return 0;
}
