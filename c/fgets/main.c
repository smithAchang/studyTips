#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <signal.h> 
#include <unistd.h>
#include <errno.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/prctl.h>
#include <sys/epoll.h>
#include <netinet/icmp6.h>      // struct nd_neighbor_solicit/advert, which contains icmp6_hdr and ND_NEIGHBOR_ADVERT
#include <netinet/in.h>         // IPPROTO_IPV6, IPPROTO_ICMPV6, INET6_ADDRSTRLEN
#include <netinet/ip.h>         // IP_MAXPACKET (65535)
#include <arpa/inet.h>          // inet_pton() and inet_ntop()
#include <poll.h>
#include <fcntl.h>




const char* cnGetSysErrnoDesc()
{
 static __thread char   s_acErrorDesc[512];
 s_acErrorDesc[0] = '\0';
 strerror_r(errno,s_acErrorDesc,sizeof(s_acErrorDesc));
 return  s_acErrorDesc;
}


int main(int argc, char* argv[])
{
    int nonblockingFlag = 0;
    
    if(argc > 1)
    {
      nonblockingFlag = 1;        
    }

    int pipefds[2];
    int rc;
    rc = pipe(pipefds);
    if(rc < 0)
    {
        perror("Create pipe in failure");
        return -1;
    }
    
    pid_t child = fork();
    
    if(child == 0)
    {
        printf("Child process must exit when parent process exited\n");
        prctl(PR_SET_PDEATHSIG, SIGTERM);

        printf("Child process set self to be the leader of process group\n");
        setpgid(0, 0);
        int blackhole = open("/dev/null", O_RDWR | O_CLOEXEC);
        if(blackhole < 0)
        {
            return -1;
        }
        
        dup2(blackhole, STDIN_FILENO);
        
        printf("Redirect the standard out and standard err\n");
        dup2(pipefds[1], STDOUT_FILENO);
        dup2(pipefds[1], STDERR_FILENO);
        
        printf("Sleep a while to watch the blocking status of fgets API\n"); 
        sleep(10);
        
        printf("fflush the output streams if any\n"); 

        fflush(stdout);
        fflush(stderr);

        /*immediately exit*/        
        _exit(0);
    }
    
    printf("Parent process continues to run ...\n");
    close(pipefds[1]);
    
    if(nonblockingFlag)
    {
        printf("Set nonblocking flag\n");
        int oldFlags = fcntl(pipefds[0], F_GETFL);
        fcntl(pipefds[0], F_SETFL, oldFlags | O_NONBLOCK);
    }
    else
    {
        printf("Using blocked fd APIs\n");
    }
    
    FILE* fp = fdopen(pipefds[0], "r");
    
    if(fp == NULL)
    {
        perror("fdopen pipe fd in failure");
        return -1;
    }
    
    int status;
    pid_t chckpid;
    char szLineBuf[512];
    char* szLine;
    
    do
    {
        szLine = fgets(szLineBuf, sizeof(szLineBuf), fp);
        if(szLine == NULL)
        {
           continue; 
        }
        
        printf("Child process output: %s\n", szLine);
        status = 0;
        chckpid = waitpid(child, &status, WNOHANG | WEXITED);
    }while(chckpid == 0 /*running*/);
    
    printf("Detect child process has exited ...\n");
    
    do
    {
        szLine = fgets(szLineBuf, sizeof(szLineBuf), fp);
        if(szLine == NULL)
        {
           perror("Read child process's lefted output");
           break; 
        }
        
        printf("Child process lefted output: %s\n", szLine);
    }while(szLine != NULL);
    
    if(WIFEXITED(status))
    {
        printf("Normally exited\n");
    }
    
    if(WIFSIGNALED(status))
    {
        printf("exited by signal: %d\n", WTERMSIG(status));
    }
    
    fclose(fp);
    printf("Will exit !!!\n");
	return 0;
}

