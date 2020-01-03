#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>

#include <sys/sem.h>

#include <sys/shm.h>

#include "const.h"
#include "lock.h"

#define MAX_ADDITION_LEN 1024

int *attach_last_size();
void detach_last_size(int *size);

int main(void)
{
    int semd = semget(SEMKEY, 1, 0);
    if (semd == -1)
    {
        perror("semget");
        return EXIT_FAILURE;
    }

    puts("Trying to get in...");
    lock(semd);

    int *size_p = attach_last_size();
    int size = *size_p;

    FILE *f = fopen(FILENAME, "r+");
    if (!f)
    {
        perror("fopen");
        puts("create your story file first");
        return EXIT_FAILURE;
    }

    char buf[size + 1];

    if (size > 0)
    {
        // move file ptr to the end
        fseek(f, -size, SEEK_END);

        // one extra character in case no null

        fread(buf, size, 1, f);
    }
    buf[size] = '\0';

    printf("Last addition: %s\n", buf);

    char addition[MAX_ADDITION_LEN];
    fputs("Your addition: ", stdout);
    fgets(addition, MAX_ADDITION_LEN, stdin);

    // update shared mem
    *size_p = strlen(addition);

    fputs(addition, f);
    
    detach_last_size(size_p);
    fclose(f);
    release(semd);
    
    return EXIT_SUCCESS;
}

int *attach_last_size()
{
    int shmd = shmget(SHMKEY, SHMSIZE, 0);
    if (shmd == -1)
    {
        perror("shmget");
    }

    void *ptr = shmat(shmd, NULL, 0);
    if (ptr == (void *)-1)
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    return (int *)ptr;
}

void detach_last_size(int *size)
{
    if (shmdt(size) == -1)
    {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}
