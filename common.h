#ifndef __COMMON_H__
#define __COMMON_H__

#include <semaphore.h>
#include <sys/msg.h>

#include "stack.h"

#define MaxStackSize 500

STACK paths;

// global args
char rootDirName[1024];    // name of root dir
char extensions[32][1024]; // extensions to filter
int extensionsCounter;

int queuePCchars; // kolejka dla Proc's->Counter (znaki)
int queuePClines; // kolejka dla Proc's->Counter (linijki)

typedef struct
{
    long type;
    char text[1024];
} MSG;

#define __END_MSG__ "|END"

sem_t *semPC;
sem_t *semQueuePC;

#endif // __COMMON_H__