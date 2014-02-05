#ifndef TESTCOMMON_HH
#define TESTCOMMON_HH

#include <cstdlib>
#include <cassert>

#include <cstdio>

/// Common macros used in all tests
//TODO : use cppunit
//TODO : mix with ctest framework...
//TODO : get inspiration from boost/detail/lightweight test
#define TEST(x) assert( (x) );
#define TEST_FVAL(val, comp) assert( abs((val) - (comp)) < SMALL_NUMBER )

///Common classes used in all tests
#include "Logger.hh"

///Common functions used in all tests
///Accessor to the error counter
inline int& testErr()
{
    static int err = 0;
    return err;
}

///Checking error counter
inline int reportErrors(Core::Logger & l)
{
    int errors = testErr();

    if( errors == 0 )
    {
        l << Core::nl << "No errors detected.";
        return 0;
    }
    else
    {
        l << Core::nl <<  errors << " error(s) detected.";
        return 1;
    }
}

inline int reportErrors()
{
    int errors = testErr();

    if( errors == 0 )
    {
        std::clog << "No errors detected.";
        return 0;
    }
    else
    {
        std::clog <<  errors << " error(s) detected.";
        return 1;
    }
}

#endif
