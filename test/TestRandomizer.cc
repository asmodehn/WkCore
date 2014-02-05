#include "Randomizer.hh"

#include "TestCommon.hh"

#include "Logger.hh"
#include "Time.hh"

#include <vector>
#include <iomanip>

///Test Ramdomizer depends on Time to time the period of test
/// And on Logger to store informations for visual check later.

using namespace Core;

Logger testlog("Randomizer");

bool loop_test_getuc (int nbtimes)
{
    testlog << nl << "Setting up table of results...";
    std::vector<unsigned char> table;
    table.reserve(nbtimes);

    unsigned char bottom, top, temp, val;
    std::numeric_limits<unsigned char> uc_limits;

    testlog << nl << "Preparing bounds...";
    bottom = Randomizer<unsigned char>::get(uc_limits.min(), uc_limits.max());
    top = Randomizer<unsigned char>::get(uc_limits.min(), uc_limits.max());
    if ( bottom >= top )
    {
        temp = bottom;    //swap if needed
        bottom= top;
        top = temp;
    }

    testlog << nl << "Looping " << nbtimes << " for random values... ";
    std::string backstr( nbtimes / 10 +1 , '\b' );
    for (int i=0; i < nbtimes; ++i)
    {
        //TODO : fix display
        testlog << std::setw( nbtimes / 10 ) << i;
        testlog.flush() ;
        val = Randomizer<unsigned char>::get(bottom, top);
        //store value for study later on.
        table.push_back(val);
        testlog << backstr;
    }
    testlog << "Done.";

    testlog << nl << "Checking for out of bounds values";
    std::vector<unsigned char>::iterator it = table.begin();
    while ( it != table.end() )
    {
        if ( *it < bottom || *it > top )
        {
            testlog<< nl << "ERROR : " << *it << " is out of [ " << bottom << "," << top << " ) bounds !";
            testErr()++;
        }
        it++;
    }


    //TODO : store some statistics on a log for visual check...
    return reportErrors(testlog) == 0;
}



int main (int argc, char * argv [] )
{
///We need to make sure the randomizer returns proper values.

///TODO Testing for some amount of time...
//for now:
    int nbloop = 1000;

    testlog.enableFileLog("TestRandomizer.log");

    testlog<< nl << "Testing unsigned char :" ;
    bool success = loop_test_getuc(nbloop);

    testlog << nl << "TODO : test other built in types";

    return (success)? 0 : -1;
}
