/*
 Test program for Core Logger
*/

#include "Core.hh"
#include <string>
using namespace Core;

int testlogstream(logstream & ls)
{
    ///test logstream default setup
    /// date insertion
    /// default "" prefix
    /// default loglevel

    ///test logstream date insertion
    ls << "test";
    //TODO
    //ls.str() == date + "test"
    ls << std::endl;

    ///test logstream custom prefix
    ls.resetprefix("LogTest :");
    ls << "test";
    //TODO
    //ls.str() == date + prefix + "test"
    ls <<std::endl;
    ls.resetprefix();

    ///test logstream and loglevel
    ls.resetLevel(loglevel::Core_LogInfo);
    ls << loglevel::Core_LogDebug << "Debug message" << std::endl; //ignored
    ls << loglevel::Core_LogInfo << "Info message" << std::endl; // ok
    ls << loglevel::Core_LogWarning << "Warning message" << std::endl; //ok
    ls << loglevel::Core_LogError << "Error message" << std::endl; //ok
    ls << loglevel::Core_LogFatal << "Fatal message" << std::endl; //ok

    ls.resetLevel(loglevel::Core_LogError);


    ls << loglevel::Core_LogDebug << "Debug message" << std::endl; //ignored
    ls << loglevel::Core_LogInfo<< "Info message" << std::endl; //ignored
    ls << loglevel::Core_LogWarning << "Warning message" << std::endl; //ignored
    ls << loglevel::Core_LogError << "Error message" << std::endl; //ok
    ls << loglevel::Core_LogFatal << "Fatal message" << std::endl; //ok


    return 0;

}



int main(int argc, char *argv[])
{
    //this should not go anywhere or do anything
    cnull << "NullStream should not be visible !" << std::endl;

    int res = 0;

    clogstreambuf clsb;
    logstream ls(&clsb);

    res += testlogstream(ls);

    filelogstreambuf flsb("MytestLog.txt");
    logstream fls(&flsb);

    res += testlogstream(fls);





    //Test NewLogger
    NewLogger log;
    log.enableFileLog("newlogger.txt");
    res += testlogstream(log);



    return res;
}

