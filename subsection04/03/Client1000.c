#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <poll.h>
#include <signal.h>
#include <stdlib.h>

#define SERVER_IP_ADDR     "127.0.0.1"
#define SERVER_PORT        9234
#define CLIENT_NUM         1000

#define SYS_TICK           10                // ms
#define DELAY_CLOSE_TIME   (10 * 1000)       // ms

typedef struct{
    int fd;
    long closeTick;
} WaitCloseClient_t;

int main(int argn, char **argv)
{
    int i;
    char *buf = malloc(BUFSIZ);
    if (buf == NULL) {
        printf("malloc buf error!\n");
        return 0;
    }
    struct pollfd *client = calloc(CLIENT_NUM, sizeof(struct pollfd));
    if (client == NULL) {
        printf("malloc client error!\n");
        free(buf);
        return 0;
    }
    WaitCloseClient_t *waitCloseClient = calloc(CLIENT_NUM, sizeof(WaitCloseClient_t));
    if (waitCloseClient == NULL) {
        printf("malloc waitCloseClient fail!\n");
        free(buf);
        free(client);
        return 0;
    }
    struct sockaddr_in serverSock;
    memset(&serverSock, 0, sizeof(serverSock));
    serverSock.sin_family = AF_INET;
    inet_aton(SERVER_IP_ADDR, &serverSock.sin_addr);
    serverSock.sin_port = htons(SERVER_PORT);

    int aliveClientNum = 0;
    for (i = 0; i < CLIENT_NUM; i++) {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            printf("client[%d] socket error!\n", i);
        } else {
            int val = fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK;
            fcntl(sockfd, F_SETFL, val);
            aliveClientNum ++;
        }
        waitCloseClient[i].fd = sockfd;
        waitCloseClient[i].closeTick = -1;
        client[i].fd = sockfd;
        client[i].events = POLLOUT;
    }
    if (aliveClientNum == 0) {
        printf("all client socket error!\n");
        free(client);
        free(buf);
        free(waitCloseClient);
        return 0;
    }
    //start!
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);
    //connect
    for (i = 0; i < CLIENT_NUM; i++) {
        if (client[i].fd != -1) {
            connect(client[i].fd, (struct sockaddr*)&serverSock, sizeof(serverSock));
        }
    }
    const char sendMsg[] = "ping\n";
    const char recvMsg[] = "pong\n";
    long sysTick = 0;
    while(1){
        int nReady = poll(client, CLIENT_NUM, SYS_TICK);
        if (nReady == 0) {
            sysTick ++;
            for (i = 0; i < CLIENT_NUM; i ++) {
                if ((waitCloseClient[i].fd != -1) && (waitCloseClient[i].closeTick != -1)
                        && (sysTick >= waitCloseClient[i].closeTick)) {
                    close(waitCloseClient[i].fd);
                    waitCloseClient[i].fd = -1;
                    if (--aliveClientNum <= 0) {
                        free(client);
                        free(buf);
                        free(waitCloseClient);
                        return 0;
                    }
                }
            }
            continue;
        }
        if (nReady < 0) {
            printf("epoll run fail!\n");
            continue;
        }
        for (i = 0; i < CLIENT_NUM; i++) {
            if (client[i].fd == -1)
                continue;
            if (client[i].revents & POLLOUT) {
                if (write(client[i].fd, sendMsg, sizeof(sendMsg))) {
                    client[i].events = POLLIN;
                    client[i].revents = 0;
                } else {
                    printf("client[%i] write error, closed!\n", i);
                    close(client[i].fd);
                    client[i].fd = -1;
                    aliveClientNum --;
                    waitCloseClient[i].fd = -1;
                }
                if (--nReady <= 0)
                    break;
                continue;
            }

            if (client[i].revents & POLLIN) {
                if (read(client[i].fd, buf, BUFSIZ) > 0) {
                    if (strcmp(buf, recvMsg) == 0) {
                        gettimeofday(&endTime, NULL);
                        int consumeTime = (int64_t)endTime.tv_sec * 1000 + endTime.tv_usec/1000
                                - (int64_t)startTime.tv_sec * 1000 - startTime.tv_usec/1000;
                        printf("client[%d] consumeTime: %d ms\n", i, consumeTime);

                        waitCloseClient[i].closeTick = sysTick + (DELAY_CLOSE_TIME/SYS_TICK);
                        client[i].fd = -1;
                    }
                } else {
                    printf("client[%d] read error! close!\n", i);
                    close(client[i].fd);
                    client[i].fd = -1;
                    aliveClientNum --;
                    waitCloseClient[i].fd = -1;
                }

                if(--nReady <= 0)
                    break;
            }
        }
    }
}
