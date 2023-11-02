#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

/* IPPROTO_SCTP SHOULD be defined in
 * /usr/include/linux/in.h but probably isn't.
 * It is an enum element, not a #define, so we can't easily check.
 */
#define SHOULD_IPPROTO_SCTP 132

int main(void)
{
    char szTips[256] = {};
    pid_t pid = fork();
    if(pid == 0)
    {
       strcpy(szTips, "Child");
       printf("%s process begin to run ...\n", szTips);       
    }
    else
    {
        stpcpy(szTips, "Parent");
        printf("%s process continue to run ...\n", szTips);       
    }
    
    int fd = socket(PF_INET, SOCK_RAW, SHOULD_IPPROTO_SCTP);

    if (fd <= 0) 
    {
        char szFailMessage[512];
        snprintf(szFailMessage, sizeof(szFailMessage), "%s check raw socket cap in failure! errorDesc", szTips);
        perror(szFailMessage);
        exit(1);
    } 
    else 
    {
        fprintf(stderr, "%s raw socket supported\n", szTips);
    }

    close(fd);
    return 0;
}