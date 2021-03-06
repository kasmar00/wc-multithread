#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include <pthread.h>
#include <sys/sysinfo.h>
#include <semaphore.h>

#include "common.h"

#include "finder.h"
#include "proc.h"
#include "counter.h"

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        perror("Za mało argumentów");
        return -1;
    }

    // initalise stack variables
    stack_init(&paths, 1);
    stack_init(&results, get_nprocs());
    extensionsCounter = 0;

    pthread_t threadIdFinder;
    strcpy(rootDirName, argv[1]);
    for (int i = 2; i < argc; i++)
    {
        strcpy(extensions[extensionsCounter], argv[i]);
        extensionsCounter++;
    }

    pthread_create(&threadIdFinder, NULL, finder, NULL);

    // sleep(10);
    pthread_t threadIdProc[get_nprocs()];
    for (int i = 0; i < get_nprocs(); i++)
    {
        pthread_create(&threadIdProc[i], NULL, proc, NULL);
    }
    // sleep(10);
    pthread_t threadIdCounter;
    pthread_create(&threadIdCounter, NULL, counter, NULL);
    // sleep(10);

    // printf("sprzątam\n");
    // sprzątanie
    pthread_join(threadIdFinder, NULL);
    for (int i = 0; i < get_nprocs(); i++)
        pthread_join(threadIdProc[i], NULL);
    pthread_join(threadIdCounter, NULL);

    return 0;
}
