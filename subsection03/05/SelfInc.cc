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

std::atomic<int64_t> p;
std::atomic<int64_t> q;
std::atomic<int64_t> r;
std::atomic<int64_t> s;

void ThreadWork(int param)
{
    uint64_t i;
    for (i = 0; i < INCREASE_CNT; i++) {
        if (param & INC_P)
            p ++;
        if (param & INC_Q)
            q ++;
        if (param & INC_R)
            r ++;
        if (param & INC_S)
            s ++;
    }
}

int main()
{
    int param[4];
    param[0] = (INC_P | INC_Q | INC_R | INC_S);
    param[1] = (INC_P | INC_Q | INC_S);
    param[2] = (INC_P | INC_R | INC_S);
    param[3] = (INC_Q | INC_S);

    p = 0;
    q = 0;
    r = 0;
    s = 0;

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
