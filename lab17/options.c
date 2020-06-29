//
// Created by Damir on 19.05.2020.
//

#include "options.h"

int print(char *text, int length) {
    int error = write(STDIN_FILENO, text, length);
    if (error == ERROR) {
        perror("Write error");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int eraseSymbols(int *length, int count) {
    int symbols_to_erase = (*length < count) ? *length : count;
    int error;
    int i;
    for (i = 0; i < symbols_to_erase; i++) {
        error = print(ERASE, ERASE_LENGTH);
        --(*length);
        if (error == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int eraseWord(const char *line, int *length) {
    int word_start_position = *length;
    while (word_start_position > 0 && !isspace(line[word_start_position - 1])) {
        word_start_position--;
    }
    while (word_start_position > 0 && isspace(line[word_start_position - 1])) {
        word_start_position--;
    }
    return eraseSymbols(length, *length - word_start_position);
}

int printSymbol(char symbol, char *line, int *length) {
    if (*length == MAX_LENGTH) {
        return breakLine(symbol, line, length);
    }
    line[*length] = symbol;
    ++(*length);
    return print(&symbol, 1);
}

int eraseLine(int *length) {
    return eraseSymbols(length, *length);
}

int makeSound() {
    return print(BEEP, BEEP_LENGTH);
}

int newLine(int *length) {
    *length = 0;
    return print(NEW_LINE, NEW_LINE_LENGTH);
}

int breakLine(char symbol, char *line, int *length) {
    int index = *length - 1;
    int error;

    if (isspace(symbol)){
        return newLine(length);
    }

    while (index > 0 && !isspace(line[index])) {
        --index;
    }

    if (index == 0 && !isspace(line[index])) {
        error = newLine(length);
        if (error == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }
        error = print(&symbol, 1);
        if (error == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }
        ++(*length);
        return EXIT_SUCCESS;
    }

    error = eraseSymbols(length, *length - index);
    if (error == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    error = newLine(length);
    if (error == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    index++;

    error = print(&line[index], MAX_LENGTH - index);
    if (error == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    while (index != MAX_LENGTH) {
        line[*length] = line[index];
        ++(*length);
        index++;
    }

    line[*length] = symbol;
    ++(*length);

    return print(&symbol, 1);
}
