#include "Callback.hh"
#include "Adapter.hh"

#include "TestCommon.hh"

#include <iostream>

using namespace Core;

#include <memory> // for auto_ptr

///defining classes used
class Called
{

    int m_testval;
public:
    Called() : m_testval(1)
    {}
public:
    int func0 ( )
    {
        return m_testval;
    }
    int func0 ( ) const
    {
        return m_testval;
    }
    int func1 (  int a )
    {
        return m_testval+a;
    }
    int func1 (  int a ) const
    {
        return m_testval+a;
    }
    int func2 ( int a, int b)
    {
        return m_testval+a+b;
    }
    int func2 ( int a, int b) const
    {
        return m_testval+a+b;
    }
    int func3 ( int a, int b, int c )
    {
        return m_testval+a+b+c;
    }
    int func3 ( int a, int b, int c ) const
    {
        return m_testval+a+b+c;
    }
    int func4 ( int a, int b, int c, int d )
    {
        return m_testval+a+b+c+d;
    }
    int func4 ( int a, int b, int c,int d ) const
    {
        return m_testval+a+b+c+d;
    }
    int func5 ( int a, int b, int c, int d , int e)
    {
        return m_testval+a+b+c+d+e;
    }
    int func5 ( int a, int b, int c,int d ,int e) const
    {
        return m_testval+a+b+c+d+e;
    }

};

///defining data used
std::auto_ptr<Core::Callback0Base<int> > cbtest0;
std::auto_ptr<Core::Callback0constBase<int> > cbtest0c;
std::auto_ptr<Core::Callback1Base<int,int> > cbtest1;
std::auto_ptr<Core::Callback1constBase<int,int> > cbtest1c;
std::auto_ptr<Core::Callback2Base<int,int,int> > cbtest2;
std::auto_ptr<Core::Callback2constBase<int,int,int> > cbtest2c;
std::auto_ptr<Core::Callback3Base<int,int,int,int> > cbtest3;
std::auto_ptr<Core::Callback3constBase<int,int,int,int> > cbtest3c;
std::auto_ptr<Core::Callback4Base<int,int,int,int,int> > cbtest4;
std::auto_ptr<Core::Callback4constBase<int,int,int,int,int> > cbtest4c;
std::auto_ptr<Core::Callback5Base<int,int,int,int,int,int> > cbtest5;
std::auto_ptr<Core::Callback5constBase<int,int,int,int,int,int> > cbtest5c;


int main( int argc, char * argv [] )
{

    Called cobj;
    //NB : cobj still exist when all the callbacks are called
    // cobj is passed as a const reference in the following, and not copied.

    cbtest0.reset( new Callback0<Called,int>(cobj,&Called::func0) );
    TEST((*cbtest0)() == 1 );

    cbtest0c.reset( new Callback0const<Called,int>(cobj,&Called::func0) );
    TEST((*cbtest0)() == 1 );

    cbtest1.reset(new Callback1<Called,int, int>(cobj,&Called::func1) );
    TEST ( (*cbtest1)( 1) == 2 );

    cbtest1c.reset(new Callback1const<Called,int, int>(cobj,&Called::func1) );
    TEST ( (*cbtest1)( 1) == 2 );

    cbtest2.reset( new Callback2<Called,int,int, int>(cobj, &Called::func2) );
    TEST( (*cbtest2)(1,2) == 4);

    cbtest2c.reset( new Callback2const<Called,int,int, int>(cobj, &Called::func2) );
    TEST( (*cbtest2)(1,2) == 4);

    cbtest3.reset( new Callback3<Called,int,int,int,int>(cobj, &Called::func3) );
    TEST( (*cbtest3)(1,2,3) == 7) ;

    cbtest3c.reset( new Callback3const<Called,int,int,int,int>(cobj, &Called::func3) );
    TEST( (*cbtest3)(1,2,3) == 7) ;

    cbtest4.reset( new Callback4<Called,int,int,int,int,int>(cobj, &Called::func4) );
    TEST( (*cbtest4)(1,2,3,4) == 11) ;

    cbtest4c.reset( new Callback4const<Called,int,int,int,int,int>(cobj, &Called::func4) );
    TEST( (*cbtest4)(1,2,3,4) == 11) ;

    return 0;
};
