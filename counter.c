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
    MSG msgChars;
    MSG msgLines;

    int activeProcs = get_nprocs();

    while (1)
    {
        sem_wait(semPC);
        msgrcv(queuePCchars, &msgChars, 1023, 0, 0);
        msgrcv(queuePClines, &msgLines, 1023, 0, 0);
        printf("actv proc: %d; rcved %s %s\n", activeProcs, msgChars.text, msgLines.text);

        if (strcmp(__END_MSG__, msgChars.text))
        {
            chars += atoi(msgChars.text);
            lines += atoi(msgLines.text);
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
