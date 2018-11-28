#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <sys/types.h>
#include <sys/time.h>

#define INCREASE_CNT      1e7      //1e7
#define INC_P             0x01
#define INC_Q             0x02
#define INC_R             0x04
#define INC_S             0x08
#define INC_ALL           (INC_P | INC_Q | INC_R | INC_S)

int64_t p = 0;
int64_t q = 0;
int64_t r = 0;
int64_t s = 0;

std::atomic<int> mutexp;
std::atomic<int> mutexq;
std::atomic<int> mutexr;
std::atomic<int> mutexs;

void ThreadWork(int param)
{
    int64_t autoIncCntp = 0;
    int64_t autoIncCntq = 0;
    int64_t autoIncCntr = 0;
    int64_t autoIncCnts = 0;

    while (param & INC_ALL) {
        if (param & INC_P) {
            if (mutexp.fetch_add(1) != 0)
                mutexp --;
            else {
                p ++;
                mutexp --;
                if (++autoIncCntp >= INCREASE_CNT)
                    param &= ~INC_P;
            }
        }
        if (param & INC_Q) {
            if (mutexq.fetch_add(1) != 0)
                mutexq --;
            else {
                q ++;
                mutexq --;
                if (++autoIncCntq >= INCREASE_CNT)
                    param &= ~INC_Q;
            }
        }
        if (param & INC_R) {
            if (mutexr.fetch_add(1) != 0)
                mutexr --;
            else {
                r ++;
                mutexr --;
                if (++autoIncCntr >= INCREASE_CNT)
                    param &= ~INC_R;
            }
        }
        if (param & INC_S) {
            if (mutexs.fetch_add(1) != 0)
                mutexs --;
            else {
                s ++;
                mutexs --;
                if (++autoIncCnts >= INCREASE_CNT)
                    param &= ~INC_S;
            }
        }
    }
}

int main()
{
    int param[4];
    param[0] = (INC_P | INC_Q | INC_R | INC_S);
    param[1] = (INC_P | INC_Q | INC_S);
    param[2] = (INC_P | INC_R | INC_S);
    param[3] = (INC_Q | INC_S);

    mutexp = 0;
    mutexq = 0;
    mutexr = 0;
    mutexs = 0;

    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    std::vector<std::thread> threads;
    for (int i = 0; i < 4; i++)
        threads.push_back(std::thread{ThreadWork, param[i]});

    for (auto& tmp: threads)
        tmp.join();

    gettimeofday(&endTime, NULL);
    int64_t consumeTime = (int64_t)endTime.tv_sec * 1000 + endTime.tv_usec / 1000
                     - (int64_t)startTime.tv_sec * 1000 - startTime.tv_usec / 1000;

    std::cout << "p = " << p << std::endl;
    std::cout << "q = " << q << std::endl;
    std::cout << "r = " << r << std::endl;
    std::cout << "s = " << s << std::endl;
    std::cout << "consume time = " << consumeTime << "ms" << std::endl;
    return 0;
}

/*
void ThreadWork(int param)
{
    for (int i = 0; i < INCREASE_CNT; i++) {
        if (param & INC_P) {
            while (mutexp.fetch_add(1) != 0) {
                mutexp --;
            }
            p ++;
            mutexp --;
        }

        if (param & INC_Q) {
            while (mutexq.fetch_add(1) != 0) {
                mutexq --;
            }
            q ++;
            mutexq --;
        }

        if (param & INC_R) {
            while (mutexr.fetch_add(1) != 0) {
                mutexr --;
            }
            r ++;
            mutexr --;
        }

        if (param & INC_S) {
            while (mutexs.fetch_add(1) != 0) {
                mutexs --;
            }
            s ++;
            mutexs --;
        }
    }
}
*/
