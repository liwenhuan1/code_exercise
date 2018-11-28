#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

int main(int argn, char **argv)
{
    struct timeval tv;
    char buffer[128];

    gettimeofday(&tv, NULL);
    struct tm* localTime = localtime(&tv.tv_sec);
    strftime(buffer, sizeof(buffer), "localtime: %Y-%m-%d %H:%M:%S", localTime);

    printf("%s.%03ld\n", buffer, tv.tv_usec/1000);
    printf("unixtime:%ld\n", tv.tv_sec);
    return 0;
}
