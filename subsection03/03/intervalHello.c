#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static void SignalProcess(int signo)
{
    if((signo == SIGQUIT) || (signo == SIGTERM)){
        printf("Bye\n");
        exit(0);
    }
}


int main(int argn, char** argv)
{
    struct sigaction act;
    
    act.sa_handler = SignalProcess;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGQUIT);  //&act.sa_mask  !&act
    sigaddset(&act.sa_mask, SIGTERM);
    act.sa_flags = 0;

    if(sigaction(SIGQUIT, &act, NULL) < 0)
        printf("set signal hander error!\n");

    if(sigaction(SIGTERM, &act, NULL) < 0)
        printf("set signal hander error!\n");
    
    while(1){
        printf("hello\n");
        sleep(1);
    }
}
