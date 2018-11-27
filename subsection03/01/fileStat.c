#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SIZE_1M      (1024 * 1024)

int fileStat(char *path, ssize_t *fileSumSize, int *fileSize1MNum)
{

    struct stat statBuf;
    if (lstat(path, &statBuf))
        return -1;

    if (S_ISDIR(statBuf.st_mode)) {
        DIR *currentDir = opendir(path);
        if (currentDir == NULL)
            return -1;

        *fileSumSize = 0;
        *fileSize1MNum = 0;

        char pathBuff[4096];
        strcpy(pathBuff, path);
        char *ptr = pathBuff + strlen(path);
        if (ptr[-1] != '/')
            *ptr++ = '/';
        struct dirent *curDirent;
        while ((curDirent = readdir(currentDir)) != NULL) {
            if ((strcmp(curDirent->d_name, ".") == 0) || (strcmp(curDirent->d_name, "..") == 0))
                continue;
            *ptr = 0;
            strcpy(ptr, curDirent->d_name);

            ssize_t tmpFileSumSize = 0;
            int tmpFileSize1MNum = 0;        //not clear 0
            if (fileStat(pathBuff, &tmpFileSumSize, &tmpFileSize1MNum) == 0) {
                *fileSumSize = *fileSumSize + tmpFileSumSize;
                *fileSize1MNum = *fileSize1MNum + tmpFileSize1MNum;
            }
        }

        closedir(currentDir);
        return 0;
    }

    //normal file
    *fileSumSize = statBuf.st_size;
    if (statBuf.st_size > SIZE_1M)
        *fileSize1MNum = 1;

    return 0;
}





int main(int argn, char **argv)
{
    if (argn != 2) {
        printf("parameter error!\n");
        return 0;
    }

    ssize_t fileSumSize;
    int fileSize1MNum;
    if (fileStat(argv[1], &fileSumSize, &fileSize1MNum) == 0) {
        printf("the all file size: %ld\n", fileSumSize);
        printf("the file size greater than 1M num: %d\n", fileSize1MNum);
    } else {
        printf("fileStat() run error!\n");
    }

    return 0;
}



