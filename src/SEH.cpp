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
    int *p = (int *)1;
    *p = 1;
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

int SEH_7_value = 0;

DECLSPEC_NOINLINE
int
SEH_7_seh_filter(unsigned int code, int variable, int variable2)
{
    SEH_7_value += 1;
    printf("code=%x\n", code);
    printf("variable=%x\n", variable);
    printf("variable2=%x\n", variable2);
    if (code == 1)
    {
        return -1;
    }
    return 1;
}

TEST(SEH, SEH_7)
{
    SEH_7_value += 1;
    __try
    {
        puts(" in try");

        SEH_7_value += 1;
        RaiseException(0xE0000001, 0, 0, 0);
    }
    __except (SEH_7_seh_filter(GetExceptionCode(), 3, 4))
    {
        SEH_7_value += 1;
        puts(" in except");
    }
    SEH_7_value += 1;

    EXPECT_EQ(5, SEH_7_value);
}

int g_SEH_8_a = 0;
int g_SEH_8_b = 3359;

TEST(SEH, SEH_8)
{
    int value = 0;

    int a = g_SEH_8_a, b = g_SEH_8_b;

    if (a == 2)
    {
        puts(" in try");
    }

    __try
    {
        value += 1;
        if (b == 4)
        {
            b = 5;
        }
        else
        {
        }
        value += 1;
    }
    __except (1)
    {
    }
    value += 1;
    EXPECT_EQ(3, value);
}

int g_SEH_9 = 0;

int
SEH_9_nested___finally___finally_with_eh_edge(void)
{
    __try
    {
        __try
        {
            g_SEH_9 += 1;
            printf("123\n");
        }
        __finally
        {
            printf("456\n");
            g_SEH_9 += 1;
            return 899;
        }
    }
    __finally
    {
        g_SEH_9 += 1;
        // Intentionally no return here.
        printf("11111\n");
    }

    printf("789\n");
    g_SEH_9 += 1;
    return 912;
}

TEST(SEH, SEH_9)
{
    int x = SEH_9_nested___finally___finally_with_eh_edge();
    printf("x=%d\n", x);

    EXPECT_EQ(3, g_SEH_9);
}

int g_SEH_10 = 0;
void
f1()
{
    g_SEH_10 += 1;
    printf("f1\n");
}
void
f2()
{
    g_SEH_10 += 1;
    printf("f2\n");
}

void
f3()
{
    g_SEH_10 += 1;
    printf("f3\n");
}

void
f(int z)
{
    __try
    {
        f1();
    }
    __finally
    {
        f2();
        if (z)
            return;
        f3();
    }
}

TEST(SEH, SEH_10)
{
    f(2);

    EXPECT_EQ(2, g_SEH_10);
}

int g_SEH_11 = 0;
int
SEH_11_myadd(int a, int b)
{
    int ret = 0;

    __try
    {
        g_SEH_11 += 1;

        __debugbreak();
    }
    __except (1)
    {
        if (a < b)
        {
            ret = a + b + 3;
        }
        else if (a > 4)
        {
            ret = a - b + 8;
        }
        else
        {
            g_SEH_11 += 1;
            ret = a - b + 4;
        }
    }

    return ret;
}

TEST(SEH, SEH_11)
{
    SEH_11_myadd(-1, -2);

    EXPECT_EQ(2, g_SEH_11);
}
