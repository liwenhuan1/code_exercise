#include <mqueue.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argn, char **argv)
{
    mqd_t mqdr, mqdw;
    struct mq_attr attr;
    char *msgBuffer;
    char sendMsg[] = "hello, my son!\n";
    char recvMsg[] = "hello, my father!\n";
    char errorMsg[] = "what?\n";

    mqdr = mq_open("/testMqRead", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR, NULL);
    mqdw = mq_open("/testMqWrite", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR, NULL);

    if((mqdr <= 0) || (mqdw <= 0))
        printf("mq_open error!\n");

    mq_getattr(mqdr, &attr);
    msgBuffer = malloc(attr.mq_msgsize);

    if(fork() == 0){
        while(1){
            if(mq_receive(mqdw, msgBuffer, attr.mq_msgsize, NULL)){
                if(strcmp(msgBuffer, sendMsg) == 0){
                    mq_send(mqdr, recvMsg, sizeof(recvMsg), 1);
                    printf("child success\n");
                    mq_close(mqdr);
                    mq_close(mqdw);
                    free(msgBuffer);
                    return 0;
                } else {
                    mq_send(mqdr, errorMsg, sizeof(errorMsg), 1);
                }
            }
        }
    } else {
        mq_send(mqdw, sendMsg, sizeof(sendMsg), 1);
        while(1){
            if(mq_receive(mqdr, msgBuffer, attr.mq_msgsize, NULL)){
                if(strcmp(msgBuffer, recvMsg) == 0){
                    printf("parent success\n");
                    mq_close(mqdr);
                    mq_close(mqdw);
                    mq_unlink("/testMqRead");
                    mq_unlink("/testMqWrite");
                    free(msgBuffer);
                    return 0;
                } else {
                    mq_send(mqdw, sendMsg, sizeof(sendMsg), 1);
                }
            } 
        }
    }
}

