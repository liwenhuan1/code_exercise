#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>

int64_t p = 0;
int64_t q = 0;
int64_t r = 0;
int64_t s = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* ThreadWork1(void *arg)
{
    while(pthread_mutex_lock(&mutex) != 0);
    for(long int i = 0; i < 1e9; i++){
        p ++;
        q ++;
        r ++;
        s ++;
    }
    pthread_mutex_unlock(&mutex);
}

void* ThreadWork2(void *arg)
{
    while(pthread_mutex_lock(&mutex) != 0);
    for(long int i = 0; i < 1e9; i++){
        p ++;
        q ++;
        s ++;
    }
    pthread_mutex_unlock(&mutex);
}

void* ThreadWork3(void *arg)
{
    while(pthread_mutex_lock(&mutex) != 0);
    for(long int i = 0; i < 1e9; i++){
        p ++;
        r ++;
        s ++;
    }
    pthread_mutex_unlock(&mutex);
}

void* ThreadWork4(void *arg)
{
    while(pthread_mutex_lock(&mutex) != 0);
    for(long int i = 0; i < 1e9; i++){
        q ++;
        s ++;
    }
    pthread_mutex_unlock(&mutex);
}

int main(int argn, char* argv[])
{
    pthread_t tidp1,tidp2,tidp3,tidp4;
    
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    if (pthread_create(&tidp1, NULL, ThreadWork1, NULL) == -1){
         printf("create thread 1 error!\n");
    }
    
    if (pthread_create(&tidp2, NULL, ThreadWork2, NULL) == -1){
         printf("create thread 2 error!\n");
    }

    if (pthread_create(&tidp3, NULL, ThreadWork3, NULL) == -1){
         printf("create thread 3 error!\n");
    }
    
    if (pthread_create(&tidp4, NULL, ThreadWork4, NULL) == -1){
         printf("create thread 4 error!\n");
    }

    pthread_join(tidp1, NULL);
    pthread_join(tidp2, NULL);
    pthread_join(tidp3, NULL);
    pthread_join(tidp4, NULL);

    printf("p = %llu\n", p);
    printf("q = %llu\n", q);
    printf("r = %llu\n", r);
    printf("s = %llu\n", s);

    gettimeofday(&endTime, NULL);
    int64_t consumeTime = (int64_t)endTime.tv_sec * 1000 + endTime.tv_usec / 1000
                     - (int64_t)startTime.tv_sec * 1000 - startTime.tv_usec / 1000;

    printf("consume time = %d ms\n", consumeTime);
}


 /*
pthread_mutex_t mutex_q = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_r = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_s = PTHREAD_MUTEX_INITIALIZER;
*/
/*
static void * thread_work(void *arg)       
{  
    int param = (int)arg;
    while(param & 0x0f){ 
        if(param & 0x01){
            if(pthread_mutex_trylock(&mutex_p) == 0){
                for(uint64_t i = 0; i < 1e9; i++)
                    p ++;
                param &= ~0x01;
                pthread_mutex_unlock(&mutex_p);
            }   
        }
        if(param & 0x02){
            if(pthread_mutex_trylock(&mutex_q) == 0){
                for(uint64_t i = 0; i < 1e9; i++)
                    q ++;
                param &= ~0x02;
                pthread_mutex_unlock(&mutex_q);
            }   
        }
        if(param & 0x04){
            if(pthread_mutex_trylock(&mutex_r) == 0){
                for(uint64_t i = 0; i < 1e9; i++)
                    r ++;
                param &= ~0x04;
                pthread_mutex_unlock(&mutex_r);
            }   
        }
        if(param & 0x08){
            if(pthread_mutex_trylock(&mutex_s) == 0){
                for(uint64_t i = 0; i < 1e9; i++)
                    s ++;
                param &= ~0x08;
                pthread_mutex_unlock(&mutex_s);
            }    
        }
    }
}
*/
