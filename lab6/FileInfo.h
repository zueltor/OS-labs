#ifndef LAB5_FILEINFO_H
#define LAB5_FILEINFO_H

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <poll.h>
#include "constants.h"

struct LineInfo_st {
    int length;
    off_t offset;
};

struct FileInfo_st {
    int file_descriptor;
    int num_lines;
    int max_num_lines;
    struct LineInfo_st *line_info;
};

typedef struct LineInfo_st LineInfo;
typedef struct FileInfo_st FileInfo;

int waitTimeout();

int printAllLines(FileInfo *file_info);

int fillLineInfoTable(FileInfo *file_info);

int initializeFileInfo(FileInfo *file_info, int file_descriptor);

int printLine(FileInfo *file_info, long line_number, char *line_buff);

int processInput(FileInfo *file_info);

int readNumber(long *number, char *number_buff);

int expandTable(FileInfo *file_info);

int closeFile(FileInfo *file_info);


#endif //LAB5_FILEINFO_H
