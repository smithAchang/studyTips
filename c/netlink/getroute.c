#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>
#include <sys/types.h>  
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>


#define RC_EXCEPTION_RETURN(EXPR, FORMAT, args...) if(EXPR)\
{\
    strerror_r(errno, szErrno, sizeof(szErrno));\
    fprintf(stderr, FORMAT" errno: %d -> %s\n", ## args, errno, szErrno);\
    return -1;\
}

#define RC_EXCEPTION_GOTO_FAIL(EXPR, FORMAT, args...) if(EXPR)\
{\
    strerror_r(errno, szErrno, sizeof(szErrno));\
    fprintf(stderr, FORMAT" errno: %d -> %s\n", ## args, errno, szErrno);\
    goto fail;\
}

int nl_filter_route(const struct nlmsghdr* n, void* arg)
{
  
  printf("recv para: %p\n", arg);

  struct rtattr * tb[RTA_MAX + 1];
  char  anyAddr[16] = {};

  const struct rtmsg* r = (const struct rtmsg*)NLMSG_DATA(n);

  if(n->nlmsg_type != RTM_GETROUTE)
  {
    fprintf(stderr,"unrecognize msg: %d\n", n->nlmsg_type);
    return -1;
  }

  int len = n->nlmsg_len - NLMSG_LENGTH(sizeof(struct rtmsg));
  if(len < 0)
  {
    fprintf(stderr,"too short: %d\n",n->nlmsg_len);
    return -1;
  }

  memset(tb, 0 , sizeof(tb));
  
  struct rtattr* rta = RTM_RTA(r);
  while(RTA_OK(rta, len))
  {
    if(rta->rta_type <= RTA_MAX)
    {
      tb[rta->rta_type] = rta;
    }
    rta = RTA_NEXT(rta, len);
  }

  if(tb[RTA_IIF] != NULL)
  {
    printf("%d\n", *(int*)RTA_DATA(tb[RTA_IIF]));
  }

  if(tb[RTA_OIF] != NULL)
  {
    printf("%d\n", *(int*)RTA_DATA(tb[RTA_OIF]));
  }

  if(tb[RTA_SRC] != NULL)
  {
    printf("%s\n", (char*)RTA_DATA(tb[RTA_SRC]));
  }

  return 0;
}

int main(int argc , char* argv[])
{
  char szErrno[256];
  if(argc < 2)
  {
    fprintf(stderr, "Usage: program dst\n");
    return -1;
  }

  RC_EXCEPTION_RETURN(argc < 2, "Usage program dst");

  size_t dstlen = strlen(argv[1]);

  printf("Hello World!\n");

  int tNetlinkUDPSock = socket(AF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);

  RC_EXCEPTION_RETURN(tNetlinkUDPSock < 0, "create net link sock in failure!");

  struct {
      struct nlmsghdr nlh;
      struct rtmsg    getr;
      struct rtattr   dst;
      unsigned char   data[64];
  } req = {};

  req.nlh.nlmsg_len  = NLMSG_LENGTH(sizeof(struct rtmsg));
  /*man 7 rtnetlink*/
  req.nlh.nlmsg_type = RTM_GETROUTE;
  // NLM_F_DUMP /*special for GET REQUEST, return all entries matching criteria passed in message*/;
  req.nlh.nlmsg_flags = NLM_F_REQUEST;
  req.nlh.nlmsg_pid   = 1234;
  req.nlh.nlmsg_seq   = 1;
  
  req.getr.rtm_family  = AF_INET6;
  req.getr.rtm_table   = RT_TABLE_UNSPEC;
  req.getr.rtm_table   = RTPROT_UNSPEC;

  req.dst.rta_len  = RTA_LENGTH(dstlen + 1);
  req.dst.rta_type = RTA_DST;
  strcpy(RTA_DATA(&req.dst), argv[1]);

  int rc = send(tNetlinkUDPSock, &req, sizeof(req), 0);

  RC_EXCEPTION_GOTO_FAIL(rc < 0, "send net link req in failure!");

  struct pollfd fds[1] = {};
  fds[0].fd     = tNetlinkUDPSock;
  fds[0].events = POLLIN;


  rc = poll(fds, 1, 2*1000 /*ms*/);

  RC_EXCEPTION_GOTO_FAIL(rc < 0, "wait response in failure!");

  struct sockaddr_nl snl;
  socklen_t addrlen = sizeof(snl);

  unsigned char buf[4096];

  ssize_t recvlen = recvfrom(tNetlinkUDPSock, buf, sizeof(buf), MSG_DONTWAIT | MSG_TRUNC, (struct sockaddr*)&snl, &addrlen);

  RC_EXCEPTION_GOTO_FAIL(rc < 0, "recv response in failure!");  
  RC_EXCEPTION_GOTO_FAIL(recvlen > (ssize_t)sizeof(buf), "recv response is too big! recvlen: %ld", recvlen);  

  const struct nlmsghdr * h = (const struct nlmsghdr*)buf;
  ssize_t msglen    = recvlen;

  while(NLMSG_OK(h, msglen))
  {
      if(h->nlmsg_pid == 0 || (h->nlmsg_pid != 0 && h->nlmsg_pid != req.nlh.nlmsg_pid))
      {
        goto skip_it;
      }

      if(h->nlmsg_type == NLMSG_DONE)
      {
        printf("reach the end!\n");
        break;
      }

      if(h->nlmsg_type == NLMSG_ERROR)
      {
        if(h->nlmsg_len < NLMSG_LENGTH(sizeof(struct nlmsgerr)))
        {
          fprintf(stderr, "error: ERROR truncated");
        }
        else
        {
          struct nlmsgerr *err = (struct nlmsgerr*) NLMSG_DATA(h);
          if(err->error == 0)
          {
            /*If the error field is zero, then this is an ACK*/
            goto skip_it;
          }

          fprintf(stderr, " error: %s, type=%u, seq=%u, pid=%d\n", strerror(-err->error), err->msg.nlmsg_type, err->msg.nlmsg_seq, err->msg.nlmsg_pid);
        }

        break;
      }

      if(h->nlmsg_type == NLMSG_ERROR)
      {

      }

      skip_it:
         h = NLMSG_NEXT(h, msglen);

      /*get the response*/
  }

  RC_EXCEPTION_GOTO_FAIL(msglen > 0, "parse data has remnant of size : %ld", msglen);  

 
  printf("test program will exit!\n"); 
  close(tNetlinkUDPSock);
  return 0;

fail:
  fprintf(stderr, "in failure, test program will exit!\n");   
  close(tNetlinkUDPSock);
  return -1;
}
