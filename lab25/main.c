#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <wait.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define READER 0
#define WRITER 1
#define SYSTEM_CALL_ERROR -1
#define BUFF_SIZE BUFSIZ
#define MODIFY_INPUT 1
#define NUM_CHILD_PROCESSES 2
#define NUM_FILE_DESCRIPTORS 2
#define NO_MODIFY 0
#define HAVE_INPUT 1
#define CHILD_ID 0
#define NO_INPUT 0

int closeFileDescriptor(int file_descriptor) {
    int error = close(file_descriptor);
    if (error == SYSTEM_CALL_ERROR) {
        perror("Error while closing file descriptor");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void modifyString(char *string, int length) {
    for (int i = 0; i < length; i++) {
        string[i] = (char) toupper(string[i]);
    }
}

int readWriteMessages(int read_fd, int write_fd, int unused_fd, int used_fd, int modify_input) {
    char buff[BUFF_SIZE];
    int error;
    int bytes_read;
    int bytes_wrote;

    error = closeFileDescriptor(unused_fd);
    if (error == EXIT_FAILURE) {
        closeFileDescriptor(used_fd);
        return EXIT_FAILURE;
    }

    while (HAVE_INPUT) {
        bytes_read = read(read_fd, buff, BUFF_SIZE);
        if (bytes_read == SYSTEM_CALL_ERROR) {
            fprintf(stderr, "Error while reading from fd %d: %s\n", read_fd, strerror(errno));
            closeFileDescriptor(used_fd);
            return EXIT_FAILURE;
        }
        if (bytes_read == NO_INPUT) {
            break;
        }

        if (modify_input) {
            modifyString(buff, bytes_read);
        }

        bytes_wrote = write(write_fd, &buff, bytes_read);
        if (bytes_wrote == SYSTEM_CALL_ERROR) {
            fprintf(stderr, "Error while writing to fd %d: %s\n", write_fd, strerror(errno));
            closeFileDescriptor(used_fd);
            return EXIT_FAILURE;
        }
    }

    return closeFileDescriptor(used_fd);
}

int waitAll(int count) {
    int status;
    pid_t pid;

    for (int i = 0; i < count; i++) {
        pid = wait(&status);
        if (pid == SYSTEM_CALL_ERROR) {
            perror("Failed to wait for child process to terminate");
            return EXIT_FAILURE;
        }
        if (WIFEXITED(status) && WEXITSTATUS(status) != EXIT_SUCCESS || WIFSIGNALED(status)) {
            fprintf(stderr, "Child process did not terminate normally\n");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int closeAll(int *fds, int count) {
    int error;
    for (int i = 0; i < count; i++) {
        error = closeFileDescriptor(fds[i]);
        if (error == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    int file_descriptors[NUM_FILE_DESCRIPTORS];
    pid_t pid;

    int error = pipe(file_descriptors);
    if (error == SYSTEM_CALL_ERROR) {
        perror("Error while opening pipe");
        return EXIT_FAILURE;
    }

    pid = fork();
    if (pid == SYSTEM_CALL_ERROR) {
        perror("Error while creating 1st child process");
        closeAll(file_descriptors, NUM_FILE_DESCRIPTORS);
        return EXIT_FAILURE;
    }
    if (pid == CHILD_ID) {
        return readWriteMessages(STDIN_FILENO, file_descriptors[WRITER],
                                 file_descriptors[READER], file_descriptors[WRITER], NO_MODIFY);
    }

    pid = fork();
    if (pid == SYSTEM_CALL_ERROR) {
        perror("Error while creating 2nd child process");
        closeAll(file_descriptors, NUM_FILE_DESCRIPTORS);
        return EXIT_FAILURE;
    }
    if (pid == CHILD_ID) {
        return readWriteMessages(file_descriptors[READER], STDOUT_FILENO,
                                 file_descriptors[WRITER], file_descriptors[READER], MODIFY_INPUT);
    }

    error = closeAll(file_descriptors, NUM_FILE_DESCRIPTORS);
    if (error == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    return waitAll(NUM_CHILD_PROCESSES);
}
