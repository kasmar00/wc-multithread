#include <stdio.h>
#include <sys/sysinfo.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"

#include "counter.h"

void *counter(void *i)
{
    // int queueFC = *((int *)i);
    int chars = 0;
    int lines = 0;
    printf("counter\n");

    int activeProcs = get_nprocs();

    while (1)
    {
        sem_wait(semQueuePC); // blokada przed przeplotem zapisu na kolejki do Counter
        char *strChars = stack_pop(&charsStack);
        char *strLines = stack_pop(&linesStack);
        sem_post(semQueuePC);
        printf("actv proc: %d; rcved %s %s\n", activeProcs, strChars, strLines);

        if (strcmp(__END_MSG__, strChars))
        {
            chars += atoi(strChars);
            lines += atoi(strLines);
        }
        else
            activeProcs--;
        if (activeProcs == 0)
            break;
    }
    printf("FINAL\n");
    printf("Liczba linii: %d\nLiczba znaków: %d\n", lines, chars);

    return 0;
}
