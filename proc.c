#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "common.h"

#include "proc.h"

void wc(char *file)
{
    int lines = 0, chars = 0;
    int d = open(file, O_RDONLY);
    if (d < 0)
    {
        printf("Error on opening file: %s\nRunning with user: %d, group: %d\nPlease check if you have access to this file\n", file, getgid(), getuid());
        perror("Błąd otwarcia pliku");
        return;
    }
    struct stat *buffer = malloc(sizeof(struct stat));
    stat(file, buffer);
    // printf("%ld\n", buffer->st_size); // file size
    char *ptr = mmap(NULL, buffer->st_size, PROT_READ, MAP_SHARED, d, 0);
    free(buffer);
    for (int i = 0; i < buffer->st_size; i++)
    {
        // printf("%c", (char)*(ptr + i));
        // liczymy linie według definicji POSIX: linia to ciąg znaków zakończony znakiem nowej linii
        if ((char)*(ptr + i) == '\n')
            lines++;
        if (!isspace((char)*(ptr + i)))
            chars++;
    }
    printf("procs: %ld, plik: %s znakow %d, linijek %d\n", pthread_self(), file, chars, lines);
    close(d);
    TWOINTS *tmp = malloc(sizeof(TWOINTS)); // odpowiednikiem byłby malloc(2*sizeof(int))
    tmp->chars = chars;
    tmp->lines = lines;
    stack_push(&results, tmp);
}

void *proc(void *i)
{
    printf("procs\n");
    while (1)
    {
        char *tmp = stack_pop(&paths);
        if (tmp != NULL)
        {
            wc(tmp);
            free(tmp);
        }
        else
            break;
    }

    stack_endData(&results);
    printf("end proc\n");
    return NULL;
}
