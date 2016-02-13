//
// Created by artyom on 14.02.16.
//

#ifndef LAB1_UTILS_H
#define LAB1_UTILS_H

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <map>
#include <string.h>

using namespace std;

typedef map<string, int> wordStat;
typedef multimap<int, string> wordStatFlipped;

template<typename A, typename B>
std::pair<B,A> flipPair(const std::pair<A,B> &p)
{
    return std::pair<B,A>(p.second, p.first);
}

template<typename A, typename B>
std::multimap<B,A> flipMap(const std::map<A,B> &src)
{
    std::multimap<B,A> dst;
    std::transform(src.begin(), src.end(),
                   std::inserter(dst, dst.begin()), flipPair<A,B>);
    return dst;
}

void printStats(wordStatFlipped stat) {
    printf("Stats: \n");
    int position = 0;

    const unsigned long showFirst = 5;
    auto end = stat.size() > showFirst ? std::next(stat.rbegin(), showFirst) : stat.rend();

    for (auto it = stat.rbegin(); it != end; ++it) {
        if (++position > 5) {
            break;
        }
        printf("%5d %s \n", it->first, it->second.c_str());
    }
}

char* loadTextFromFile(const char* path) {
    FILE*file = fopen(path, "r");
    if (file == NULL) {
        perror("File error");
        exit(1);
    }

    // obtain file size:
    fseek(file, 0, SEEK_END);
    long lSize = (size_t) ftell(file);
    rewind(file);

    // allocate memory to contain the whole file:
    char* buffer = (char*)malloc((size_t) lSize);
    memset(buffer, '\0', (size_t) lSize);

    // copy the file into the buffer:
    int result = fread(buffer, 1, lSize, file);

    return buffer;
}

#endif //LAB1_UTILS_H
