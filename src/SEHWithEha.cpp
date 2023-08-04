#include <gtest/gtest.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

TEST(SEHWithEha, SEHWithEha_0)
{
    printf("except 00000 !\n");
    int value = 0;
    try
    {
        value += 1;
        int *p = (int *)1;
        *p = NULL;
    }
    catch (...)
    {
        value += 1;
        printf("except!\n");
    }

    if (value == 2)
    {
        printf("ok\n");
    }
    else
    {
        printf("failed\n");
    }
}

TEST(SEHWithEha, SEHWithEha_1)
{
    int value = 0;
    try
    {
        value += 1;
        int *p = (int *)1;
        *p = NULL;
    }
    catch (...)
    {
        value += 1;
        printf("except!\n");
    }

    EXPECT_EQ(2, value);
}

TEST(SEHWithEha, SEHWithEha_2)
{
    int value = 0;
    try
    {
        value += 1;
        int *p = (int *)1;
        *p = NULL;
    }
    catch (...)
    {
        value += 1;
        printf("except!\n");
    }

    try
    {
        value += 1;
        int *p = (int *)0;
        *p = NULL;
    }
    catch (...)
    {
        value += 1;
        printf("except2!\n");
    }

    EXPECT_EQ(4, value);
}

TEST(SEHWithEha, SEHWithEha_3)
{
    int value = 0;
    try
    {
        value += 1;
        std::vector<int> a;
        a.push_back(1);
        a.push_back(2);
        for (auto v : a)
        {
            printf("v:%d\n", v);
        }
        int *p = (int *)1;
        *p = NULL;
    }
    catch (...)
    {
        value += 1;
        printf("except!\n");
        EXPECT_EQ(2, value);
        return;
    }
    printf("unreachable\n");
}

TEST(SEHWithEha, SEHWithEha_4)
{
    int value = 0;
    std::string s1 = "123213";
    try
    {
        value += 1;
        std::vector<int> a;
        a.push_back(1);
        a.push_back(2);
        for (auto v : a)
        {
            printf("v:%d\n", v);
        }

        std::string s2 = "22222222";
        printf("try s2=%s\n", s2.c_str());

        int *p = (int *)1;
        *p = NULL;
        value += 1;
    }
    catch (...)
    {
        value += 1;
        printf("except!\n");
        printf("except! s1=%s\n", s1.c_str());
        EXPECT_EQ(2, value);
        return;
    }
    printf("unreachable\n");
}

TEST(SEHWithEha, SEHWithEha_5)
{
    int value = 0;
    std::string s1 = "123213";
    try
    {
        value += 1;
        std::vector<int> a;
        a.push_back(1);
        a.push_back(2);
        for (auto v : a)
        {
            printf("v:%d\n", v);
        }

        value += 1;
        std::string s2 = "22222222";
        printf("try s2=%s\n", s2.c_str());

        value += 1;
        int *p = (int *)1;
        *p = NULL;
    }
    catch (...)
    {
        value += 1;
        printf("except!\n");
        printf("except! s1=%s\n", s1.c_str());
    }

    try
    {
        value += 1;
        std::vector<int> a;
        a.push_back(1);
        a.push_back(2);
        for (auto v : a)
        {
            printf("v:%d\n", v);
        }

        value += 1;
        std::string s2 = "22222222";
        printf("try2 s2=%s\n", s2.c_str());

        int *p = (int *)1;
        *p = NULL;
    }
    catch (...)
    {
        value += 1;
        printf("except2!\n");
        printf("except2! s1=%s\n", s1.c_str());
        EXPECT_EQ(7, value);
    }

    value += 1;
    EXPECT_EQ(8, value);
}

TEST(SEHWithEha, SEHWithEha_6)
{
    int value = 0;
    try
    {
        value += 1;
        printf("try 1\n");
        int *p = (int *)1;
        *p = NULL;
    }
    catch (...)
    {
        value += 1;
        printf("except!\n");
    }

    value += 1;

    try
    {
        value += 1;
        printf("try 2\n");
        int *p = (int *)1;
        *p = NULL;
    }
    catch (...)
    {
        value += 1;
        printf("except!\n");
    }

    value += 1;
    EXPECT_EQ(6, value);
}

//// https://github.com/llvm/llvm-project/issues/54922
int SEHWithEha_7_value = 0;
__declspec(noinline) void SEHWithEha_7_do_nothing() {}
__declspec(noinline) void SEHWithEha_7_test()
{
    try
    {
        SEHWithEha_7_value += 1;
        SEHWithEha_7_do_nothing();
    }
    catch (...)
    {
        SEHWithEha_7_value += 1;
        printf("hello\n");
    }
    SEHWithEha_7_value += 1;
    int *p = (int *)1;
    *p = 2;
    SEHWithEha_7_value += 1;
}

TEST(SEHWithEha, SEHWithEha_7)
{
    try
    {
        SEHWithEha_7_value += 1;
        SEHWithEha_7_test();
    }
    catch (...)
    {
        SEHWithEha_7_value += 1;
        printf("world\n");
    }

    EXPECT_EQ(4, SEHWithEha_7_value);
}

// https://github.com/llvm/llvm-project/issues/62723
int SEHWithEha_8_value = 0;
int volatile *NullPtr = 0;
DECLSPEC_NOINLINE
void
SEHWithEha_8foo()
{
    SEHWithEha_8_value += 1;
    printf("foo\n");
}

DECLSPEC_NOINLINE
void
SEHWithEha_8bar()
{
    SEHWithEha_8_value += 1;
    printf("bar\n");
}

void
SEHWithEha_8baz()
{
    try
    {
        try
        {
        }
        catch (...)
        {
            SEHWithEha_8_value += 1;
            SEHWithEha_8foo();
            *NullPtr = 1;
        }
        *NullPtr = 0;
    }
    catch (...)
    {
        SEHWithEha_8_value += 1;
        SEHWithEha_8bar();
    }
}

void
SEHWithEha_8baz2()
{
    try
    {
        try
        {
            SEHWithEha_8_value += 1;
            *NullPtr = 0;
        }
        catch (...)
        {
            SEHWithEha_8_value += 1;
            SEHWithEha_8foo();
            *NullPtr = 1;
            SEHWithEha_8_value += 1;
        }
    }
    catch (...)
    {
        SEHWithEha_8bar();
    }
}

TEST(SEHWithEha, SEHWithEha_8)
{
    printf("baz-------------\n");
    SEHWithEha_8baz();
    printf("baz2-------------\n");
    SEHWithEha_8baz2();
    EXPECT_EQ(6, SEHWithEha_8_value);
}
