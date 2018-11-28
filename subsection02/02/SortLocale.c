#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define INIT_LINE         2000


int compare(const void *a, const void *b)
{
    return -strcoll(*(char **)a, *(char **)b);
}

/*
void FileLineSortLocale(char **buffer, int n)
{
    int i, j;
    for (i = 0; i < (n - 1); i ++) {
        for(j = i + 1; j < n; j++) {
            if(strcoll(buffer[i], buffer[j]) > 0) {
                char *tmp = buffer[i];
                buffer[i] = buffer[j];
                buffer[j] = tmp;
            }
        }
    }
}
*/

int main(int argn, char **argv)
{
    if (argn != 2) {
        printf("input param error!\n");
        return 0;
    }
    char *ptr = setlocale(LC_COLLATE, "zh_CN.utf8");
    if ((ptr == NULL) || (strcmp(ptr, "zh_CN.utf8") != 0))
        printf("setlocale zh_CN.utf8 error!\n");

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("open file fail!\n");
        return 0;
    }
    int curFileLineMax = INIT_LINE;
    int realLine = 0;
    char **fileBuff = calloc(curFileLineMax, sizeof(char *));
    if (fileBuff == NULL) {
        printf("malloc fileBuff error!\n");
        fclose(file);
        return 0;
    }

    char lineBuff[BUFSIZ];
    while (fgets(lineBuff, BUFSIZ, file) != NULL) {
        if (realLine >= curFileLineMax) {
            curFileLineMax = realLine * 2;
            char **ptr = realloc(fileBuff, sizeof(char *) * curFileLineMax);
            if (ptr == NULL) {
                printf("realloc fileBuff error!\n");
                goto THE_END;
            } else
                fileBuff = ptr;
        }

        fileBuff[realLine] = malloc(strlen(lineBuff) + 1);
        if (fileBuff[realLine] == NULL) {
            printf("malloc fileBuff[%d] error!\n", realLine);
            goto THE_END;
        }

        strcpy(fileBuff[realLine], lineBuff);
        realLine ++;
    }

//    FileLineSortLocale(fileBuff, realLine);
    qsort(fileBuff, realLine, sizeof(char *), compare);
    for (int i = 0; i < realLine; i++)
        printf("%s", fileBuff[i]);

THE_END:
    for (int i = 0; i < realLine; i++) {
        if (fileBuff[i])
            free(fileBuff[i]);
    }
    free(fileBuff);
    fclose(file);
    return 0;
}
