#include <iostream>
#include "utils.h"
#include "count.h"

using namespace std;

int main() {
    pair<const char*, size_t> text = loadTextFromFile("book.txt");

    auto counted = countWords(text.first, text.second);
    auto flipped = flipMap<string, int>(counted);
    printStats(flipped);
    return 0;
}