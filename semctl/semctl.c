#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>

#include <sys/sem.h>

#include <sys/shm.h>

#include "semctl.h"

int main(int argc, char *argv[])
{
    // there should be only one input and it should have len == 2, and should be in format "-[]"
    if (argc != 2 || strlen(argv[1]) != 2 || argv[1][0] != '-')
    {
        print_help();
        return EXIT_FAILURE;
    }

    int semd = -1;
    int shmd = -1;

    switch (argv[1][1])
    {
    case 'c':
        ;
        // make semaphore
        semd = semget(SEMKEY, 1, IPC_CREAT | 0644);
        if (semd == -1)
        {
            perror("semget");
            return EXIT_FAILURE;
        }

        puts("Semaphore created");
        // init sem val
        union semun arg;
        arg.val = 0;
        if (semctl(semd, 0, SETVAL) != 0)
        {
            perror("semctl");
            return EXIT_FAILURE;
        }

        // make shared mem
        if (shmget(SHMKEY, SHMSIZE, IPC_CREAT | 0644) == -1)
        {
            perror("shmget");
            return EXIT_FAILURE;
        }

        puts("Shared memory created");

        // make file
        FILE *file = fopen(FILENAME, "w");
        if (!file)
        {
            perror("fopen");
            return EXIT_FAILURE;
        }

        puts("File created");
        fclose(file);

        break;

    case 'r':
        ;
        semd = semget(SEMKEY, 1, 0);
        if (semd == -1)
        {
            perror("semget");
            return EXIT_FAILURE;
        }
        struct sembuf op;
        op.sem_flg = 0;
        op.sem_num = 0;

        // wait for sem
        op.sem_op = 0;
        if (semop(semd, &op, 1) == -1)
        {
            perror("semop");
            return EXIT_FAILURE;
        }

        // now sem released, increase it
        op.sem_op = 1;
        if (semop(semd, &op, 1) == -1)
        {
            perror("semop");
            return EXIT_FAILURE;
        }
        // display the whole file
        display_file();

        // op.sem_op = -1;
        // if (semop(semd, &op, 1) == -1)
        // {
        //     perror("semop");
        //     return EXIT_FAILURE;
        // }

        // remove semaphore, shared mem, and file
        semctl(semd, 0, IPC_RMID);
        
        puts("semaphore removed");

        shmd = shmget(SHMKEY, SHMSIZE, 0);
        if (shmd == -1)
        {
            perror("shmget");
            return EXIT_FAILURE;
        }
        if (shmctl(shmd, IPC_RMID, 0) != 0)
        {
            perror("shmctl");
            return EXIT_FAILURE;
        }
        
        if (remove(FILENAME) != 0)
        {
            perror("remove");
            return EXIT_FAILURE;
        }

        break;

    case 'v':
        // display the whole file

        display_file();
        fclose(file);

        break;
    default:
        printf("Unrecognized option: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void print_help()
{
    puts("-------------------------------------");
    puts("Shared Memory control for story project");
    puts("./shmctl [option]");
    puts("where option can be:");
    puts("\t-c\tmake the story file");
    puts("\t-r\tdisplay the story and remove the story file");
    puts("\t-v\tdisplay the story");
    puts("");
}

void display_file()
{
    FILE *f = fopen(FILENAME, "r");
    if (!f)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    puts("The story so far:");
    char c;
    while ((c = fgetc(f)) != EOF)
    {
        fputc(c, stdout);
    }
    fclose(f);
}