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

    int fadapt1 (  void* v )
    {
        return m_testval;
    }
    int fadapt1 (  void* v ) const
    {
        return m_testval;
    }
    int fadapt2 ( int a, void* v)
    {
        return m_testval+a;
    }
    int fadapt2 ( int a, void* v) const
    {
        return m_testval+a;
    }
    int fadapt3 ( int a, int b, void* v )
    {
        return m_testval+a+b;
    }
    int fadapt3 ( int a, int b, void* v ) const
    {
        return m_testval+a+b;
    }
    int fadapt4 ( int a, int b, int c,void* v )
    {
        return m_testval+a+b+c;
    }
    int fadapt4 ( int a, int b, int c,void* v ) const
    {
        return m_testval+a+b+c;
    }
    int fadapt5 ( int a, int b, int c,int d,void* v )
    {
        return m_testval+a+b+c+d;
    }
    int fadapt5 ( int a, int b, int c,int d,void* v ) const
    {
        return m_testval+a+b+c+d;
    }
    int fadapt6 ( int a, int b, int c,int d,int e,void* v )
    {
        return m_testval+a+b+c+d+e;
    }
    int fadapt6 ( int a, int b, int c,int d,int e,void* v ) const
    {
        return m_testval+a+b+c+d+e;
    }
};

///defining data used

std::auto_ptr<Core::Callback1Base<void*,int> > adtest1;
std::auto_ptr<Core::Callback1constBase<void*,int> > adtest1c;
std::auto_ptr<Core::Callback2Base<int,void*,int> > adtest2;
std::auto_ptr<Core::Callback2constBase<int,void*,int> > adtest2c;
std::auto_ptr<Core::Callback3Base<int,int,void*,int> > adtest3;
std::auto_ptr<Core::Callback3constBase<int,int,void*,int> > adtest3c;
std::auto_ptr<Core::Callback4Base<int,int,int,void*,int> > adtest4;
std::auto_ptr<Core::Callback4constBase<int,int,int,void*,int> > adtest4c;
std::auto_ptr<Core::Callback5Base<int,int,int,int,void*,int> > adtest5;
std::auto_ptr<Core::Callback5constBase<int,int,int,int,void*,int> > adtest5c;
std::auto_ptr<Core::Callback6Base<int,int,int,int,int,void*,int> > adtest6;
std::auto_ptr<Core::Callback6constBase<int,int,int,int,int,void*,int> > adtest6c;

std::auto_ptr<Core::Callback1Base<void*,int> > adtestcopy;


//assumed legacy invoker always have a ending void* parameter, to be able to pass any argument number to the callback
int legacy1invoker( int (*func) ( void*), void* v)
{
    return  func ( v ) ;
}
int legacy2invoker( int (*func) ( int, void*) , int a, void*v)
{
    return func ( a, v);
}
int legacy3invoker( int (*func) ( int, int, void*), int a, int b, void* v)
{
    return func ( a ,b , v) ;
}
int legacy4invoker( int (*func) ( int, int, int, void*), int a, int b, int c, void* v)
{
    return func ( a ,b , c, v) ;
}
int legacy5invoker( int (*func) ( int, int, int, int, void*), int a, int b, int c, int d, void* v)
{
    return func ( a ,b , c, d, v) ;
}
int legacy6invoker( int (*func) ( int, int, int, int, int, void*), int a, int b, int c, int d, int e, void* v)
{
    return func ( a ,b , c, d, e, v) ;
}

///TODO : multiple C callbacks through multiple adaptors
///TODO : delayed C callbacks through adaptor

