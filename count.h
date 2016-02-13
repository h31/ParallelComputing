//
// Created by artyom on 14.02.16.
//

#ifndef LAB1_SIMPLE_COUNT_H
#define LAB1_SIMPLE_COUNT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

const int threadBlockSize = 10;

wordStat countWords(const char* text) {
    char* workArray = new char[strlen(text)];
    strcpy(workArray, text);
    const char* delim = " ,. \"!-?()";

    wordStat stat;

    char* pch = strtok(workArray, delim);

    while (pch != NULL) {
        int prevCount = stat.count(pch) ? stat[pch] : 0;
        stat[pch] = prevCount + 1;
        pch = strtok (NULL, delim);
    }
    delete workArray;
    return stat;
}
#endif //LAB1_SIMPLE_COUNT_H
