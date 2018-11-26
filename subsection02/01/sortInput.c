#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char lineBuff[BUFSIZ];

int compare(const void *a, const void *b)
{
    if(*(unsigned char *)a > *(unsigned char *)b)
        return -1;
    else if(*(unsigned char *)a == *(unsigned char *)b)
        return 0;
    else 
        return 1;
}

int main(int argn, char **argv)
{
    FILE *file = fopen("input.txt", "r");
    if(file == NULL){
        printf("open file fail!\n");
        return -1;
    }
    while(fgets(lineBuff, BUFSIZ, file) == lineBuff){
        qsort(lineBuff, strlen(lineBuff), 1, compare);
        fputs(lineBuff, stdout);
    } 
    return 0;
}
