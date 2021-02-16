#include <stdio.h>
#include <sys/sysinfo.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"

#include "counter.h"

void *counter(void *i)
{
    int chars = 0;
    int lines = 0;
    printf("counter\n");

    while (1)
    {
        TWOINTS *tmp = stack_pop(&results);

        if (tmp != NULL)
        {
            printf("actv proc: %d; rcved %d %d\n", results.activePushers, tmp->chars, tmp->lines);
            chars += tmp->chars;
            lines += tmp->lines;
            free(tmp);
        }
        else
            break;
    }
    printf("FINAL\n");
    printf("Liczba linii: %d\nLiczba znaków: %d\n", lines, chars);

    return 0;
}
