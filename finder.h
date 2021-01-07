#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>

#include <pthread.h>
#include <sys/sysinfo.h>
#include <stdlib.h>
#include <dirent.h>

#include "common.h"

void dirRead(int queue, char dirName[])
{
    printf("szukanie w %s\n", dirName);
    DIR *rootDir;
    rootDir = opendir(dirName);

    struct dirent *dirEnt;

    while ((dirEnt = readdir(rootDir)) != NULL)
    {
        switch (dirEnt->d_type)
        {
        case DT_DIR:
            printf("Dir {");
            if (strcmp(dirEnt->d_name, "..") != 0 && strcmp(dirEnt->d_name, ".") != 0)
            {
                char buf[1024];
                snprintf(buf, sizeof buf, "%s/%s", dirName, dirEnt->d_name);
                dirRead(queue, buf);
            }
            printf("} ");
            break;
        case DT_REG:
            printf("Reg");
            break;

        default:
            printf("Other/Unkown");
            break;
        }
        printf(" %s\n", dirEnt->d_name);
        char buf[1024];
        snprintf(buf, sizeof buf, "%s/%s", dirName, dirEnt->d_name);
        MSG msg;
        msg.type = 1;
        strcpy(msg.text, buf);
        msgsnd(queue, &msg, strlen(msg.text) + 1, 0);
        sem_post(semFP);
    }
}

void *finder(void *info)
{
    // sleep(1);
    //TODO: check only files with extensions
    printf("thread\n");
    sem_wait(semMF);
    MSG msg;
    msgrcv(queueMF, &msg, 1024, 0, 0);
    char dirName[1024];
    strcpy(dirName, msg.text);

    char ext[32][1024];
    int i = 0;
    while (msgrcv(queueMF, &msg, 1024, 0, IPC_NOWAIT) != -1)
    {
        strcpy(ext[i], msg.text);
        i++;
    }

    printf("dir: %s\n", dirName);
    for (int j = 0; j < i; j++)
        printf("args: %s\n", ext[j]);
    dirRead(queueFP, dirName);
    for (int i = 0; i < get_nprocs(); i++)
    {
        strcpy(msg.text, __END_MSG__);
        msgsnd(queueFP, &msg, strlen(msg.text) + 1, 0);
        sem_post(semFP);
    }

    return NULL;
}