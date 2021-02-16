#ifndef __COMMON_H__
#define __COMMON_H__

#include "stack.h"

#define MaxStackSize 500

// stacks for communication
STACK paths;   //from finder to procs
STACK results; // from procs to counter

// struct for storing information on stack results
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