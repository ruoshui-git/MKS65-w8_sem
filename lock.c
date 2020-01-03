#include <stdlib.h>
#include <stdio.h>
#include <sys/sem.h>


void lock(int semd)
{
    
    struct sembuf op;
    op.sem_flg = 0;
    op.sem_num = 0;
    
    // wait for it to be released
    if (semop(semd, &op, 1) == -1)
    {
        perror("semop");
        exit(EXIT_FAILURE);
    }

    // now sem released, increase it
    op.sem_op = 1;
    if (semop(semd, &op, 1) == -1)
    {
        perror("semop");
        exit(EXIT_FAILURE);
    }
}

void release(int semd)
{
    
    struct sembuf op;
    op.sem_flg = 0;
    op.sem_num = 0;
    
    op.sem_op = -1;
    if (semop(semd, &op, 1) == -1)
    {
        perror("semop");
        exit(EXIT_FAILURE);
    }
}