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
    char sendMsg[] = "hello, my son!\n";
    char recvMsg[] = "hello, my father!\n";
    char errorMsg[] = "what?\n";

    mqd_t mqdr = mq_open("/testMqueue", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR, NULL);
    if (mqdr <= 0) {
        printf("mq_open error!\n");
        return 0;
    }

    struct mq_attr attr;
    mq_getattr(mqdr, &attr);

    char *msgBuffer = malloc(attr.mq_msgsize);
    if (msgBuffer == NULL) {
        printf("msgBuffer malloc error!\n");
        mq_close(mqdr);
        mq_unlink("/testMqueue");
        return 0;
    }

    pid_t pid = fork();
    if (pid == 0) {
        while(1) {
            if (mq_receive(mqdr, msgBuffer, attr.mq_msgsize, NULL)) {
                if (strcmp(msgBuffer, sendMsg) == 0) {
                    mq_send(mqdr, recvMsg, sizeof(recvMsg), 1);
                    printf("child success\n");
                    mq_close(mqdr);
                    free(msgBuffer);
                    return 0;
                } else {
                    mq_send(mqdr, errorMsg, sizeof(errorMsg), 1);
                }
            }
        }
    } else if (pid > 0) {
        mq_send(mqdr, sendMsg, sizeof(sendMsg), 1);
        while(1){
            if(mq_receive(mqdr, msgBuffer, attr.mq_msgsize, NULL)){
                if(strcmp(msgBuffer, recvMsg) == 0){
                    printf("parent success\n");
                    mq_close(mqdr);
                    mq_unlink("/testMqueue");
                    free(msgBuffer);
                    return 0;
                } else {
                    mq_send(mqdr, sendMsg, sizeof(sendMsg), 1);
                }
            }
        }
    } else {
        printf("fork error!\n");
    }

    free(msgBuffer);
    mq_close(mqdr);
    mq_unlink("/testMqueue");
    return 0;
}

