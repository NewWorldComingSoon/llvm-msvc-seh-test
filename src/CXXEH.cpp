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

void
CXXEH_3_parse()
{
    throw std::exception("throw--------");
    return;
}

TEST(CXXEH, CXXEH_3)
{
    try
    {
        CXXEH_3_parse();
        return;
    }
    catch (const std::exception &e)
    {
        printf("except=%s\n", e.what());
        EXPECT_STREQ("throw--------", e.what());
        return;
    }
}

int CXXEH_4_value = 0;
void
CXXEH_4_Foo()
{
    static bool b;
    if (!b)
    {
        printf("Foo 1st time\n");
        b = true;
    }
    CXXEH_4_value += 1;
    printf("Alloca: %p\n", alloca(4));
    try
    {
        CXXEH_4_value += 1;
        throw 3;
    }
    catch (int e)
    {
        throw e;
    }
}

TEST(CXXEH, CXXEH_4)
{
    try
    {
        CXXEH_4_value += 1;
        CXXEH_4_Foo();
        printf("FOO\n");
    }
    catch (int e)
    {
        CXXEH_4_value += 1;
        printf("EXN\n");
    }

    EXPECT_EQ(4, CXXEH_4_value);
}

struct A
{
};
struct B : virtual A
{
};
struct C : virtual A
{
};
struct D : virtual A
{
};
struct E : private B, public C, private D
{
};

extern "C" void
abort();

int CXXEH_5_value = 0;
void
fne(E *e)
{
    CXXEH_5_value += 1;
    throw e;
}

void
check(E *e)
{
    try
    {
        CXXEH_5_value += 1;
        fne(e);
    }
    catch (A *p)
    {
        CXXEH_5_value += 1;
        std::cout << "caught" << std::endl;
    }
    catch (...)
    {
        CXXEH_5_value += 1;
        std::cout << "catch(...) abort" << std::endl;
        abort();
    }
    return;
}

// https://github.com/llvm/llvm-project/issues/64383
TEST(CXXEH, CXXEH_5)
{
    E e;
    check((E *)0);

    EXPECT_EQ(3, CXXEH_5_value);
}
