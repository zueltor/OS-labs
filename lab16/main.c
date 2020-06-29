#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#define READ_ERROR -1

int askQuestion(const char *question, char *answer) {
    int error;

    printf("%s\n", question);

    error = read(STDIN_FILENO, answer, 1);
    if (error == READ_ERROR) {
        perror("Failed to read an answer");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int setAttributes(struct termios *old_attrs, struct termios *new_attrs) {
    int error;

    error = tcgetattr(STDIN_FILENO, old_attrs);
    if (error) {
        perror("Failed to get attributes");
        return EXIT_FAILURE;
    }

    *new_attrs = *old_attrs;
    new_attrs->c_lflag &= ~(ICANON | ECHO);
    new_attrs->c_cc[VMIN] = 1;

    error = tcsetattr(STDIN_FILENO, TCSANOW, new_attrs);
    if (error) {
        perror("Failed to set attributes");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void printResult(char answer) {
    if (answer == 'A' || answer == 'a') {
        printf("\nCorrect\n");
        return;
    }
    if (answer > 'A' && answer <= 'z') {
        printf("\nIncorrect\n");
        return;
    }

    printf("\nThat is not a letter of the english alphabet\n");
}

int main() {
    struct termios old_attrs, new_attrs;
    int error;
    char *question = "What is the first letter of the english alphabet?";

    error = setAttributes(&old_attrs, &new_attrs);
    if (error == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    char answer;
    error = askQuestion(question, &answer);
    if (error) {
        tcsetattr(STDIN_FILENO, TCSANOW, &old_attrs);
        return EXIT_FAILURE;
    }

    printResult(answer);

    error = tcsetattr(STDIN_FILENO, TCSANOW, &old_attrs);
    if (error) {
        perror("Failed to set attributes back");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}