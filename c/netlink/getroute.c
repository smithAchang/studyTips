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


#define RC_EXCEPTION_RETURN(EXPR, FORMAT, args...) do {\
  if(EXPR)\
  {\
      strerror_r(errno, szErrno, sizeof(szErrno));\
      fprintf(stderr, FORMAT" errno: %d -> %s\n", ## args, errno, szErrno);\
      return -1; \
  }\
}while(0)

#define RC_EXCEPTION_GOTO_FAIL(EXPR, FORMAT, args...) do {\
  if(EXPR)\
  {\
      strerror_r(errno, szErrno, sizeof(szErrno));\
      fprintf(stderr, FORMAT" errno: %d -> %s\n", ## args, errno, szErrno);\
      goto fail_flow; \
  }\
}while(0)

int  __attribute__((nonnull(2))) nl_filter_route(int family, const struct nlmsghdr* n, unsigned short expected_msgtype, void* arg)
{
  
  printf("recv para: %p\n", arg);

  struct rtattr * tb[RTA_MAX + 1];

  const struct rtmsg* r = (const struct rtmsg*)NLMSG_DATA(n);

  if(n->nlmsg_type != expected_msgtype)
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

  printf("come from route netlink message\n"
         "rtm_family:     %u\n"
         "rtm_dst_len:    %u\n"
         "rtm_src_len:    %u\n"
         "rtm_table:      %u\n"
         "rtm_protocol:   %u\n"
         "rtm_scope:      %u\n",
          r->rtm_family,
          r->rtm_dst_len,
          r->rtm_src_len,
          r->rtm_table,
          r->rtm_protocol,
          r->rtm_scope
        );

  printf("\ncome from following rta(s)\n");
  if(tb[RTA_TABLE] != NULL)
  {
    printf("RTA_TABLE: %d\n", *(int*)RTA_DATA(tb[RTA_TABLE]));
  }

  if(tb[RTA_IIF] != NULL)
  {
    printf("RTA_IIF: %d\n", *(int*)RTA_DATA(tb[RTA_IIF]));
  }

  if(tb[RTA_OIF] != NULL)
  {
    printf("RTA_OIF: %d\n", *(int*)RTA_DATA(tb[RTA_OIF]));
  }

  if(tb[RTA_SRC] != NULL)
  {
    printf("RTA_SRC: %s\n", (char*)RTA_DATA(tb[RTA_SRC]));
  }

  if(tb[RTA_DST] != NULL)
  {
    char szDst[64] = {};
    inet_ntop(family, RTA_DATA(tb[RTA_DST]), szDst, sizeof(szDst));
    printf("RTA_DST: %s\n", szDst);
  }

  return 0;
}

#define PROG_USAGE() fprintf(stderr, "Usage: %s [-a] dst\n", argv[0])

