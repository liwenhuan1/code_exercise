#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SIZE_1M      (1024 * 1024)

static char pathBuff[4096];
static int fileSize1MCnt;

size_t fileStat(char *path)
{
    size_t sumSize = 0;
    struct stat statBuf;
    if(lstat(path, &statBuf))
        return 0;

    /*
    if(S_ISREG(statBuf.st_mode) || S_ISLNK(statBuf.st_mode)){
        if(statBuf.st_size > SIZE_1M){
            printf("%s\n", path);
        }
        return statBuf.st_size;
    }*/

    if(S_ISDIR(statBuf.st_mode)){
        DIR *currentDir = opendir(path);
        if(currentDir == NULL)
            return 0;
    
        strcpy(pathBuff, path);
        char *relativePath = pathBuff + strlen(path);
        *relativePath ++ = '/';
        *relativePath = 0;

        struct dirent *curDirent;
        while((curDirent = readdir(currentDir)) != NULL){
//            printf(cur_dirent->d_name);
//            printf("\n");
            if((strcmp(curDirent->d_name, ".") == 0) || 
                (strcmp(curDirent->d_name, "..") == 0))
                continue;

            strcpy(relativePath, curDirent->d_name);
            sumSize += fileStat(pathBuff);
        }

        closedir(currentDir);
        return sumSize;
    }

    if(statBuf.st_size > SIZE_1M)
        fileSize1MCnt ++;
//        printf("%s\n", path);
    return statBuf.st_size; 

//    return 0;
}



int main(int argn, char **argv)
{
    if(argn != 2)
        printf("parameter error!\n");
    
    fileSize1MCnt = 0;
    size_t sumSize = fileStat(argv[1]);
    printf("the all file size: %ld\n", sumSize);
    printf("the file size greater than 1M count: %d\n", fileSize1MCnt);
}
