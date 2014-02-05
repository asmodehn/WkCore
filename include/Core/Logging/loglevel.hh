#ifndef CORE_LOGLEVEL_HH
#define CORE_LOGLEVEL_HH

//TODO
// we ned to keep an enum
// we just need to add proper operators around it
//http://www.edm2.com/0405/enumeration.html

#include <ostream>

namespace Core
{

namespace loglevel
{


enum Level
{
    Core_LogFatal = 0,
    Core_LogError = 1,
    Core_LogWarning = 2,
    Core_LogInfo = 3,
    Core_LogDebug = 4
};


//beware ++ means more important that is opposite in integer value in enums
//logleve can not be more important than FATAL (=0)
inline void operator++(Level& lvl)
{
    lvl = (lvl == Core_LogFatal)?Core_LogFatal:Level(lvl-1);
}
inline void operator++(Level& lvl, int)
{
    lvl = (lvl == Core_LogFatal)?Core_LogFatal:Level(lvl-1);
}
//loglevel can be as uninteressant as we want.
inline void operator--(Level& lvl)
{
    lvl = Level(lvl+1);
}
inline void operator--(Level& lvl, int)
{
    lvl = Level(lvl+1);
}

std::ostream& operator<<( std::ostream& os, const Level& lvl );

} //loglevel



} //Core



#endif
