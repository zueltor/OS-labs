#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
 
void openClose(char *filename){
    int error;
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror(filename);
        return;
    }
 
    printf("File is opened successfully\n");
 
    error = fclose(file);
    if (error == EOF) {
        perror(filename);
        return;
    }
 
    printf("File is closed successfully\n");
}
 
void printID(){
    int uid;
    int euid;
    uid = getuid();
    euid = geteuid();
    printf("UID=%u\nEffective UID=%u\n", uid, euid);
}
 
int main(int argc, char *argv[]) {
    char *filename = (argc == 2) ? argv[1] : "file";
    int uid;
    int error;
 
    printID();
    openClose(filename);
 
    uid = getuid();
    error = setuid(uid);
    if (error == -1) {
        perror("Failed to set uid");
        return EXIT_FAILURE;
    }
 
    printID();
 
    openClose(filename);
 
    return EXIT_SUCCESS;
}