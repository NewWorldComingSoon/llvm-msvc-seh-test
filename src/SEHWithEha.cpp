#include <gtest/gtest.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

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

