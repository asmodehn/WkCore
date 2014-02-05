#include "Core.hh"

#include <cstdlib>
#include <cassert>

//TODO : use cppunit
//TODO : mix with ctest framework...
#define TEST(x) assert(x);
#define TEST_FVAL(val, comp) assert( abs((val) - (comp)) < SMALL_NUMBER )

using namespace Core;

int main ( int argc, char* argv[] )
{

    ///testing now, sleep, usleep and time difference
    Time t1 = Time::now();
    Core::sleep(2);
    Time t2 = Time::now();
    double t = t2 - t1;

    //printf(" Time interval should be 2.000 seconds : %f\n", t);

    TEST ( t == 2.0 );

    ///testing clock, and it s precision
    ///pblm : expected result ?
    ///TODO


    ///test dates
    ///TODO







}