int main(int argc , char* argv[])
{
  char szErrno[256];
  if(argc < 2)
  {
    PROG_USAGE();
    return -1;
  }

  int ack_flag = 0, opt;


  while ((opt = getopt(argc, argv, "a")) != -1) 
  {
     switch (opt) 
     {
       case 'a':
           ack_flag = 1;
           break;
       default: /* '?' */
           PROG_USAGE();
           return -1;
     }
  }


  RC_EXCEPTION_RETURN(argc < 2, "Usage program dst");

  int rc;
  size_t dstlen = strlen(argv[1]);

  printf("Hello World!\n");

  int tNetlinkUDPSock = socket(AF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);

  RC_EXCEPTION_RETURN(tNetlinkUDPSock < 0, "create net link sock in failure!");

  struct {
      struct nlmsghdr nlh;
      struct rtmsg    getr;/*route relative msg*/
      struct 
      {
        struct rtattr   h;
        unsigned char   data[64];  
      } dst;   
  } req = {};

  
  /*man 7 rtnetlink*/
  req.nlh.nlmsg_type    = RTM_GETROUTE;
  
  // NLM_F_DUMP /*special for GET REQUEST, return all entries matching criteria passed in message*/;
  req.nlh.nlmsg_flags   = NLM_F_REQUEST;
  if(ack_flag)
  {
    req.nlh.nlmsg_flags |= NLM_F_ACK;
  }

  req.nlh.nlmsg_pid     = 1234;
  req.nlh.nlmsg_seq     = 1;
  req.getr.rtm_family   = AF_INET6;
  req.getr.rtm_table    = RT_TABLE_UNSPEC;
  req.getr.rtm_protocol = RTPROT_UNSPEC;

  req.dst.h.rta_type    = RTA_DST;
  struct in6_addr in6Addr;
  rc                    = inet_pton(AF_INET6, argv[1], &in6Addr.s6_addr);
  if(rc                 <= 0)
  {
    /*0 is family or format is invalid*/
    req.getr.rtm_family   = AF_INET;
    req.dst.h.rta_len     = RTA_LENGTH(sizeof(struct in_addr));
  }
  else
  {
    req.dst.h.rta_len     = RTA_LENGTH(sizeof(struct in6_addr));
  }
  inet_pton(req.getr.rtm_family, argv[1], RTA_DATA(&req.dst.h));


  req.nlh.nlmsg_len     = NLMSG_LENGTH(sizeof(struct rtmsg) + req.dst.h.rta_len);
  

  /*socket in a connected state, the intended recipient is known*/
  rc = send(tNetlinkUDPSock, &req, sizeof(req), 0/*flags*/);

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

  RC_EXCEPTION_GOTO_FAIL(recvlen < 0, "recv response in failure!");  
  RC_EXCEPTION_GOTO_FAIL(recvlen > (ssize_t)sizeof(buf), "recv response is too big! recvlen: %ld", recvlen);  

  const struct nlmsghdr * h = (const struct nlmsghdr*)buf;
  ssize_t msglen            = recvlen;
  pid_t   self_pid          = getpid();

  while(NLMSG_OK(h, msglen))
  {
      if(h->nlmsg_pid == 0 || (h->nlmsg_pid != 0 && (h->nlmsg_pid != req.nlh.nlmsg_pid && (pid_t)h->nlmsg_pid != self_pid)))
      {
        fprintf(stderr, "error: not expected response msg! self_pid: %d, request nlmsg_pid: %d, response nlmsg_pid: %d", self_pid, req.nlh.nlmsg_pid, h->nlmsg_pid);
        goto skip_it;
      }

      if(h->nlmsg_type == NLMSG_DONE)
      {
        printf("reach the end! NLMSG_DONE \n");
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
            printf("got response ack! self_pid: %d, request nlmsg_pid: %d, seq: %d, response nlmsg_pid: %d, seq: %d", self_pid, req.nlh.nlmsg_pid, req.nlh.nlmsg_seq, h->nlmsg_pid, h->nlmsg_seq);
            goto skip_it;
          }

          fprintf(stderr, " error: %s, type=%u, seq=%u, pid=%d\n", strerror(-(err->error)), err->msg.nlmsg_type, err->msg.nlmsg_seq, err->msg.nlmsg_pid);
        }

        break;
      }

      printf("get response nlmsg: %u ...\n", h->nlmsg_type); 

      /*get the response*/
      nl_filter_route(req.getr.rtm_family, h , RTM_NEWROUTE, NULL);
    
      skip_it:
         h = NLMSG_NEXT(h, msglen);
  }

  RC_EXCEPTION_GOTO_FAIL(msglen > 0, "parse data has remnant of size : %ld", msglen);  

 
  printf("\ntest program will exit!\n"); 
  close(tNetlinkUDPSock);
  return 0;

fail_flow:
  fprintf(stderr, "\nin failure, test program will exit!\n");   
  close(tNetlinkUDPSock);
  return -1;
}
