#include <stdio.h>

#include "stack.h"

void stack_push(STACK *stack, void *element)
{
    pthread_mutex_lock(&stack->mutex);
    // printf("entered push\n");

    while (stack->pointer == MaxStackSize - 1) // cant push on full stack
        pthread_cond_wait(&stack->overCond, &stack->mutex);
    stack->pointer++;
    stack->elements[stack->pointer] = element;
    // printf("Pushed: %s \n", (char *)stack->elements[stack->pointer]);
    // printf("%d\n", stack->pointer);
    pthread_mutex_unlock(&stack->mutex);

    // printf("exit push\n");
    pthread_cond_signal(&stack->underCond);
}

void *stack_pop(STACK *stack)
{
    pthread_mutex_lock(&stack->mutex);

    // printf("entered pop\n");
    while (stack->pointer < 0 && stack->activePushers > 0) // cant pop from empty stack
    {
        // printf("Waiting on underCond\n");
        pthread_cond_wait(&stack->underCond, &stack->mutex);
    }
    if (stack->pointer < 0 && stack->activePushers < 1) // stack empty an not gonna get more elements
    {
        // printf("no more elements on stack\n");
        pthread_mutex_unlock(&stack->mutex);
        return NULL;
    }

    void *tmp = stack->elements[stack->pointer];
    stack->pointer--;

    pthread_mutex_unlock(&stack->mutex);

    pthread_cond_signal(&stack->overCond);
    // printf("exit pop\n");
    return tmp;
}

void stack_read(STACK *stack)
{
    printf("Printing stack elements\n");
    printf("%d\n", stack->pointer);
    for (int i = 0; i <= stack->pointer; i++)
    {
        printf("%s \n", (char *)stack->elements[i]);
    }
}

void stack_init(STACK *stack, int pushers)
{
    stack->pointer = -1;
    stack->activePushers = pushers;
    pthread_mutex_init(&stack->mutex, NULL);
    pthread_cond_init(&stack->overCond, NULL);
    pthread_cond_init(&stack->underCond, NULL);
}

void stack_endData(STACK *stack)
{
    pthread_mutex_lock(&stack->mutex);
    stack->activePushers = -1;
    pthread_cond_broadcast(&stack->underCond);
    pthread_mutex_unlock(&stack->mutex);
}
