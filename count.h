//
// Created by artyom on 14.02.16.
//

#ifndef LAB1_SIMPLE_COUNT_H
#define LAB1_SIMPLE_COUNT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

const int threadCount = 4;

wordStat countWords(const char* text, size_t len) {
    char* workArray = new char[len + 1];
    strncpy(workArray, text, len);
    workArray[len] = '\0';

    const char* delim = " ,. \"!-?()";
    char *saveptr;

    wordStat stat;

    char* pch = strtok_r(workArray, delim, &saveptr);

    while (pch != NULL) {
        int prevCount = stat.count(pch) ? stat[pch] : 0;
        stat[pch] = prevCount + 1;
        pch = strtok_r(NULL, delim, &saveptr);
    }
    return stat;
}


wordStat countWordsParallel(const char* text, size_t len) {
    wordStat stat;

    size_t segmentSize = len/threadCount + 1;
    size_t startPos = 0;

    for (int i = 1; i <= threadCount; i++) {
        size_t endPos = startPos + segmentSize;
        while (true) {
            if ((endPos > len) || (text[endPos] == ' ')) {
                break;
            }
            endPos++;
        }
        auto localMap = countWords(text + startPos, endPos - startPos);
        for (auto& it: localMap) {
            int prevCount = stat.count(it.first) ? stat[it.first] : 0;
            stat[it.first] = prevCount + it.second;
        }
        startPos = endPos + 1;
        if (startPos > len) {
            break;
        }
    }
    return stat;
}

#endif //LAB1_SIMPLE_COUNT_H
