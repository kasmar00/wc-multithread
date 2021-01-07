#ifndef __COMMON_H__
#define __COMMON_H__

#include <sys/msg.h>

int queueMF;      // kolejka dla main->finder
int queueFP;      // kolejka dla Finder->Proc's
int queuePCchars; // kolejka dla Proc's->Counter (znaki)
int queuePClines; // kolejka dla Proc's->Counter (linijki)

typedef struct
{
    long type;
    char text[1024];
} MSG;

#define __END_MSG__ "|END"

sem_t *semMF;
sem_t *semFP;
sem_t *semPC;

#endif // __COMMON_H__