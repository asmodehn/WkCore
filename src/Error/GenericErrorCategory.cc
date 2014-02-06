#include "Core/Error/ErrorCategory.hh"

#include <cstring> // for strerror/strerror_r

namespace Core
{
namespace Error
{

const char * GenericErrorCategory::name() const
{
    return "generic";
}

std::string GenericErrorCategory::message( int ev ) const
{
static std::string unknown_err( "Unknown error" );
// strerror_r is preferred because it is always thread safe,
// however, we fallback to strerror in certain cases because:
//   -- Windows doesn't provide strerror_r.
//   -- HP and Sun do provide strerror_r on newer systems, but there is
//      no way to tell if is available at runtime and in any case their
//      versions of strerror are thread safe anyhow.
//   -- Linux only sometimes provides strerror_r.
//   -- Tru64 provides strerror_r only when compiled -pthread.
//   -- VMS doesn't provide strerror_r, but on this platform, strerror is
//      thread safe.
# if defined(BOOST_WINDOWS_API) || defined(__hpux) || defined(__sun)\
    || (defined(__linux) && (!defined(__USE_XOPEN2K) || defined(BOOST_SYSTEM_USE_STRERROR)))\
    || (defined(__osf__) && !defined(_REENTRANT))\
    || (defined(__INTEGRITY))\
    || (defined(__vms))\
    || (defined(__QNXNTO__))
    const char * c_str = std::strerror( ev );
    return  c_str
    ? std::string( c_str )
    : unknown_err;
# else  // use strerror_r
    char buf[64];
    char * bp = buf;
    std::size_t sz = sizeof(buf);
#  if defined(__CYGWIN__) || defined(__USE_GNU)
    // Oddball version of strerror_r
    const char * c_str = strerror_r( ev, bp, sz );
    return  c_str
    ? std::string( c_str )
    : unknown_err;
#  else
    // POSIX version of strerror_r
    int result;
    for (;;)
    {
		// strerror_r returns 0 on success, otherwise ERANGE if buffer too small,
		// invalid_argument if ev not a valid error number
#  if defined (__sgi)
		const char * c_str = strerror( ev );
		result = 0;
		return  c_str
		? std::string( c_str )
		: unknown_err;
#  else
#   if defined(_WIN32)
		//from older boost for VS2010
		result = strerror_s( bp, sz, ev );
#   else
		//from boost 1.55.0
		result = strerror_r( ev, bp, sz );
#   endif
		
#  endif
		if (result == 0 )
			break;
		else
		{
#  if defined(__linux)
			// Linux strerror_r returns -1 on error, with error number in errno
			result = errno;
#  endif
			if ( result !=  ERANGE ) break;
			if ( sz > sizeof(buf) ) std::free( bp );
			sz *= 2;
			if ( (bp = static_cast<char*>(std::malloc( sz ))) == 0 )
			return std::string( "ENOMEM" );
		}
    }
    std::string msg;
    try
    {
		msg = ( ( result == Generic::invalid_argument ) ? "Unknown error" : bp );
    }
	// See ticket #2098
    catch(...)
    {
		// just eat the exception
    }

    if ( sz > sizeof(buf) ) std::free( bp );
    sz = 0;
    return msg;
#  endif   // else POSIX version of strerror_r
# endif  // else use strerror_r
}

CORE_API const ErrorCategory & getGenericCategory()
{
    static const GenericErrorCategory GenericCategoryConst;
    return GenericCategoryConst;
}

} //Error

} // Core


