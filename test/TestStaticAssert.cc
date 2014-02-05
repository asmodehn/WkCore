#include "StaticAssert.hh"

int main(int argc, char* argv[])
{
    StaticAssert(true,YEAH);

// Uncomment this to generate an error on compile time
//    StaticAssert(0,BOUH);

    return 0;
}
