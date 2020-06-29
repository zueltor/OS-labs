#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

extern char **environ;

#define FORK_ERROR -1
#define WAIT_ERROR -1
#define MIN_ARG_COUNT 2
#define CHILD_ID 0

int execvpe(const char *file, char *argv[], char *envp[]) {
    char **old_env = environ;
    environ = envp;
    execvp(file, argv);
    environ = old_env;
    return EXIT_FAILURE;
}

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
        return EXIT_SUCCESS;
    }

    fprintf(stderr, "Unknown cause of process termination\n");
    return EXIT_FAILURE;
}

int execute(char *command, char **argv, char **envp) {
    pid_t process_id = fork();
    if (process_id == FORK_ERROR) {
        perror("Could not fork the process");
        EXIT_FAILURE;
    }

    if (process_id == CHILD_ID) {
        execvpe(command, argv, envp);
        perror("Could not execute command");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {

    if (argc < MIN_ARG_COUNT) {
        fprintf(stderr, "At least 1 argument (the name of command) is required\n");
        return EXIT_FAILURE;
    }

    char *command = argv[1];
    char **command_argv = &argv[1];
    char *command_envp[] = {"VARIABLE1=VALUE1", "VAR2=VALUE2", "PATH=/usr/bin:/bin", NULL};

    int error = execute(command, command_argv, command_envp);
    if (error != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    return waitChild();
}
