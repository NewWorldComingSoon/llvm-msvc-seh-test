#include <gtest/gtest.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

void
CXXEH_1_parse(int a, int b)
{
    if (a + b == 4)
    {
        throw std::exception("heihei");
    }
    printf("a+b=%d\n", a + b);
    return;
}

TEST(CXXEH, CXXEH_1)
{
    try
    {
        printf("start parse\n");
        CXXEH_1_parse(1, 3);
    }
    catch (const std::exception &e)
    {
        printf("except=%s\n", e.what());
        EXPECT_STREQ("heihei", e.what());
    }
}
