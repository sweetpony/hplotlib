#include <gtest/gtest.h>

#include "Text.hpp"

using namespace hpl;

Text* generateText() {
    Limits limits(0.0, 0.0, 10.0, 10.0);
    Text* t = new Text("abcde", 1.0, 2.0, 4.0, 5.0, limits);
    return t;
}

TEST(Text, member_validity)
{
    Text* t = generateText();
    EXPECT_EQ("abcde", t->text);
}

