#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "common.h"

#include "proc.h"

void wc(char *file)
{
    int lines = 0, chars = 0;
    struct stat *buffer = malloc(sizeof(struct stat));
    stat(file, buffer);
    printf("%ld\n", buffer->st_size);
    int d = open(file, O_RDONLY);
    char *ptr = mmap(NULL, buffer->st_size, PROT_READ, MAP_SHARED, d, 0);
    for (int i = 0; i < buffer->st_size; i++)
    {
        // printf("%c", (char)*(ptr + i));
        if ((char)*(ptr + i) == '\n')
            lines++;
        if (!isspace((char)*(ptr + i)))
            chars++;
    }
    printf("procs: %ld, plik: %s znakow %d, linijek %d\n", pthread_self(), file, chars, lines);
    close(d);
    char strChars[1024];
    char strLines[1024];
    sprintf(strChars, "%d", chars);
    sprintf(strLines, "%d", lines);
    char *a = strdup(strChars);
    char *b = strdup(strLines);
    // sem_wait(semQueuePC); // blokada przed przeplotem zapisu na kolejki do Counter
    stack_push(&charsStack, a);
    stack_push(&linesStack, b);
    // sem_post(semQueuePC);
}

void *proc(void *i)
{
    printf("procs\n");
    while (1)
    {
        char *tmp = stack_pop(&paths);
        if (tmp != NULL)
            wc(tmp);
        else
            break;
    }

    printf("ending proc\n");
    stack_endData(&charsStack);
    stack_endData(&linesStack);
    printf("end proc\n");
    return NULL;
}
