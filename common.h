#ifndef __COMMON_H__
#define __COMMON_H__

#include "stack.h"

#define MaxStackSize 500

STACK paths;
STACK results;

typedef struct
{
    int lines;
    int chars;
} TWOINTS;

// global args
char rootDirName[1024];    // name of root dir
char extensions[32][1024]; // extensions to filter
int extensionsCounter;

#endif // __COMMON_H__