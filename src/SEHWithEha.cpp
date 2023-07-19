#include <gtest/gtest.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

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