int main( int argc, char * argv [] )
{

    Called cobj;

///1 argument
    adtest1.reset(new Callback1<Called,void*, int>(cobj,&Called::fadapt1) );
    Adapter1<int> ad1(adtest1.get());
    TEST(legacy1invoker(Adapter1<int>::adapt,ad1.build_voidP_args(NULL)) == 1 );

    //the const verion
    adtest1c.reset(new Callback1const<Called,void*, int>(cobj,&Called::fadapt1) );
    Adapter1const<int> ad1c(adtest1c.get());
    TEST(legacy1invoker(Adapter1const<int>::adapt,ad1c.build_voidP_args(NULL)) == 1 );

///2 arguments
    adtest2.reset(new Callback2<Called,int,void*,int>(cobj,&Called::fadapt2) );
    Adapter2<int,int> ad2(adtest2.get());
    TEST(legacy2invoker(Adapter2<int,int>::adapt,1,ad2.build_voidP_args(NULL)) == 2 );

    //the const verion
    adtest2c.reset(new Callback2const<Called,int,void*,int>(cobj,&Called::fadapt2) );
    Adapter2const<int,int> ad2c(adtest2c.get());
    TEST(legacy2invoker(Adapter2const<int,int>::adapt,1,ad2c.build_voidP_args(NULL)) == 2 );


///3 arguments
    adtest3.reset(new Callback3<Called,int,int,void*,int>(cobj,&Called::fadapt3) );
    Adapter3<int,int,int> ad3(adtest3.get());
    TEST(legacy3invoker(Adapter3<int,int,int>::adapt,1,2,ad3.build_voidP_args(NULL)) == 4 );

    //the const verion
    adtest3c.reset(new Callback3const<Called,int,int,void*,int>(cobj,&Called::fadapt3) );
    Adapter3const<int,int,int> ad3c(adtest3c.get());
    TEST(legacy3invoker(Adapter3const<int,int,int>::adapt,1,2,ad3c.build_voidP_args(NULL)) == 4 );

///4 arguments
    adtest4.reset(new Callback4<Called,int,int,int,void*,int>(cobj,&Called::fadapt4) );
    Adapter4<int,int,int,int> ad4(adtest4.get());
    TEST(legacy4invoker(Adapter4<int,int,int,int>::adapt,1,2,3,ad4.build_voidP_args(NULL)) == 7 );

    //the const verion
    adtest4c.reset(new Callback4const<Called,int,int,int,void*,int>(cobj,&Called::fadapt4) );
    Adapter4const<int,int,int,int> ad4c(adtest4c.get());
    TEST(legacy4invoker(Adapter4const<int,int,int,int>::adapt,1,2,3,ad4c.build_voidP_args(NULL)) == 7 );

///5 arguments
    adtest5.reset(new Callback5<Called,int,int,int,int,void*,int>(cobj,&Called::fadapt5) );
    Adapter5<int,int,int,int,int> ad5(adtest5.get());
    TEST(legacy5invoker(Adapter5<int,int,int,int,int>::adapt,1,2,3,4,ad5.build_voidP_args(NULL)) == 11 );

    //the const verion
    adtest5c.reset(new Callback5const<Called,int,int,int,int,void*,int>(cobj,&Called::fadapt5) );
    Adapter5const<int,int,int,int,int> ad5c(adtest5c.get());
    TEST(legacy5invoker(Adapter5const<int,int,int,int,int>::adapt,1,2,3,4,ad5c.build_voidP_args(NULL)) ==11 );

///6 arguments
    adtest6.reset(new Callback6<Called,int,int,int,int,int,void*,int>(cobj,&Called::fadapt6) );
    Adapter6<int,int,int,int,int,int> ad6(adtest6.get());
    TEST(legacy6invoker(Adapter6<int,int,int,int,int,int>::adapt,1,2,3,4,5,ad6.build_voidP_args(NULL)) == 16 );

    //the const verion
    adtest6c.reset(new Callback6const<Called,int,int,int,int,int,void*,int>(cobj,&Called::fadapt6) );
    Adapter6const<int,int,int,int,int,int> ad6c(adtest6c.get());
    TEST(legacy6invoker(Adapter6const<int,int,int,int,int,int>::adapt,1,2,3,4,5,ad6c.build_voidP_args(NULL)) ==16 );

///Testing that Adapter survives copy (useful when used in threads)
	///DOES NOT WORK 
	//adtestcopy.reset(new Callback1<Called,void*, int>(cobj,&Called::fadapt1) );
    //Adapter1<int>* adptr = new Adapter1<int>(adtestcopy.get());
	//void* args = adptr->build_voidP_args(NULL);
	//delete adptr,adptr=0;
    //TEST(legacy1invoker(Adapter1<int>::adapt,args) == 1 );
	
    return 0;
}

