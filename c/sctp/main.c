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
  int fd;

  fd = socket(PF_INET, SOCK_STREAM, SHOULD_IPPROTO_SCTP);

  if (fd <= 0) {
    perror("checksctp");
    exit(1);
  } else {
    fprintf(stderr, "SCTP supported\n");
  }

  close(fd);
  return 0;
}
