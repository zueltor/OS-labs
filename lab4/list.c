#include <stdio.h>
#include <stdlib.h>
#include "list.h"

List *createList() {
    List *list = (List *) malloc(sizeof(List));

    if (list == NULL) {
        perror("Not enough memory to create list");
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;
    return list;
}

Node *createNode(const char *data, size_t string_size) {
    Node *new_node = (Node *) malloc(sizeof(Node));

    if (new_node == NULL) {
        perror("Not enough memory to create node");
        return NULL;
    }

    new_node->data = (char *) malloc((string_size) * sizeof(char));
    if (data == NULL) {
        perror("Not enough memory to hold string");
        free(new_node);
        return NULL;
    }

    new_node->next = NULL;
    strncpy(new_node->data, data, string_size - 1);
    new_node->data[string_size - 1] = '\0';
    return new_node;
}

int insertToList(List *list, const char *data, size_t string_size) {
    if (list == NULL) {
        fprintf(stderr, "List null ptr error");
        return EXIT_FAILURE;
    }

    Node *new_node = createNode(data, string_size);
    if (new_node == NULL) {
        return EXIT_FAILURE;
    }

    if (list->head == NULL) {
        list->head = new_node;
    } else {
        list->tail->next = new_node;
    }

    list->tail = new_node;
    return EXIT_SUCCESS;
}

void freeList(List *list) {
    if (list == NULL) {
        return;
    }

    Node *current = list->head;
    Node *next;

    while (current != NULL) {
        next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    free(list);
}

int processInput(FILE *input, List *list) {
    size_t string_size;
    char *result;
    int error;

    char *buff = (char *) malloc(BUFF_SIZE * sizeof(char));
    if (buff == NULL) {
        perror("Not enough memory to create buffer");
        return EXIT_FAILURE;
    }

    while (1) {
        result = fgets(buff, BUFF_SIZE, input);
        if (result == NULL || buff[0] == '.') {
            free(buff);
            return EXIT_SUCCESS;
        }

        string_size = strlen(buff);
        if (string_size + 1 == BUFF_SIZE && buff[BUFF_SIZE - 2] != '\n') {
            fprintf(stderr, "Too long string to hold\n"
                            "Maximum allowed length is %d\n", BUFF_SIZE - 1);
            free(buff);
            return EXIT_FAILURE;
        }

        error = insertToList(list, buff, string_size);
        if (error != EXIT_SUCCESS) {
            free(buff);
            return EXIT_FAILURE;
        }
    }
}

int printToFile(FILE *output, List *list) {
    if (list == NULL) {
        fprintf(stderr, "List null ptr error");
        return EXIT_FAILURE;
    }

    if (output == NULL) {
        fprintf(stderr, "File null ptr error");
        return EXIT_FAILURE;
    }

    Node *current = list->head;

    while (current != NULL) {
        fprintf(output, "%s\n", current->data);
        current = current->next;
    }

    return EXIT_SUCCESS;
}
