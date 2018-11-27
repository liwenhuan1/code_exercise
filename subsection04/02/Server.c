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
    char *buf = malloc(BUFSIZ);
    if (buf == NULL) {
        printf("malloc buf error!\n");
        return 0;
    }
    memset(buf, 0, BUFSIZ);
    struct pollfd *client = calloc(MAX_CONNECT, sizeof(struct pollfd));
    if (client == NULL) {
        printf("malloc client error!\n");
        free(buf);
        return 0;
    }
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        printf("socket error!\n");
        goto ERROR_STEP2;
        return 0;
    }

    struct sockaddr_in serverSock;
    memset(&serverSock, 0, sizeof(serverSock));
    serverSock.sin_family = AF_INET;
    inet_aton(SERVER_IP_ADDR, &(serverSock.sin_addr));
    serverSock.sin_port = htons(SERVER_PORT);
    if (bind(listenfd, (struct sockaddr*)&serverSock, sizeof(serverSock)) != 0) {
        printf("bind error!\n");
        goto ERROR_STEP3;
        return 0;
    }
    if (listen(listenfd, MAX_CONNECT) != 0) {
        printf("listen error!\n");
        goto ERROR_STEP3;
        return 0;
    }

    int i, maxClientIdx = 0;
    client[0].fd = listenfd;
    client[0].events = POLLIN;
    for (i = 1; i < MAX_CONNECT; i++)
        client[i].fd = -1;

    const char recvMsg[] = "ping\n";
    const char replyMsg[] = "pong\n";

    while(1) {
        int nReady = poll(client, maxClientIdx + 1, -1);
        if (nReady <= 0)
            continue;

        if (client[0].revents & POLLIN) {
            int connfd = accept(listenfd, NULL, NULL);
            if (connfd != -1) {
                for (i = 1; i < MAX_CONNECT; i++) {
                    if (client[i].fd < 0) {
                        client[i].fd = connfd;
                        client[i].events = POLLIN;
                        client[i].revents = 0;
                        break;
                    }
                }

                if (i >= MAX_CONNECT) {
                    printf("to many clients!\n");
                    close(connfd);
                } else if (i > maxClientIdx)
                    maxClientIdx = i;
            }
            if (--nReady <= 0)
                continue;
        }

        for (i = 1; i <= maxClientIdx; i++) {
            if (client[i].fd < 0)
                continue;
            if (client[i].revents & POLLIN) {
                ssize_t nRead;
                if ((nRead = read(client[i].fd, buf, BUFSIZ)) <= 0) {
                    close(client[i].fd);
                    client[i].fd = -1;
                } else {
                    buf[nRead] = 0;
                    printf("%s", buf);
                    if (strcmp(buf, recvMsg) == 0) {
                        if (write(client[i].fd, replyMsg, sizeof(replyMsg)) <= 0) {
                            close(client[i].fd);
                            client[i].fd = -1;
                        }
                    }
                }
                if(--nReady <= 0)
                    break;
            }
        }
    }

ERROR_STEP3:
    close(listenfd);
ERROR_STEP2:
    free(client);
    free(buf);

    return 0;
}
