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

        char pathBuff[4096];
        struct dirent *curDirent;
        while ((curDirent = readdir(currentDir)) != NULL) {
            if ((strcmp(curDirent->d_name, ".") == 0) || (strcmp(curDirent->d_name, "..") == 0))
                continue;
            strcpy(pathBuff, path);
            strcat(pathBuff, "/");
            strcat(pathBuff, curDirent->d_name);
            fileStat(pathBuff, fileSumSize, fileSize1MNum);
        }

        closedir(currentDir);
        return 0;
    }

    //normal file
    *fileSumSize += statBuf.st_size;
    if (statBuf.st_size > SIZE_1M)
        *fileSize1MNum += 1;

    return 0;
}





int main(int argn, char **argv)
{
    if (argn != 2) {
        printf("input parameter error!\n");
        return 0;
    }

    ssize_t fileSumSize = 0;
    int fileSize1MNum = 0;
    fileStat(argv[1], &fileSumSize, &fileSize1MNum);

    printf("the all file size: %ld\n", fileSumSize);
    printf("the file size greater than 1M num: %d\n", fileSize1MNum);
    return 0;
}



