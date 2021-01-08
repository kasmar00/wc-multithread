void *counter(void *i)
{
    // int queueFC = *((int *)i);
    int chars = 0;
    int lines = 0;
    printf("counter\n");
    sem_wait(semPC);
    MSG msgChars;
    MSG msgLines;

    int activeProcs = get_nprocs();
    printf("counter after sem\n");
    msgrcv(queuePCchars, &msgChars, 1023, 0, 0);
    msgrcv(queuePClines, &msgLines, 1023, 0, 0);
    if (!strcmp(__END_MSG__, msgChars.text))
        activeProcs--;
    printf("counter after msg\n");
    while (activeProcs)
    {

        printf("actv proc: %d; rcved %s %s\n", activeProcs, msgChars.text, msgLines.text);
        if (strcmp(__END_MSG__, msgChars.text))
        {
            chars += atoi(msgChars.text);
            lines += atoi(msgLines.text);
        }
        sem_wait(semPC);
        msgrcv(queuePCchars, &msgChars, 1023, 0, 0);
        msgrcv(queuePClines, &msgLines, 1023, 0, 0);
        if (!strcmp(__END_MSG__, msgChars.text))
            activeProcs--;
    }
    printf("FINAL\n");
    printf("Liczba linii: %d\nLiczba znaków: %d\n", lines, chars);

    return NULL;
}
