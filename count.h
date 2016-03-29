//
// Created by artyom on 14.02.16.
//

#ifndef LAB1_SIMPLE_COUNT_H
#define LAB1_SIMPLE_COUNT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#include <thread>
#include "utils.h"

#ifdef WITH_OPENMP
const int defaultThreadCount = omp_get_num_procs();
#else
const int defaultThreadCount = std::thread::hardware_concurrency();
#endif

void* countThread(void* arg);

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
    delete[] workArray;
    return stat;
}

wordStat countWordsBlockwise(const char* text, size_t len, int threadCount) {
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

#ifdef WITH_OPENMP

wordStat countWordsOpenMP(const char* text, size_t len, int threadCount) {
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

#endif

struct threadData {
    const char* text;
    size_t len;
    size_t start;
    size_t end;

    wordStat* stat;
    pthread_mutex_t* mutex;

    pthread_t thread_id;
};

wordStat countWordsPthreads(const char* text, size_t len, int threadCount) {
    wordStat stat;
    vector<size_t> blockStart;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    size_t blockSize = len / threadCount + 1;
    size_t startPos = 0;
    size_t endPos;
    threadData data[threadCount];

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
        data[i].text = text;
        data[i].len = len;
        data[i].start = blockStart[i];
        data[i].end = i == (threadCount-1) ? len : blockStart[i + 1];

        data[i].stat = &stat;
        data[i].mutex = &mutex;

        int s = pthread_create(&data[i].thread_id, NULL,
                               &countThread, &data[i]);
        assert (s == 0);
    }

    for (int i = 0; i < threadCount; i++) {
        pthread_join(data[i].thread_id, NULL);
    }

    return stat;
}

void* countThread(void* arg) {
    threadData data = *(threadData*) arg;

    size_t startPos = data.start;
    size_t endPos = data.end;

    auto localMap = countWords(data.text + startPos, endPos - startPos);

    pthread_mutex_lock(data.mutex);
    for (auto& it: localMap) {
        int prevCount = data.stat->count(it.first) ? (*data.stat)[it.first] : 0;
        (*data.stat)[it.first] = prevCount + it.second;
    }
    pthread_mutex_unlock(data.mutex);
    return NULL;
}

#endif //LAB1_SIMPLE_COUNT_H
