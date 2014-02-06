#ifndef NEWLOGGER_HH
#define NEWLOGGER_HH

#include "Core/Logging/logstream.hh"

//to be able to access cnull
#include "Core/Logging/nullstream.hh"


namespace Core
{

/***
 *
 * \class NewLogger
 * \brief NewLogger manages the Logging outputs for the application.
 *
 */

class NewLogger : public logstream
{
public:
    NewLogger();
    ~NewLogger();

    ///by default outputs to std::clog
    //enable more Sinks
    bool enableFileLog(const std::string & filename);
    //enableSysLog();
    //enableWinLog();

    void disableFileLog();
    //disableSysLog();
    //disableWinLog();


    //accessors to logstreams ( to set separatelevels for example )
    logstream& getFileLog();
    //logstream& getSysLog();
    //logstream& getWinLog();

    template<typename M>
    NewLogger& operator << (const M & msg);
    //to enable manipulators on Logger
    NewLogger& operator << (std::ostream& (*manip)(std::ostream&));
    NewLogger& operator << (std::ios_base& (*manip)(std::ios_base&));
    NewLogger& operator << (NewLogger& (*manip)(NewLogger&));//to enable specific manipulators on Logger

    friend NewLogger& operator<<(NewLogger &log, loglevel::Level lvl);

    void setFilelogLevel(loglevel::Level lvl);


private :
    //list of logstreambuf. Each logstreambuf provide a different ouput to logstream
    logstreambuf* pvm_filelsb;
    //logstreambuf pvm_syslsb;
    //logstreambuf pvm_winlsb;

protected :
    //list of logstream. Each logstream has one output and only one
    logstream* ptm_filelogstream;
    //logstream ptm_syslogstream;
    //logstream ptm_winlogstream;
};

template<typename M>
NewLogger& NewLogger::operator<< ( const M & msg)
{
    /*
    if (_consoleLog)
        std::clog << msg;
    if (_fileLog)
        _ofstr << msg;
        */
    return *this;
}


}


#endif
