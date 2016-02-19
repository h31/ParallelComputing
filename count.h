//
// Created by artyom on 14.02.16.
//

#ifndef LAB1_SIMPLE_COUNT_H
#define LAB1_SIMPLE_COUNT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#include "utils.h"

const int threadCount = omp_get_num_procs();

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

wordStat countWordsBlockwise(const char* text, size_t len) {
    wordStat stat;
    vector<size_t> blockStart;

    size_t blockSize = len / threadCount + 1;
    size_t startPos = 0;
    size_t endPos;

    for (int i = 0; i < threadCount; i++) {
        blockStart.push_back(startPos);
        endPos = startPos + blockSize;
        while (true) {
            if ((endPos > len) || (text[endPos] == ' ')) {
                break;
            }
            endPos++;
        }
        startPos = endPos + 1;
        if (startPos > len) {
            break;
        }
    }

    for (int i = 0; i < threadCount; i++) {
        startPos = blockStart[i];
        endPos = i == (threadCount-1) ? len : blockStart[i + 1];
        auto localMap = countWords(text + startPos, endPos - startPos);
        for (auto& it: localMap) {
            int prevCount = stat.count(it.first) ? stat[it.first] : 0;
            stat[it.first] = prevCount + it.second;
        }
    }
    return stat;
}

wordStat countWordsOpenMP(const char* text, size_t len) {
    wordStat stat;
    vector<size_t> blockStart;

    size_t blockSize = len / threadCount + 1;
    size_t startPos = 0;
    size_t endPos;

    for (int i = 0; i < threadCount; i++) {
        blockStart.push_back(startPos);
        endPos = startPos + blockSize;
        while (true) {
            if ((endPos > len) || (text[endPos] == ' ')) {
                break;
            }
            endPos++;
        }
        startPos = endPos + 1;
        if (startPos > len) {
            break;
        }
    }

#pragma omp parallel for
    for (int i = 0; i < threadCount; i++) {
        startPos = blockStart[i];
        endPos = i == (threadCount-1) ? len : blockStart[i + 1];
        auto localMap = countWords(text + startPos, endPos - startPos);
#pragma omp critical(merge)
        for (auto& it: localMap) {
            int prevCount = stat.count(it.first) ? stat[it.first] : 0;
            stat[it.first] = prevCount + it.second;
        }
    }
    return stat;
}

#endif //LAB1_SIMPLE_COUNT_H
