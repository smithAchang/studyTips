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

void usage(char* argv[])
{
    char szContinueSpaces[128];
    memset(szContinueSpaces, ' ', sizeof(szContinueSpaces) - 1);
    szContinueSpaces[sizeof(szContinueSpaces) - 1] = '\0';

    char szOptionIndentFormatStr[128];
    char szUsageHeader[128];
    char szOptionIndent[128];
    snprintf(szUsageHeader, sizeof(szUsageHeader), "Usage: %s", argv[0]);
    /*minwidth, maxwidth*/
    const size_t  prefixLen = strlen(szUsageHeader);
    snprintf(szOptionIndentFormatStr, sizeof(szOptionIndentFormatStr), "%%%zu.%zus", prefixLen, prefixLen);
    snprintf(szOptionIndent, sizeof(szOptionIndent), szOptionIndentFormatStr, szContinueSpaces);
    fprintf(stderr, "Usage: %s [-s]\n"
                    "%s with no option,proc will wait any key before exit\n"
                    "%s -s indicate proc to run infinitly\n", argv[0], szOptionIndent, szOptionIndent);
}

int main(int argc, char* argv[])
{
    int serviceFlag = 0, aFlagValue = 0;
    int opt;
    while((opt = getopt(argc, argv, "s")) != -1)
    {
        switch(opt)
        {
            case 's':
                printf("has set -s flag!\n");
                serviceFlag = 1;
                break;
            case 'a':
                aFlagValue = atoi(optarg);
                printf("has set -a value option with value: %d\n", aFlagValue);
                break;
            default: /* '?' */
                usage(argv);
                return 1;
        }
    }
    
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
        goto wait;
    } 
    else 
    {
        fprintf(stderr, "%s raw socket supported\n", szTips);
    }

    close(fd);

wait:
    if(serviceFlag == 0)
    {
        if(pid != 0)
        {
            const unsigned int seconds = 1;
            sleep(seconds);
            printf("Parent role, enter any key to exit ...\n");      
            getchar();    
        }
        else
        {
            printf("Child will exit!\n");
        }
    }
    else
    {
        unsigned int count = 0;
        if(pid != 0)
        {
            while(1)
            {
                printf("Parent is running as service, count: %u ...\n", count);
                const unsigned int seconds = 1;
                sleep(seconds);            
            }
        }
        else
        {
            printf("Child will exit, only Parent is running as service!\n");
        }
    }        
    
    
    return 0;
}