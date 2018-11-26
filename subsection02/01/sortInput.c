#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_LINE         2000


char lineBuff[BUFSIZ];

int compare(const void *a, const void *b)
{
    return -strcmp((char *)a, (char *)b); 
}

int main(int argn, char **argv)
{
    FILE *file = fopen("input.txt", "r");
    if(file == NULL){
        printf("open file fail!\n");
        return -1;
    }

    int curFileLineMax = INIT_LINE;
    int realLine = 0;
    char **fileBuff = calloc(curFileLineMax, sizeof(char *));

    while(fgets(lineBuff, BUFSIZ, file) == lineBuff){
        if(realLine >= curFileLineMax){
            curFileLineMax = realLine * 2;
            fileBuff = realloc(fileBuff, sizeof(char *) * curFileLineMax);
        }
        
        fileBuff[realLine] = malloc(strlen(lineBuff));
        strcpy(fileBuff[realLine], lineBuff);
        realLine ++;
    }

    qsort(fileBuff, realLine, sizeof(char *), compare);
    for(int i = 0; i < realLine; i++)
        fputs(fileBuff[i], stdout);

    /*
    while(fgets(lineBuff, BUFSIZ, file) == lineBuff){
        qsort(lineBuff, strlen(lineBuff), 1, compare);
        fputs(lineBuff, stdout);
    } 
    */
    return 0;
}
