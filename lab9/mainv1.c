#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define FORK_ERROR -1
#define CHILD_ID 0

int execute(char *command, char **argv) {
    pid_t process_id = fork();
    if (process_id == FORK_ERROR) {
        perror("Could not fork the process");
        EXIT_FAILURE;
    }

    if (process_id == CHILD_ID) {
        execvp(command, argv);
        perror("Could not execute command");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {

    char *filename = (argc > 1) ? argv[1] : "file.txt";
    char *command = "cat";
    char *command_argv[] = {command, filename, NULL};

    int error = execute(command, command_argv);
    if (error != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    printf("\nParent process message\n");

    return EXIT_SUCCESS;
}
