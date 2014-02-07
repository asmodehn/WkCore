#include "Core/Logging/logstream.hh"

namespace Core
{

logstream::logstream ()
        : std::ostringstream()
{
    //to hook up to usual stream design
    pvm_lsb = new clogstreambuf();
    this->init(pvm_lsb);

    //setup default flags
    this->flags ( std::ios::left | std::ios::hex | std::ios::showbase | std::ios::boolalpha );

    //setup default prefix
    resetprefix();

}

logstream::logstream (logstreambuf* lsb)
        : std::ostringstream()
{
    //to hook up to usual stream design
    pvm_lsb = lsb;
    this->init(pvm_lsb);

    //setup default flags
    this->flags ( std::ios::left | std::ios::hex | std::ios::showbase | std::ios::boolalpha );

    //setup default prefix
    resetprefix();

}

logstream::~logstream()
{
}

//to manage prefix
void logstream::resetprefix(const std::string& newprefix)
{
    rdbuf()->resetprefix(newprefix);
}

const std::string & logstream::getprefix() const
{
    return rdbuf()->getprefix();
}


logstream& operator<<(logstream &ls, loglevel::Level lvl)
{
    if ( ls.getLevel() >= lvl )
    {
        ls.rdbuf()->filterin();
        //dynamic casting to call the ostream << ( ostream, loglevel) operator
        dynamic_cast<std::ostream&>(ls) << lvl;
    }
    else
    {
        ls.rdbuf()->filterout();
    }
    return ls ;
}

logstream& logstream::level(loglevel::Level l)
{
    *this << l;
    return *this;
}

} // Core


