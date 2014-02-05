#include "Functor.hh"

#include "TestCommon.hh"

int func0 ( )
{
    return 1;
}
int func1 ( int a )
{
    return 1+a;
}
int func2 ( int a, int b)
{
    return 1+a+b;
}
int func3 ( int a, int b, int c)
{
    return 1+a+b+c;
}
int func4 ( int a, int b, int c, int d)
{
    return 1+a+b+c+d;
}


class Obj
{
    int m_testval;
public:
    Obj() : m_testval(1)
    {}
public:
    int func0 ( )
    {
        return m_testval;
    }
    int cfunc0 ( ) const
    {
        return m_testval;
    }
    int func1 ( int a)
    {
        return m_testval+a;
    }
    int cfunc1 ( int a) const
    {
        return m_testval+a;
    }
    int func2 ( int a, int b)
    {
        return m_testval+a+b;
    }
    int cfunc2 ( int a, int b) const
    {
        return m_testval+a+b;
    }
    int func3 ( int a, int b, int c)
    {
        return m_testval+a+b+c;
    }
    int cfunc3 ( int a, int b, int c) const
    {
        return m_testval+a+b+c;
    }
    int func4 (int a, int b, int c, int d)
    {
        return m_testval+a+b+c+d;
    }
    int cfunc4 (int a, int b, int c, int d) const
    {
        return m_testval+a+b+c+d;
    }
    int func5 (int a, int b, int c, int d, int e)
    {
        return m_testval+a+b+c+d+e;
    }
    int cfunc5 (int a, int b, int c, int d, int e) const
    {
        return m_testval+a+b+c+d+e;
    }
    int func6 (int a, int b, int c, int d, int e, int f)
    {
        return m_testval+a+b+c+d+e+f;
    }
    int cfunc6 (int a, int b, int c, int d, int e, int f) const
    {
        return m_testval+a+b+c+d+e+f;
    }

};

int main ( int argc, char * argv [] )
{
    Obj o;

    //Testing direct call of member function pointer

    // 0 arguments
    TEST( (std::mem_fun(&Obj::func0))(&o) == 1 );
    TEST( (std::mem_fun_ref(&Obj::func0))(o) == 1 );
    //const
    TEST( (std::mem_fun(&Obj::cfunc0))(&o) == 1 );
    TEST( (std::mem_fun_ref(&Obj::cfunc0))(o) == 1 );

    // 1 arguments
    TEST( (std::mem_fun(&Obj::func1))(&o,2) == 3 );
    TEST( (std::mem_fun_ref(&Obj::func1))(o,2) == 3 );
    //const
    TEST( (std::mem_fun(&Obj::cfunc1))(&o,2) == 3 );
    TEST( (std::mem_fun_ref(&Obj::cfunc1))(o,2) == 3 );

    // 2 arguments
    TEST( (Core::stdext::mem_fun(&Obj::func2))(&o,2,3) == 6 );
    TEST( (Core::stdext::mem_fun_ref(&Obj::func2))(o,2,3) == 6 );
    //const
    TEST( (Core::stdext::mem_fun(&Obj::cfunc2))(&o,2,3) == 6 );
    TEST( (Core::stdext::mem_fun_ref(&Obj::cfunc2))(o,2,3) == 6 );

    // 3 arguments
    TEST( (Core::stdext::mem_fun(&Obj::func3))(&o,2,3,4) == 10 );
    TEST( (Core::stdext::mem_fun_ref(&Obj::func3))(o,2,3,4) == 10 );
    //const
    TEST( (Core::stdext::mem_fun(&Obj::cfunc3))(&o,2,3,4) == 10 );
    TEST( (Core::stdext::mem_fun_ref(&Obj::cfunc3))(o,2,3,4) == 10 );

    // 4 arguments
    TEST( (Core::stdext::mem_fun(&Obj::func4))(&o,2,3,4,5) == 15 );
    TEST( (Core::stdext::mem_fun_ref(&Obj::func4))(o,2,3,4,5) == 15 );
    //const
    TEST( (Core::stdext::mem_fun(&Obj::cfunc4))(&o,2,3,4,5) == 15 );
    TEST( (Core::stdext::mem_fun_ref(&Obj::cfunc4))(o,2,3,4,5) == 15 );

    // 5 arguments
    TEST( (Core::stdext::mem_fun(&Obj::func5))(&o,2,3,4,5,6) == 21 );
    TEST( (Core::stdext::mem_fun_ref(&Obj::func5))(o,2,3,4,5,6) == 21 );
    //const
    TEST( (Core::stdext::mem_fun(&Obj::cfunc5))(&o,2,3,4,5,6) == 21 );
    TEST( (Core::stdext::mem_fun_ref(&Obj::cfunc5))(o,2,3,4,5,6) == 21 );

    // 6 arguments
    TEST( (Core::stdext::mem_fun(&Obj::func6))(&o,2,3,4,5,6,7) == 28 );
    TEST( (Core::stdext::mem_fun_ref(&Obj::func6))(o,2,3,4,5,6,7) == 28 );
    //const
    TEST( (Core::stdext::mem_fun(&Obj::cfunc6))(&o,2,3,4,5,6,7) == 28 );
    TEST( (Core::stdext::mem_fun_ref(&Obj::cfunc6))(o,2,3,4,5,6,7) == 28 );

    return 0;
}
