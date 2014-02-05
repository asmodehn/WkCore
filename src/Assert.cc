#include "Assert.hh"

#include <cstdio>

void assertion_failed(char const * expr, char const * func, char const * file, long line)
{
    printf("%s::%li in %s --> Core Assertion FAILED : %s", file, line, func, expr);
}
