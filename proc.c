
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <ctype.h>
#include <stdio.h>
#include <pthread.h>
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
    //send to counter
    MSG msgChar;
    MSG msgLine;
    msgChar.type = 1;
    msgLine.type = 1;
    sprintf(msgChar.text, "%d", chars);
    sprintf(msgLine.text, "%d", lines);
    sem_wait(semQueuePC); // blokada przed przeplotem zapisu na kolejki do Counter
    msgsnd(queuePCchars, &msgChar, strlen(msgChar.text) + 1, 0);
    msgsnd(queuePClines, &msgLine, strlen(msgLine.text) + 1, 0);
    sem_post(semQueuePC);
    sem_post(semPC); // informacja dla COunter o nowej wiadomości
}

void *proc(void *i)
{
    printf("procs\n");
    while (1)
    {
        char *tmp = stack_pop(&paths);
        if (strcmp(__END_MSG__, tmp))
            wc(tmp);
        else
            break;
    }

    printf("ending proc\n");
    MSG msg;
    msg.type = 1;
    strcpy(msg.text, __END_MSG__);
    sem_wait(semQueuePC); // blokada przed przeplotem zapisu na kolejki do Counter
    msgsnd(queuePCchars, &msg, strlen(msg.text) + 1, 0);
    msgsnd(queuePClines, &msg, strlen(msg.text) + 1, 0);
    sem_post(semQueuePC);
    sem_post(semPC);
    printf("end proc\n");
    return NULL;
}
