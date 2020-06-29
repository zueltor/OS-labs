#include "FileInfo.h"

int main(int argc, char **argv) {
    int file_descriptor;
    int error;
    char *filename = (argc == 2) ? argv[1] : "file.txt";

    printf("Filename is %s\n", filename);

    file_descriptor = open(filename, O_RDONLY);
    if (file_descriptor == OPEN_ERROR) {
        perror("File open error");
        return EXIT_FAILURE;
    }

    FileInfo file_info;
    error = initializeFileInfo(&file_info, file_descriptor);
    if (error != EXIT_SUCCESS) {
        closeFile(&file_info);
        return EXIT_FAILURE;
    }

    error = fillLineInfoTable(&file_info);
    if (error != EXIT_SUCCESS) {
        closeFile(&file_info);
        return EXIT_FAILURE;
    }

    error = processInput(&file_info);
    if (error != EXIT_SUCCESS) {
        closeFile(&file_info);
        return EXIT_FAILURE;
    }

    return closeFile(&file_info);
}
