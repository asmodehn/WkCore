#include "Core/NewLogger.hh"

namespace Core
{


NewLogger::NewLogger()
        : logstream(), //default clog output
        pvm_filelsb(0),ptm_filelogstream(0)
{

}

NewLogger::~NewLogger()
{
    disableFileLog();
}

bool NewLogger::enableFileLog(const std::string & filename)
{
    if (ptm_filelogstream != 0 || pvm_filelsb != 0)
    {
        *ptm_filelogstream << loglevel::Core_LogWarning << " Logfile changed to " << filename << std::endl;
        disableFileLog(); //only one file log at once
    }
    pvm_filelsb = new filelogstreambuf(filename);
    if (pvm_filelsb) ptm_filelogstream = new logstream(pvm_filelsb);
    else
    {
        disableFileLog();
    }

    return (ptm_filelogstream != 0);
}

void NewLogger::disableFileLog()
{
    if (ptm_filelogstream !=0) delete ptm_filelogstream, ptm_filelogstream = 0;
    if (pvm_filelsb !=0) delete pvm_filelsb, pvm_filelsb = 0;
}


void  NewLogger::setFilelogLevel(loglevel::Level lvl)
{
    ptm_filelogstream->resetLevel(lvl);
}

NewLogger& operator<<(NewLogger &log, loglevel::Level lvl)
{
    dynamic_cast<logstream&>(log) << lvl;
    *(log.ptm_filelogstream) << lvl;
    return log;
}



}
