union semun {
               int              val;    /* Value for SETVAL */
               struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
               unsigned short  *array;  /* Array for GETALL, SETALL */
               struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
           };

#define SHMKEY 123456789

#include <stdio.h>
#include <stdlib.h>

int main (int argc, char * [] argv)
{
    // there should be only one input and it should have len == 2, and should be in format "-[]"
    if (argc != 2 || strlen(argv[1]) != 2 || argv[1][0] != '-')
    {
        print_help();
        return EXIT_FAILURE;
    }

    switch (argv[1][1])
    {
        case 'c':

        // make shared mem

        // make semaphore

        // make file

            break;

        case 'r':
        // display the whole file

        // remove semaphore, shared mem, and file





            break;

        case 'v':

        // display the whole file

            break;
        default:
            printf("Unrecognized option: %s\n", argv[1]);
            return EXIT_FAILURE;
    }
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
