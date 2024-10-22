# `TCP`在非阻塞模式下部分发送测试场景
+ 启动服务器端监听
+ 客户端连接服务器端
+ 设置客户端连接**非阻塞**状态
+ 每次在客户端连接上发送质数数量的字节，例如，`4099`，并查验发送套接字接口返回值是否出现发送字节小于设定字节的现象

> 服务器端在接受连接后，并不启动读取过程，使得每次发送方发送数据，会出现发送缓冲区累积、接收方接收缓冲区减小，直至出现部分发送场景

# 代码
```c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <pthread.h>

unsigned short PORT = 5678;

static void* thread_func(void* arg)
{
    int tcp_server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(tcp_server_socket <0)
    {
        perror("Create server sock in failure!");
        return NULL;
    }
    
    if(bind(tcp_server_socket, (struct sockaddr*)arg, sizeof(struct sockaddr_in)) < 0)
    {
        perror("Bind service port in failure!");
        return NULL;
    }
    
    if(listen(tcp_server_socket, 5) < 0)
    {
        perror("Listen in failure!");
        return NULL;
    }
    
    while(1)
    {
        printf("Begin to accept client ...\n");
        struct sockaddr_in clientAddr = {};
        socklen_t sockAddrLen = sizeof(clientAddr);
        int clientSockFd  = accept(tcp_server_socket, (struct sockaddr*)&clientAddr, &sockAddrLen);
        
        if(clientSockFd < 0)
        {
            perror("Accept client in failure!");
            return NULL;
        }
    }
    
    return NULL;
}

static void setNonBlocking(int sockfd)
{
    int flags = fcntl(sockfd, F_GETFL, 0);
    
    if(flags == -1) {
        perror("fcntl get");
        return;
    }
    
    flags |= O_NONBLOCK;
    
    if(fcntl(sockfd, F_SETFL, flags) == -1) {
        perror("fcntl set");
        return;
    }
}

int main(int argc, char* argv[])
{
    int opt, rc;
    char szMessage[256];
    
    while((opt = getopt(argc, argv, "p:")) != -1) {
        switch(opt) {
            case 'p':
                PORT = atoi(optarg);
                break;
            default: /* '?' */
                fprintf(stderr, "Usage: %s [-p port] \n", argv[0]);
                return 1;
            
        }
    }
    
    
    struct sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr =  INADDR_ANY;
    serverAddr.sin_port = htons(PORT);
    
    pthread_t thread_id;
    
    if(pthread_create(&thread_id, NULL, thread_func, (void*)&serverAddr) != 0)
    {
        perror("Create child thread in failure");
        return -1;
    }
    
    printf("Sleep to wait server socket startup ...\n");
    sleep(1);
    
    int tcp_client_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if(tcp_client_socket < 0) {
        perror("Create client sock in failure!");
        return -1;
    }
    
    if(connect(tcp_client_socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Client connect server in failure!");
        return -2;
    }
    
    setNonBlocking(tcp_client_socket);
    
    char abBuffer[4099];
    
    while(1) {
        rc = send(tcp_client_socket, abBuffer, sizeof(abBuffer), 0);
        if(rc < 0) {
            perror("Send data to server in failure!");
            break;
        }
        
        if(rc >= 0 && rc < (int)sizeof(abBuffer)){
            printf("Has occured partial sending! sended bytes: %d of totoal size: %zu\n", rc, sizeof(abBuffer));
            break;
        }

        /*sleep a while for observing*/
        sleep(1);
    }
    
    return 0;
}

```