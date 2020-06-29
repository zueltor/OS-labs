#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define FORK_ERROR -1
#define WAIT_ERROR -1
#define CHILD_ID 0

int waitChild() {
    int wait_status;
    pid_t pid = wait(&wait_status);
    if (pid == WAIT_ERROR) {
        perror("Error while waiting for process to terminate");
        return EXIT_FAILURE;
    }
    if (WIFEXITED(wait_status)) {
        int exit_status = WEXITSTATUS(wait_status);
        printf("Child process terminated normally with exit status %d\n", exit_status);
        return EXIT_SUCCESS;
    }
    if (WIFSIGNALED(wait_status)) {
        int terminate_signal = WTERMSIG(wait_status);
        printf("Child process was terminated by signal %d\n", terminate_signal);
    }
    return EXIT_SUCCESS;
}

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

    error = waitChild();
    if (error != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    printf("\nParent process message\n");

    return EXIT_SUCCESS;
}
