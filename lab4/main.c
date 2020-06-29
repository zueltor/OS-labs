#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main() {
    int error;
    List *list = createList();

    if (list == NULL) {
        return EXIT_FAILURE;
    }

    error = processInput(stdin, list);

    if (error != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    error = printToFile(stdout, list);

    if (error != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    freeList(list);

    return EXIT_SUCCESS;
}
