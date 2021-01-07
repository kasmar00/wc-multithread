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
    queueMF = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR | IPC_CREAT);      // kolejka dla main->finder
    queueFP = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR | IPC_CREAT);      // kolejka dla Finder->Proc's
    queuePCchars = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR | IPC_CREAT); // kolejka dla Proc's->Counter (znaki)
    queuePClines = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR | IPC_CREAT); // kolejka dla Proc's->Counter (linijki)

    semMF = sem_open("/semMF", O_CREAT, 0600, 1); // semafor zezwalajaćy na odczyt z kolejki MF
    semFP = sem_open("/semFP", O_CREAT, 0600, 1); // semafor obrazujący liczbe wiadomości w kolejece FP
    semPC = sem_open("/semPC", O_CREAT, 0600, 1); // semafor obrazujący liczbe wiadomości w kolejece FP

    MSG msg;
    msg.type = 1;
    // strcpy(msg.text, "aaa");
    // msgsnd(mid, &msg, strlen(msg.text) + 1, 0);

    // msgrcv(mid, &msg, 1024, 0, 0);
    // printf("%s\n", msg.text);
    pthread_t threadIdFinder;
    pthread_create(&threadIdFinder, NULL, finder, NULL);
    printf("Przesylam na MF\n");
    for (int i = 1; i < argc; i++) //przesłanie ściezki i rozszerzeń do wątku finder
    {
        strcpy(msg.text, argv[i]);
        msgsnd(queueMF, &msg, strlen(msg.text) + 1, 0);
    }
    printf("Koniec przesyłania na MF\n");
    sem_post(semMF);

    pthread_t threadIdProc[get_nprocs()];
    for (int i = 0; i < get_nprocs(); i++)
    {
        pthread_create(&threadIdProc[i], NULL, proc, NULL);
    }
    pthread_t threadIdCounter;
    pthread_create(&threadIdCounter, NULL, counter, NULL);
    // sleep(50);

    printf("sprzątam\n");
    // sprzątanie
    pthread_join(threadIdFinder, NULL);
    for (int i = 0; i < get_nprocs(); i++)
        pthread_join(threadIdProc[i], NULL);
    pthread_join(threadIdCounter, NULL);
    msgctl(queueMF, IPC_RMID, NULL);
    msgctl(queueFP, IPC_RMID, NULL);
    msgctl(queuePCchars, IPC_RMID, NULL);
    msgctl(queuePClines, IPC_RMID, NULL);

    sem_close(semMF);
    sem_unlink("/semMF");
    sem_close(semFP);
    sem_unlink("/semFP");
    sem_close(semPC);
    sem_unlink("/semPC");
    return 0;
}
