#include <gtest/gtest.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

TEST(SEH, SEH_1)
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

TEST(SEH, SEH_2)
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

TEST(SEH, SEH_3)
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

TEST(SEH, SEH_4)
{
    int value = 0;
    __try
    {
        value += 1;
        printf("try 1\n");
        int *p = (int *)1;
        *p = NULL;
    }
    __except (1)
    {
        value += 1;
        printf("except!\n");
        // seh的返回没有问题
        EXPECT_EQ(2, value);
        return;
    }
    printf("unreachable!\n");
    __debugbreak();
}

//// https://github.com/llvm/llvm-project/issues/54922
int SEH_5_value = 0;
__declspec(noinline) void SEH_5_do_nothing() {}
__declspec(noinline) void SEH_5_test()
{
    __try
    {
        SEH_5_value += 1;
        SEH_5_do_nothing();
    }
    __except (1)
    {
        SEH_5_value += 1;
        printf("hello\n");
    }
    SEH_5_value += 1;
    __debugbreak();
    SEH_5_value += 1;
}

TEST(SEH, SEH_5)
{
    __try
    {
        SEH_5_value += 1;
        SEH_5_test();
    }
    __except (1)
    {
        SEH_5_value += 1;
        printf("world\n");
    }

    EXPECT_EQ(4, SEH_5_value);
}

//// https://github.com/llvm/llvm-project/issues/63706
int SEH_6_value = 0;
void
SEH_6_run(int variable)
{
    __try
    {
        SEH_6_value += 1;
        printf("3333\n");
        RaiseException(0xE0000001, 0, 0, 0);
    }

    __except ([variable](unsigned int code, struct _EXCEPTION_POINTERS *ep) -> int {
        int copy = variable;
        printf("copy=%d\n", copy);
        SEH_6_value += 1;
        return EXCEPTION_EXECUTE_HANDLER;
    }(GetExceptionCode(), GetExceptionInformation()))
    {
        SEH_6_value += 1;
        printf("2222\n");
    }
    SEH_6_value += 1;
}

TEST(SEH, SEH_6)
{
    SEH_6_value += 1;
    SEH_6_run(11);
    EXPECT_EQ(5, SEH_6_value);
}
