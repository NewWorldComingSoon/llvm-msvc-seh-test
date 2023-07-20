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

void
CXXEH_2_parse(const std::string &fmt, const std::string &str)
{
    std::string allstr = fmt + str;
    if (allstr.compare("1234") == 0)
    {
        throw std::exception("heihei");
    }
    printf("a+b=%s\n", allstr.c_str());
    return;
}

void
CXXEH_2_tryParse(const std::string &fmt, const std::string &str)
{
    try
    {
        printf("start parse\n");
        CXXEH_2_parse(fmt, str);
    }
    catch (const std::exception &e)
    {
        printf("except=%s\n", e.what());
        EXPECT_STREQ("heihei", e.what());
    }

    printf("end tryParse\n");
}

TEST(CXXEH, CXXEH_2)
{
    std::string fmt = "12";
    std::string str = "34";
    CXXEH_2_tryParse(fmt, str);
}
