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

#define SERVER_IP_ADDR     "127.0.0.1"
#define SERVER_PORT        9234

#define CLIENT_NUM         1000

#define DELAY_CLOSE_TIME   10

typedef struct{
    int fd;
    struct timeval closeTime; 
} CompleteTimer_t;

static CompleteTimer_t *completeQueue;
static int completeCnt = 0;
static int startIdx = 0;
static int endIdx = 0;
static int aliveClientNum = CLIENT_NUM;
static struct itimerval completeTimer;

static void timeoutProcess(int signo)
{
//    printf("singal ok!\n");
    
    close(completeQueue[startIdx].fd);
    startIdx = (++startIdx) % CLIENT_NUM;
    completeCnt --;

    struct timeval curTime;
    gettimeofday(&curTime, NULL);

    while(completeCnt){
        if((curTime.tv_sec > completeQueue[startIdx].closeTime.tv_sec) || 
            ((curTime.tv_sec == completeQueue[startIdx].closeTime.tv_sec) &&
            ((curTime.tv_usec + 3000) >= completeQueue[startIdx].closeTime.tv_usec))){
            
            close(completeQueue[startIdx].fd);
            startIdx = (++startIdx) % CLIENT_NUM;
            completeCnt --;
                
        } else {
            completeTimer.it_value.tv_sec = completeQueue[startIdx].closeTime.tv_sec 
                    - curTime.tv_sec;
            if(completeQueue[startIdx].closeTime.tv_usec > curTime.tv_usec){
                completeTimer.it_value.tv_usec = completeQueue[startIdx].closeTime.tv_usec 
                    - curTime.tv_usec;
            } else {
                completeTimer.it_value.tv_usec = 1000 * 1000 - curTime.tv_usec
                    + completeQueue[startIdx].closeTime.tv_usec;
                completeTimer.it_value.tv_sec --;
            }

//            completeTimer.it_value = completeQueue[startIdx].closeTime;
            setitimer(ITIMER_REAL, &completeTimer, NULL);
            return;
        }
    }

    if((aliveClientNum <= 0) && (completeCnt <= 0))
        exit(0);
}

int main(int argn, char **argv)
{
    int sockfd, val, i, nReady;
    struct sockaddr_in serverSock;
    char *buf;   
    ssize_t nRead;
    struct pollfd *client;
    
    buf = malloc(BUFSIZ);
    client = calloc(CLIENT_NUM, sizeof(struct pollfd));
    //server-sock
    memset(&serverSock, 0, sizeof(serverSock));
    serverSock.sin_family = AF_INET;
    inet_aton(SERVER_IP_ADDR, &serverSock.sin_addr);
    serverSock.sin_port = htons(SERVER_PORT);

    //sockfd - 1000
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    client[0].fd = sockfd;
    client[0].events = POLLOUT;
    val = fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK;
    fcntl(sockfd, F_SETFL, val);

    for(i == 1; i < CLIENT_NUM; i++){
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        client[i].fd = sockfd;
        client[i].events = POLLOUT;
        fcntl(sockfd, F_SETFL, val);
    }

    //signal
    struct sigaction act;
    act.sa_handler = timeoutProcess;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGALRM);
    act.sa_flags = 0;
    if(sigaction(SIGALRM, &act, NULL) < 0)
        printf("set SIGALRM error!\n");

    completeQueue = calloc(CLIENT_NUM, sizeof(CompleteTimer_t));
    memset(&completeTimer, 0, sizeof(completeTimer));
    //start!
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    //connect
    for(i = 0; i < CLIENT_NUM; i++)
        connect(client[i].fd, (struct sockaddr*)&serverSock, sizeof(serverSock));
    
    
    while(1){
        while(aliveClientNum <= 0){
            sleep(10); 
            printf("completeCnt = %d\n", completeCnt);
        }
            

        nReady = poll(client, CLIENT_NUM, -1);
        if(nReady <= 0)
            continue;

        for(i = 0; i < CLIENT_NUM; i++){
            if(client[i].fd == -1)
                continue;
            
            if(client[i].revents & POLLOUT){
                if(write(client[i].fd, "ping", 5)){
                    client[i].events = POLLIN;
                    client[i].revents = 0;
                } else {
                    printf("client[%i] connect error, closed!\n", i);
                    close(client[i].fd);
                    client[i].fd = -1;
                    aliveClientNum --;
                }
                
                if(--nReady <= 0)
                    break;
                
                continue;
            }

            if(client[i].revents & POLLIN){
                if((nRead = read(client[i].fd, buf, BUFSIZ)) > 0){
                    write(STDOUT_FILENO, buf, nRead);
                    if(strcmp(buf, "pong") == 0){
                        gettimeofday(&endTime, NULL);
                        int consumeTime = (int64_t)endTime.tv_sec * 1000 + endTime.tv_usec/1000
                                - (int64_t)startTime.tv_sec * 1000 - startTime.tv_usec/1000;
                        printf("  client[%d] consumeTime: %d ms\n", i, consumeTime);
                        
                        //
                        completeQueue[endIdx].fd = client[i].fd;
                        completeQueue[endIdx].closeTime = endTime;
                        completeQueue[endIdx].closeTime.tv_sec += DELAY_CLOSE_TIME;        
                        endIdx = (++endIdx) % CLIENT_NUM;
                        
                        
                        if(completeCnt == 0){
                            completeCnt ++;
                            completeTimer.it_value.tv_sec = DELAY_CLOSE_TIME;
                            completeTimer.it_value.tv_usec = 0;
                            setitimer(ITIMER_REAL, &completeTimer, NULL);
                        } else {
                            completeCnt ++;
                        }
                            
                        //close(client[i].fd);
                        client[i].fd = -1;
                        aliveClientNum --;
                    } else {
                        if(write(client[i].fd, "ping", 5) <= 0){
                            printf("client[%d] connect error! close!\n", i);
                            close(client[i].fd);
                            client[i].fd = -1;
                            aliveClientNum --;
                        }
                    }
                } else {
                    printf("client[%d] connect error! close!\n", i);
                    close(client[i].fd);
                    client[i].fd = -1;
                    aliveClientNum --;
                }

                if(--nReady <= 0)
                    break;
            }
        }
    }
}
