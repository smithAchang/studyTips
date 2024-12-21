#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <signal.h> 
#include <unistd.h>
#include <errno.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/epoll.h>
#include <netinet/icmp6.h>      // struct nd_neighbor_solicit/advert, which contains icmp6_hdr and ND_NEIGHBOR_ADVERT
#include <netinet/in.h>         // IPPROTO_IPV6, IPPROTO_ICMPV6, INET6_ADDRSTRLEN
#include <netinet/ip.h>         // IP_MAXPACKET (65535)
#include <arpa/inet.h>          // inet_pton() and inet_ntop()
#include <poll.h>
#include <fcntl.h>


typedef unsigned short    WORD16;
typedef unsigned short    WORD;
typedef unsigned int    WORD32;
typedef unsigned int    DWORD;
typedef int SOCKET;

#define EPOLL_EVENTS (EPOLLIN|EPOLLHUP|EPOLLRDHUP|EPOLLET)

#ifndef likely
#define likely(x)   __builtin_expect(!!(x), 1)
#endif /* likely */
#ifndef unlikely
#define unlikely(x) __builtin_expect(!!(x), 0)
#endif /* unlikely */

const char* cnGetSysErrnoDesc()
{
 static __thread char   s_acErrorDesc[512];
 s_acErrorDesc[0] = '\0';
 strerror_r(errno,s_acErrorDesc,sizeof(s_acErrorDesc));
 return  s_acErrorDesc;
}


int main(int argc, char* argv[])
{
 
    if(argc < 2)
    {
        printf("You must provide the dest addr!\n");
        return -1;
    }

    printf("The dest addr: %s\n", argv[1]);
   
	int tSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (-1 == tSock)
	{
		printf("\n[OSS]:%s,Can not get a Socket errno:%d,errnoDesc:%s\n",__func__, errno,cnGetSysErrnoDesc());
		return -1;
	}
    char buf[8192];

    /*use local socket to test*/
	unsigned short wPort  = 6775; 
	struct sockaddr_in  tSocketAddr = {};
	tSocketAddr.sin_family          = AF_INET; 
    inet_pton(AF_INET, argv[1], &tSocketAddr.sin_addr);
	tSocketAddr.sin_port            = htons(wPort);

    printf("Send data to a non-exist IP ....\n");
    for(size_t i=0; i<65536; ++i)
    {
        time_t  begin = time(NULL);
        ssize_t nums = sendto(tSock, buf, sizeof(buf), 0, (const struct sockaddr *)&tSocketAddr, sizeof(tSocketAddr));
        time_t end  = time(NULL);
        printf("Has send %zd bytes at loop: %zu when time:%ld -> %ld\n", nums, i, begin, end);
        if(nums < 0)
        {
            printf("errno: %d, desc: %s\n", errno, cnGetSysErrnoDesc());
        }

        if((end - begin) > 1)
        {
            printf("Has been blocked long time ..........................................................\n");
            sleep(10);
        }
    }
  
    printf("will exit !!!\n");
	return 0;
}

