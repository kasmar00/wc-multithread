#include "common.h"

void wc(char *file)
{
    //TODO: change to mmap
    int d = open(file, O_RDONLY);
    char buf[1];
    int n, lines = 0, chars = 0;
    while ((n = read(d, buf, 1)) > 0)
    {
        if (buf[0] == '\n')
        {
            lines++;
        }
        chars++;
    }
    printf("plik: %s znakow %d, linijek %d\n", file, chars, lines);
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
    // int queueFC = *((int *)i);
    // sleep(2);
    printf("procs\n");
    MSG msga;
    while (1)
    {
        sem_wait(semFP);
        msgrcv(queueFP, &msga, 1024, 0, 0);
        if (strcmp(__END_MSG__, msga.text))
            wc(msga.text);
        else
            break;
    }

    MSG msg;
    msg.type = 1;
    strcpy(msg.text, __END_MSG__);
    sem_wait(semQueuePC); // blokada przed przeplotem zapisu na kolejki do Counter
    msgsnd(queuePCchars, &msg, strlen(msg.text) + 1, 0);
    msgsnd(queuePClines, &msg, strlen(msg.text) + 1, 0);
    sem_post(semQueuePC);
    sem_post(semPC);
    printf("end proc\n");
    // while (1)
    // ;
    return NULL;
}
