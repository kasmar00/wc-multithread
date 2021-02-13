#ifndef __COMMON_H__
#define __COMMON_H__

#include <semaphore.h>

#include "stack.h"

#define MaxStackSize 500

STACK paths;
STACK charsStack;
STACK linesStack;

// global args
char rootDirName[1024];    // name of root dir
char extensions[32][1024]; // extensions to filter
int extensionsCounter;

sem_t *semQueuePC;

#endif // __COMMON_H__