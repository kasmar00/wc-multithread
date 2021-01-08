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
    // strcpy(msgChar.text, "23");
    sprintf(msgChar.text, "%d", chars);
    msgsnd(queuePCchars, &msgChar, strlen(msgChar.text) + 1, 0);
    // strcpy(msgLine.text, "45");
    sprintf(msgLine.text, "%d", lines);
    msgsnd(queuePClines, &msgLine, strlen(msgLine.text) + 1, 0);
    sem_post(semPC);
}

void *proc(void *i)
{
    // int queueFC = *((int *)i);
    // sleep(2);
    printf("procs\n");
    MSG msga;
    sem_wait(semFP);
    printf("procs after sem\n");
    msgrcv(queueFP, &msga, 1024, 0, 0);
    printf("procs after msg\n");
    while (strcmp(__END_MSG__, msga.text))
    {
        // printf("%s\n", msga.text);
        wc(msga.text);
        sem_wait(semFP);
        msgrcv(queueFP, &msga, 1024, 0, 0);
    }
    MSG msg;
    msg.type = 1;
    strcpy(msg.text, __END_MSG__);
    msgsnd(queuePCchars, &msg, strlen(msg.text) + 1, 0);
    msgsnd(queuePClines, &msg, strlen(msg.text) + 1, 0);
    sem_post(semPC);
    printf("end proc\n");
    // while (1)
    // ;
    return NULL;
}
