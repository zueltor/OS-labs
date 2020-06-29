#ifndef LAB4_LIST_H
#define LAB4_LIST_H

#include <string.h>

#define BUFF_SIZE BUFSIZ

typedef struct Node_st Node;
struct Node_st {
    char *data;
    Node *next;
};

typedef struct List_st List;
struct List_st {
    Node *head;
    Node *tail;
};

List *createList();

Node *createNode(const char *data, size_t string_size);

int insertToList(List *list, const char *data, size_t string_size);

void freeList(List *list);

int processInput(FILE *input, List *list);

int printToFile(FILE *output, List *list);

#endif //LAB4_LIST_H

