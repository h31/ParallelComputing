#include <iostream>
#include "utils.h"
#include "count.h"

using namespace std;

int main() {
    char* text = loadTextFromFile("book.txt");

    auto counted = countWordsParallel(text, strlen(text));
    auto flipped = flipMap<string, int>(counted);
    printStats(flipped);
    return 0;
}