#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>

int64_t p = 0;
int64_t q = 0;
int64_t r = 0;
int64_t s = 0;
pthread_mutex_t mutexp = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexq = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexr = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexs = PTHREAD_MUTEX_INITIALIZER;

static void* WorkThread(void *arg)
{
    int param = (int)arg;

    int64_t autoIncCntp = 0;
    int64_t autoIncCntq = 0;
    int64_t autoIncCntr = 0;
    int64_t autoIncCnts = 0;

    while (param & 0x0f) {
        if (param & 0x01) {
            if (pthread_mutex_trylock(&mutexp) == 0) {
                p ++;
                pthread_mutex_unlock(&mutexp);
                if (++autoIncCntp >= 1e9)
                    param &= ~0x01;
            }
        }
        if (param & 0x02) {
            if (pthread_mutex_trylock(&mutexq) == 0) {
                q ++;
                pthread_mutex_unlock(&mutexq);
                if (++autoIncCntq >= 1e9)
                    param &= ~0x02;
            }
        }
        if (param & 0x04) {
            if (pthread_mutex_trylock(&mutexr) == 0) {
                r ++;
                pthread_mutex_unlock(&mutexr);
                if (++autoIncCntr >= 1e9)
                    param &= ~0x04;
            }
        }
        if (param & 0x08) {
            if (pthread_mutex_trylock(&mutexs) == 0) {
                s ++;
                pthread_mutex_unlock(&mutexs);
                if (++autoIncCnts >= 1e9)
                   param &= ~0x08;
            }
        }
    }

    return NULL;
}


int main(int argn, char* argv[])
{
    pthread_t tidp1,tidp2,tidp3,tidp4;

    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    if (pthread_create(&tidp1, NULL, WorkThread, (int *)0x0f) == -1)
         printf("create thread 1 error!\n");

    if (pthread_create(&tidp2, NULL, WorkThread, (int *)0x0b) == -1)
         printf("create thread 2 error!\n");

    if (pthread_create(&tidp3, NULL, WorkThread, (int *)0x0d) == -1)
         printf("create thread 3 error!\n");

    if (pthread_create(&tidp4, NULL, WorkThread, (int *)0x0a) == -1)
         printf("create thread 4 error!\n");


    pthread_join(tidp1, NULL);
    pthread_join(tidp2, NULL);
    pthread_join(tidp3, NULL);
    pthread_join(tidp4, NULL);

    printf("p = %ld\n", p);
    printf("q = %ld\n", q);
    printf("r = %ld\n", r);
    printf("s = %ld\n", s);

    gettimeofday(&endTime, NULL);
    int64_t consumeTime = (int64_t)endTime.tv_sec * 1000 + endTime.tv_usec / 1000
                     - (int64_t)startTime.tv_sec * 1000 - startTime.tv_usec / 1000;

    printf("consume time = %ld ms\n", consumeTime);
}



