#include "constants.h"
#include "options.h"

int setNewAttributes(struct termios *old_attrs, struct termios *new_attrs) {
    int error = tcgetattr(STDIN_FILENO, old_attrs);
    if (error == ERROR) {
        perror("Failed to get attributes");
        return EXIT_FAILURE;
    }

    *new_attrs = *old_attrs;
    new_attrs->c_lflag &= ~(ICANON | ECHO | ISIG);
    new_attrs->c_cc[VMIN] = 1;

    error = tcsetattr(STDIN_FILENO, TCSANOW, new_attrs);
    if (error == ERROR) {
        perror("Failed to set attributes");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int setOldAttributes(struct termios *old_attrs) {
    int error = tcsetattr(STDIN_FILENO, TCSANOW, old_attrs);
    if (error == ERROR) {
        perror("Failed to set old attributes");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int chooseOption(char symbol, char *line, int *length) {

    switch (symbol) {
        case ERASE_SYMBOL:
            return eraseSymbols(length, 1);
        case ERASE_WORD:
            return eraseWord(line, length);
        case ERASE_LINE:
            return eraseLine(length);
        case NEW_LINE_SYMBOL:
            return newLine(length);
        case EXIT:
            return EXIT_SUCCESS;
        default:
            if (!isprint(symbol)) {
                return makeSound();
            }
    }
    return printSymbol(symbol, line, length);
}

int processInput(int fd) {
    int length = 0;
    int error;
    char line[MAX_LENGTH] = {0};
    char symbol;

    do {
        error = read(fd, &symbol, 1);
        if (error == ERROR) {
            return EXIT_FAILURE;
        }
        error = chooseOption(symbol, line, &length);
        if (error == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }
    } while (symbol != EXIT || length != 0);

    return EXIT_SUCCESS;
}

int main() {
    struct termios old_attrs, new_attrs;
    int error;

    error = setNewAttributes(&old_attrs, &new_attrs);
    if (error == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    error = processInput(STDIN_FILENO);
    if (error == EXIT_FAILURE) {
        setOldAttributes(&old_attrs);
        return EXIT_FAILURE;
    }

    return setOldAttributes(&old_attrs);
}
