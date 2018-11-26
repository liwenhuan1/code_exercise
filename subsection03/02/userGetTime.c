#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

int main(int argn, char **argv)
{
    struct timeval tv;
    char buffer [128];

    gettimeofday(&tv, NULL);
    struct tm* local_time = localtime(&tv.tv_sec);

    strftime(buffer, sizeof(buffer), "localtime: %Y-%m-%d %H:%M:%S", local_time);
    
    char *ptr = buffer + strlen(buffer);
    sprintf(ptr, ".%03d", (int)tv.tv_usec/1000);
    printf("%s\n", buffer);
    /*
    printf("localtime:%d-%02d-%02d %02d:%02d:%02d.%04d\n", 
            (local_time->tm_year + 1900), local_time->tm_mon, 
            local_time->tm_mday, local_time->tm_hour,
            local_time->tm_min, local_time->tm_sec, (int)(tv.tv_usec/1000));
    */
    printf("unixtime:%d\n", (int)tv.tv_sec);
}
