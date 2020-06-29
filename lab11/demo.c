#include <stdio.h>

void printLines(char **argv) {
    for (char **c = argv; *c != NULL; ++c) {
        printf("%s\n", *c);
    }
}

int main(int argc, char **argv, char **envp) {
    int k = 2 / (argc - 1);

    printf("Args:\n");
    printLines(argv);

    printf("Env variables:\n");
    printLines(envp);
    return 0;
}
