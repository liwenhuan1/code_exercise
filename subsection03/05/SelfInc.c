#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>

#define INCREASE_CNT      1e7
#define INC_P             0x01
#define INC_Q             0x02
#define INC_R             0x04
#define INC_S             0x08
#define INC_ALL           (INC_P | INC_Q | INC_R | INC_S)

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
    int param = *(int *)arg;

    int64_t autoIncCntp = 0;
    int64_t autoIncCntq = 0;
    int64_t autoIncCntr = 0;
    int64_t autoIncCnts = 0;

    while (param & INC_ALL) {
        if (param & INC_P) {
            if (pthread_mutex_trylock(&mutexp) == 0) {
                p ++;
                pthread_mutex_unlock(&mutexp);
                if (++autoIncCntp >= INCREASE_CNT)
                    param &= ~INC_P;
            }
        }
        if (param & INC_Q) {
            if (pthread_mutex_trylock(&mutexq) == 0) {
                q ++;
                pthread_mutex_unlock(&mutexq);
                if (++autoIncCntq >= INCREASE_CNT)
                    param &= ~INC_Q;
            }
        }
        if (param & INC_R) {
            if (pthread_mutex_trylock(&mutexr) == 0) {
                r ++;
                pthread_mutex_unlock(&mutexr);
                if (++autoIncCntr >= INCREASE_CNT)
                    param &= ~INC_R;
            }
        }
        if (param & INC_S) {
            if (pthread_mutex_trylock(&mutexs) == 0) {
                s ++;
                pthread_mutex_unlock(&mutexs);
                if (++autoIncCnts >= INCREASE_CNT)
                   param &= ~INC_S;
            }
        }
    }

    return NULL;
}


int main(int argn, char* argv[])
{
    pthread_t tidp[4];
    int i;

    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    int param[4];
    param[0] = (INC_P | INC_Q | INC_R | INC_S);
    param[1] = (INC_P | INC_Q | INC_S);
    param[2] = (INC_P | INC_R | INC_S);
    param[3] = (INC_Q | INC_S);

    for (i = 0; i < 4; i++) {
        if(pthread_create(&tidp[i], NULL, WorkThread, &param[i]) == -1)
            printf("creat thread %d error!", i);
    }

    for (i = 0; i < 4; i++)
        pthread_join(tidp[i], NULL);

    printf("p = %ld\n", p);
    printf("q = %ld\n", q);
    printf("r = %ld\n", r);
    printf("s = %ld\n", s);

    gettimeofday(&endTime, NULL);
    int64_t consumeTime = (int64_t)endTime.tv_sec * 1000 + endTime.tv_usec / 1000
                     - (int64_t)startTime.tv_sec * 1000 - startTime.tv_usec / 1000;

    printf("consume time = %ld ms\n", consumeTime);
}



