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

#define SERVER_IP_ADDR     "127.0.0.1"
#define SERVER_PORT        9234

int main(int argn, char **argv)
{
    int sockfd;
    struct sockaddr_in serverSock;
    char buf[BUFSIZ];   
    ssize_t nRead;
    
    int i,level = 0;
    for(i = 0; i < 10; i++){
        if(fork() == 0){
            level = i + 1;
        }
    }

    if((level == 4) || (level == 5))
        return 0;


    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd <= 0)
        printf("socket error!\n");

    memset(&serverSock, 0, sizeof(serverSock));
    serverSock.sin_family = AF_INET;
    inet_aton(SERVER_IP_ADDR, &serverSock.sin_addr);
    serverSock.sin_port = htons(SERVER_PORT);

    i = connect(sockfd, (struct sockaddr*)&serverSock, sizeof(serverSock));
    if(i != 0){
        printf("connect error!\n");
        return 0;
    }
    
//    printf("connect success! sockfd = %d\n", sockfd);
    write(sockfd, "ping", 5);

    while(1){
        if((nRead = read(sockfd, buf, BUFSIZ)) > 0){
            write(STDOUT_FILENO, buf, nRead);
            if(strcmp(buf, "pong") == 0){
                gettimeofday(&endTime, NULL);
                if(endTime.tv_usec < startTime.tv_usec){
                    endTime.tv_usec += 1*1000*1000 - startTime.tv_usec;
                    endTime.tv_sec = endTime.tv_sec -startTime.tv_sec - 1;
                } else {
                    endTime.tv_usec -= startTime.tv_usec;
                    endTime.tv_sec  -= startTime.tv_sec;
                }
                printf("use time: %d.%04d S\n", endTime.tv_sec, endTime.tv_usec/1000);
                sleep(10);
                close(sockfd);
                return 0;
            } else {
                sleep(2);
                write(sockfd, "ping", 5);
            }
        } else {
            printf("connect error! close!\n");
            close(sockfd);
            return 0;
        }
    }
}
