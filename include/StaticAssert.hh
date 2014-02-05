#ifndef STATICASSERT_HH
#define STATICASSERT_HH

namespace misc
{
template <bool> struct compile_time_check;

template <> struct compile_time_check<true> {};

}

/*
   Similiar to assert, StaticAssert is only in operation when NDEBUG is not
   defined. It will test its first argument at compile time and on failure
   report the error message of the second argument, which must be a valid c++
   classname. i.e. no spaces, punctuation or reserved keywords.
   */

#ifndef NDEBUG
#   define StaticAssert1(test, errormsg)                         \
    do {                                                        \
        typedef misc::compile_time_check< (test) != 0 > Static_Assertion_Failed; \
        Static_Assertion_Failed ERROR_##errormsg;		\
sizeof(ERROR_##errormsg);                                          \
} while (0)
#else
#   define StaticAssert1(test, errormsg)                         \
    do {} while (0)
#endif

//To be able to choose a strategy
#define StaticAssert(test, errormsg) StaticAssert1(test, errormsg)

#endif // STATICASSERT_HH
