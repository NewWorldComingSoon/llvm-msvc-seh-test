#include <gtest/gtest.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

TEST(SEH, SHE_1)
{
    int value = 0;

    __try
    {
        printf("try start\n");
        value += 1;
        __debugbreak();
        printf("try failed\n");
    }
    __except (1)
    {
        value += 1;
        printf("except seh 1\n");
    }

    value += 1;

    __try
    {
        value += 1;
        printf("try start2\n");
        __debugbreak();
        printf("try failed2\n");
    }
    __except (1)
    {
        value += 1;
        printf("except seh 2\n");
    }

    EXPECT_EQ(5, value);
}

TEST(SEH, SHE_2)
{
    int value = 0;

    __try
    {
        value += 1;
        printf("try start\n");
        _asm {int 3}
        printf("try failed\n");
    }
    __except (1)
    {
        value += 1;
        printf("except seh 1\n");
    }

    value += 1;

    __try
    {
        value += 1;
        printf("try start2\n");
        _asm {int 3}
        printf("try failed2\n");
    }
    __except (1)
    {
        value += 1;
        printf("except seh 2\n");
    }

    EXPECT_EQ(5, value);
}

TEST(SEH, SHE_3)
{
    int value = 0;
    __try
    {
        value += 1;
        int *p = (int *)1;
        *p = NULL;
    }
    __except (1)
    {
        value += 1;
        printf("except!\n");
    }

    EXPECT_EQ(2, value);
}

