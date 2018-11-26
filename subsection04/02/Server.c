#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SERVER_IP_ADDR     "127.0.0.1"
#define SERVER_PORT        9234
#define MAX_CONNECT        1010

int main(int argn, char **argv)
{
    int listenfd, connfd;
    struct sockaddr_in serverSock;
    struct pollfd *client;
    int maxClientIdx, nReady, i;
    ssize_t nRead;
    char *buf;

    buf = malloc(BUFSIZ);
    client = calloc(MAX_CONNECT, sizeof(struct pollfd));
    if((buf == NULL) || (client == NULL))
        printf("malloc error!\n");

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
//    printf("listenfd = %d\n", listenfd);
    
    memset(&serverSock, 0, sizeof(serverSock));
    serverSock.sin_family = AF_INET;
    inet_aton(SERVER_IP_ADDR, &(serverSock.sin_addr));
    serverSock.sin_port = htons(SERVER_PORT);

    i = bind(listenfd, (struct sockaddr*)&serverSock, sizeof(serverSock));
    if(i != 0)
        printf("bind error!\n");
    i = listen(listenfd, MAX_CONNECT);
    if(i != 0)
        printf("listen error!\n");

    client[0].fd = listenfd;
    client[0].events = POLLIN;
    for(i = 1; i < MAX_CONNECT; i++)
        client[i].fd = -1;
    maxClientIdx = 0;
    
//    printf("server start!\n");
    while(1){
        nReady = poll(client, maxClientIdx + 1, -1);
        if(nReady <= 0)
            continue;

        if(client[0].revents & POLLIN){
            connfd = accept(listenfd, NULL, NULL);
//            printf("accept connfd = %d", connfd);
            if(connfd > 0){
                for(i = 1; i < MAX_CONNECT; i++)
                    if(client[i].fd < 0){
                        client[i].fd = connfd;
                        client[i].events = POLLIN;
                        client[i].revents = 0;
                        break;
                    }

                if(i == MAX_CONNECT)
                    printf("to many clients!\n");
                if(i > maxClientIdx)
                    maxClientIdx = i;
            }
            if(--nReady <= 0)
                continue;
        }

        for(i = 1; i <= maxClientIdx; i++){
            if(client[i].fd < 0)
                continue;
            if(client[i].revents & POLLIN){
                if((nRead = read(client[i].fd, buf, BUFSIZ)) <= 0){
                    close(client[i].fd);
                    client[i].fd = -1;
                } else {
                    write(STDOUT_FILENO, buf, nRead);
                    if(strcmp(buf, "ping") == 0){
                        write(client[i].fd, "pong", 5);
                    } 
                }

                if(--nReady <= 0)
                    continue;
            }
        }
    }
}
