#include "Core/Logging/loglevel.hh"

namespace Core
{
namespace loglevel
{
std::ostream& operator<<( std::ostream& os, const Level& lvl )
{
    switch ( lvl )
    {
    case Core_LogFatal:
        os << "FTL";
        break;
    case Core_LogError:
        os << "ERR";
        break;
    case Core_LogWarning:
        os << "WRN";
        break;
    case Core_LogInfo:
        os << "INF";
        break;
    case Core_LogDebug:
        os << "DBG";
        break;
    }
    return os;
}

} //loglevel
}//Core
