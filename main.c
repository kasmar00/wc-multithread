#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <fcntl.h>

#include <pthread.h>
#include <sys/sysinfo.h>
#include <stdlib.h>
#include <dirent.h>

#include <sys/stat.h>
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
    queuePCchars = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR | IPC_CREAT); // kolejka dla Proc's->Counter (znaki)
    queuePClines = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR | IPC_CREAT); // kolejka dla Proc's->Counter (linijki)

    semPC = sem_open("/semPC", O_CREAT, 0600, 1); // semafor obrazujący liczbe wiadomości w kolejece FP
    semQueuePC = sem_open("/semQueuePC", O_CREAT, 0600, 1);

    // initalise stack variables
    stack_init(&paths);
    extensionsCounter = 0;

    pthread_t threadIdFinder;
    strcpy(rootDirName, argv[1]);
    for (int i = 2; i < argc; i++)
    {
        strcpy(extensions[extensionsCounter], argv[i]);
        extensionsCounter++;
    }

    pthread_create(&threadIdFinder, NULL, finder, NULL);

    sleep(10);
    pthread_t threadIdProc[get_nprocs()];
    for (int i = 0; i < get_nprocs(); i++)
    {
        pthread_create(&threadIdProc[i], NULL, proc, NULL);
    }
    sleep(10);
    pthread_t threadIdCounter;
    pthread_create(&threadIdCounter, NULL, counter, NULL);
    sleep(10);

    printf("sprzątam\n");
    // sprzątanie
    pthread_join(threadIdFinder, NULL);
    for (int i = 0; i < get_nprocs(); i++)
        pthread_join(threadIdProc[i], NULL);
    pthread_join(threadIdCounter, NULL);
    msgctl(queuePCchars, IPC_RMID, NULL);
    msgctl(queuePClines, IPC_RMID, NULL);

    sem_close(semPC);
    sem_unlink("/semPC");
    sem_close(semQueuePC);
    sem_unlink("/semQueuePC");
    return 0;
}
