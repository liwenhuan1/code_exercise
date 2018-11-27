#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SERVER_IP_ADDR     "127.0.0.1"
#define SERVER_PORT        9234

int main(int argn, char **argv)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket error!\n");
        return 0;
    }

    struct sockaddr_in serverSock;
    memset(&serverSock, 0, sizeof(serverSock));
    serverSock.sin_family = AF_INET;
    inet_aton(SERVER_IP_ADDR, &serverSock.sin_addr);
    serverSock.sin_port = htons(SERVER_PORT);

    if (connect(sockfd, (struct sockaddr*)&serverSock, sizeof(serverSock)) != 0) {
        printf("connect error!\n");
        close(sockfd);
        return 0;
    }

    const char sendMsg[] = "ping\n";
    const char recvMsg[] = "pong\n";
    if (write(sockfd, sendMsg, sizeof(sendMsg)) <= 0) {
        printf("write sockfd error!\n");
        close(sockfd);
        return 0;
    }

    char buf[BUFSIZ];
    ssize_t nRead;
    while(1){
        if ((nRead = read(sockfd, buf, BUFSIZ)) > 0) {
            buf[nRead] = 0;
            printf("%s", buf);
            if (strcmp(buf, recvMsg) == 0) {
                close(sockfd);
                return 0;
            } else {
                sleep(2);
                if (write(sockfd, sendMsg, sizeof(sendMsg)) <= 0) {
                    printf("write sockfd error!\n");
                    close(sockfd);
                    return 0;
                }
            }
        } else {
            printf("read error! close fd!\n");
            close(sockfd);
            return 0;
        }
    }
}
