#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static void SignalProcess(int signo)
{
    if ((signo == SIGQUIT) || (signo == SIGTERM)) {
        printf("Bye\n");
        exit(0);
    } else if (signo == SIGALRM) {
        alarm(1);
        printf("hello\n");
    }
}



int main(int argn, char** argv)
{
    struct sigaction act;

    act.sa_handler = SignalProcess;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGQUIT);
    sigaddset(&act.sa_mask, SIGTERM);
    sigaddset(&act.sa_mask, SIGALRM);
    act.sa_flags = 0;

    if (sigaction(SIGQUIT, &act, NULL) < 0)
        printf("set SIGQUIT error!\n");

    if (sigaction(SIGTERM, &act, NULL) < 0)
        printf("set SIGTERM error!\n");

    if (sigaction(SIGALRM, &act, NULL) < 0)
        printf("set SIGALRM error!\n");

    alarm(1);

    while(1){
        pause();
    }
}
