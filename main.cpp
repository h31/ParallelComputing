#include <iostream>
#include "utils.h"
#include "count.h"

using namespace std;

int main() {
    const char* text = loadTextFromFile("book.txt");

    auto counted = countWords(text);
    auto flipped = flipMap<string, int>(counted);
    printStats(flipped);
    return 0;
}