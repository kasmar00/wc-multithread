#ifndef __STACK_H__
#define __STACK_H__

#include <pthread.h>

#define MaxStackSize 500

typedef struct
{
    void *elements[MaxStackSize];
    int pointer;
    int activePushers;                  // counts number of pushing threads, 0 means no more values will be pushed
    pthread_mutex_t mutex;              // mutual exclusion at acces to stack
    pthread_cond_t underCond, overCond; // protection ageainst under and over flow of paths
} STACK;

void stack_push(STACK *stack, void *element);
void *stack_pop(STACK *stack);

void stack_read(STACK *stack);

void stack_init(STACK *stack, int pushers);
void stack_endData(STACK *stack);

#endif // __STACK_H__