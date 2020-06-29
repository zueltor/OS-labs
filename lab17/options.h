//
// Created by Damir on 19.05.2020.
//

#ifndef LAB17_OPTIONS_H
#define LAB17_OPTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include "constants.h"

int print(char *text, int length);

int eraseSymbols(int *length, int count);

int eraseLine(int *length);

int eraseWord(const char *line, int *length);

int makeSound();

int newLine(int *length);

int breakLine(char symbol, char *line, int *length);

int printSymbol(char symbol, char *line, int *length);

#endif //LAB17_OPTIONS_H
