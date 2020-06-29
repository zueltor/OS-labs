#include "FileInfo.h"

int fillLineInfoTable(FileInfo *file_info) {
    if (file_info == NULL) {
        fprintf(stderr, "Null pointer error while filling line information\n");
        return EXIT_FAILURE;
    }

    char buff[BUFF_SIZE];
    int bytes_read;
    int line_length = 0;
    int current_line = 0;
    int error;
    off_t offset = 0;

    do {
        bytes_read = read(file_info->file_descriptor, buff, BUFF_SIZE);

        for (int i = 0; i < bytes_read; i++) {
            if (buff[i] != NEW_LINE) {
                line_length++;
                continue;
            }

            if (current_line == file_info->max_num_lines) {
                error = expandTable(file_info);
                if (error == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
            }

            file_info->line_info[current_line].length = line_length;
            file_info->line_info[current_line].offset = offset;
            offset += line_length + 1;
            current_line++;
            line_length = 0;
        }
    } while (bytes_read > 0);

    if (bytes_read == READ_ERROR) {
        perror("Could not read from file");
        return EXIT_FAILURE;
    }

    file_info->num_lines = current_line;
    return EXIT_SUCCESS;
}

int initializeFileInfo(FileInfo *file_info, int file_descriptor) {
    if (file_info == NULL) {
        fprintf(stderr, "Null pointer error while initializing file information\n");
        return EXIT_FAILURE;
    }

    file_info->file_descriptor = file_descriptor;
    file_info->num_lines = 0;
    file_info->max_num_lines = NUM_LINES_LIMIT;
    file_info->line_info = (LineInfo *) malloc(NUM_LINES_LIMIT * sizeof(LineInfo));
    if (file_info->line_info == NULL) {
        perror("Error while allocating space to hold line information");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int printLine(FileInfo *file_info, long line_number, char *line_buff) {

    if (line_number == EXIT_LINE_NUMBER) {
        return EXIT_SUCCESS;
    }

    if (file_info == NULL || line_buff == NULL) {
        fprintf(stderr, "Null pointer error while printing line\n");
        return EXIT_FAILURE;
    }

    long line_index = line_number - 1;
    int lseek_error;
    int bytes_left = file_info->line_info[line_index].length;
    int bytes_read;
    int chunk = (BUFF_SIZE > bytes_left) ? bytes_left : BUFF_SIZE;

    lseek_error = lseek(file_info->file_descriptor, file_info->line_info[line_index].offset, SEEK_SET);
    if (lseek_error == LSEEK_ERROR) {
        perror("Error while finding required line number");
        return EXIT_FAILURE;
    }

    printf("Line number %ld:\n", line_number);

    while (bytes_left > 0) {
        bytes_read = read(file_info->file_descriptor, line_buff, chunk);
        if (bytes_read == READ_ERROR) {
            perror("Error while printing required line");
            return EXIT_FAILURE;
        }
        if (bytes_read == NO_INPUT) {
            break;
        }
        printf("%.*s", bytes_read, line_buff);
        bytes_left -= bytes_read;
        chunk = (BUFF_SIZE > bytes_left) ? bytes_left : BUFF_SIZE;
    }

    if (bytes_left > 0) {
        fprintf(stderr, "Could not fullly print required line\n");
    }

    printf("\n");
    return EXIT_SUCCESS;
}

int readNumber(long *number, char *number_buff) {

    int bytes_read = read(STDIN_FD, number_buff, BUFF_SIZE);
    if (bytes_read == READ_ERROR) {
        perror("Could not read input number");
        return EXIT_FAILURE;
    }

    if (bytes_read == NO_INPUT) {
        fprintf(stderr, "No input to read number\n");
        return EXIT_FAILURE;
    }

    if (number_buff[bytes_read - 1] != NEW_LINE) {
        fprintf(stderr, "Entered string is too long\n");
        return EXIT_FAILURE;
    }

    number_buff[bytes_read - 1] = END_OF_LINE;

    char *end_ptr;
    char *buff_end_ptr = number_buff + bytes_read - 1;

    *number = strtol(number_buff, &end_ptr, 10);

    if (errno == ERANGE) {
        fprintf(stderr, "Number is outside the range  of  representable values\n");
        return EXIT_FAILURE;
    }

    for (char *c = end_ptr; c != buff_end_ptr; c++) {
        if (!isspace(*c)) {
            fprintf(stderr, "Wrong number format\n");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int processInput(FileInfo *file_info) {
    if (file_info == NULL) {
        fprintf(stderr, "Null pointer error while processing input\n");
        return EXIT_FAILURE;
    }

    int error;
    long line_number;
    char buff[BUFF_SIZE];

    do {
        printf("Enter line number from 1 to %d to print it. Enter 0 to exit.\n", file_info->num_lines);

        error = readNumber(&line_number, buff);
        if (error == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }

        if (line_number < 0 || line_number > file_info->num_lines) {
            fprintf(stderr, "Entered number is out of range\n");
            return EXIT_FAILURE;
        }

        error = printLine(file_info, line_number, buff);
        if (error == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }

    } while (line_number != EXIT_LINE_NUMBER);

    return EXIT_SUCCESS;
}

int closeFile(FileInfo *file_info) {
    if (file_info == NULL) {
        return EXIT_SUCCESS;
    }

    int error;

    free(file_info->line_info);

    error = close(file_info->file_descriptor);
    if (error == CLOSE_ERROR) {
        perror("Could not close file");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int expandTable(FileInfo *file_info) {
    if (file_info == NULL) {
        fprintf(stderr, "Null pointer error while expanding table\n");
        return EXIT_FAILURE;
    }

    file_info->max_num_lines *= 2;
    file_info->line_info = (LineInfo *) realloc(file_info->line_info, sizeof(LineInfo) * file_info->max_num_lines);
    if (file_info->line_info == NULL) {
        fprintf(stderr, "Could not reallocate table\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
