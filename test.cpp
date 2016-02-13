//
// Created by artyom on 13.02.16.
//

#include "gtest/gtest.h"
#include "count.h"

TEST(WordCountTest, ConstStringTest)
{
    const char* text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit."
            " Curabitur at sollicitudin risus. Proin id dictum ex. Cum sociis"
            " natoque penatibus et magnis dis parturient montes, nascetur"
            " ridiculus mus. Donec porta felis magna, eu fringilla sapien porta"
            " consectetur. Vestibulum sagittis, dui sit amet sagittis posuere,"
            " augue tellus mollis mauris, id ornare.";

    auto counted = countWords(text);
    auto flipped = flipMap<string, int>(counted);
    int wordsTotal = 0;
    for (auto it: flipped) {
        wordsTotal += it.first;
    }
    ASSERT_EQ(wordsTotal, 50);

    wordStatFlipped m = {{2, "amet"},
                         {2, "consectetur"},
                         {2, "id"}};

    ASSERT_TRUE(std::equal(m.begin(), m.end(), flipped.rbegin()));
}

TEST(WordCountTest, FileTest)
{
    const char* text = loadTextFromFile("book.txt");

    auto counted = countWords(text);
    auto flipped = flipMap<string, int>(counted);

    wordStatFlipped m = {{20304, "и"},
                         {10198, "в"},
                         {8428, "не"},
                         {7822, "что"},
                         {6453, "на"}};

    ASSERT_TRUE(std::equal(m.rbegin(), m.rend(), flipped.rbegin()));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}