void *counter(void *i)
{
    // int queueFC = *((int *)i);
    int chars = 0;
    int lines = 0;
    printf("counter\n");
    sem_wait(semPC);
    MSG msgChars;
    MSG msgLines;

    printf("counter after sem\n");
    msgrcv(queuePCchars, &msgChars, 1023, 0, 0);
    msgrcv(queuePClines, &msgLines, 1023, 0, 0);
    printf("counter after msg\n");
    int activeProcs = get_nprocs();
    while (activeProcs)
    {

        printf("rcved %s %s\n", msgChars.text, msgLines.text);
        if (strcmp(__END_MSG__, msgChars.text))
            chars += atoi(msgChars.text);
        lines += atoi(msgLines.text);
        sem_wait(semPC);
        msgrcv(queuePCchars, &msgChars, 1023, 0, 0);
        msgrcv(queuePClines, &msgLines, 1023, 0, 0);
        if (!strcmp(__END_MSG__, msgChars.text))
            activeProcs--;
    }
    printf("FINAL\n");
    printf("Lines: %d\n Chars: %d\n", lines, chars);

    return NULL;
}
