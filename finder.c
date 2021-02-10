#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>

#include <pthread.h>
#include <sys/sysinfo.h>
#include <stdlib.h>
#include <dirent.h>

#include "common.h"

#include "finder.h"

// https://stackoverflow.com/a/744822
int checkExtension(const char *filename, const char *extension)
{
    if (!filename || !extension)
        return 0;
    size_t lenFilename = strlen(filename);
    size_t lenExtension = strlen(extension);
    if (lenExtension > lenFilename)
        return 0;
    // if ()
    return strncmp(filename + lenFilename - lenExtension, extension, lenExtension) == 0;
    // filename to wskażnik na początek napisu, więc dodajemy jego długość i odejmujemy długość suffixu, żeby uzyskać samo rozszerzenie pliku
}

void dirRead(int queue, char *dirName)
{
    printf("szukanie w %s\n", dirName);
    DIR *rootDir;
    rootDir = opendir(dirName);

    struct dirent *dirEnt;

    while ((dirEnt = readdir(rootDir)) != NULL)
    {
        switch (dirEnt->d_type)
        {
        case DT_DIR:
            printf("Dir %s {\n", dirEnt->d_name);
            if (strcmp(dirEnt->d_name, "..") != 0 && strcmp(dirEnt->d_name, ".") != 0)
            {
                char buf[1024];
                snprintf(buf, sizeof buf, "%s/%s", dirName, dirEnt->d_name);
                dirRead(queue, buf);
            }
            printf("} \n");
            break;
        case DT_REG:
            printf("Reg %s\n", dirEnt->d_name);
            char buf[1024];
            snprintf(buf, sizeof buf, "%s/%s", dirName, dirEnt->d_name);
            int flag = 0;
            for (int i = 0; i < extensionsCounter; i++) //check extensions
            {
                if (checkExtension(buf, extensions[i]))
                    flag = 1;
            }
            if (flag)
            {
                MSG msg;
                msg.type = 1;
                strcpy(msg.text, buf);
                msgsnd(queue, &msg, strlen(msg.text) + 1, 0);
                sem_post(semFP);
            }
            break;

        default:
            printf("Other/Unkown %s\n", dirEnt->d_name);
            break;
        }
    }
}

void *finder(void *info)
{
    printf("thread\n");
    MSG msg;

    printf("dir: %s\n", rootDirName);
    for (int j = 0; j < extensionsCounter; j++)
        printf("args: %s\n", extensions[j]);
    dirRead(queueFP, rootDirName);
    for (int i = 0; i < get_nprocs(); i++)
    {
        strcpy(msg.text, __END_MSG__);
        msgsnd(queueFP, &msg, strlen(msg.text) + 1, 0);
        sem_post(semFP);
    }
    printf("end finder\n");
    return NULL;
}