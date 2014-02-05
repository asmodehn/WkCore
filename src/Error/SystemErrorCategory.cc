#include "Core/Error/ErrorCategory.hh"

#include "Core/Error/ErrorCondition.hh"

#include <vector>
#include <cstdlib>
#include <cassert>

# if defined( CORE_PLATFORM_WIN32 )
#   include <windows.h>
#   ifndef ERROR_INCORRECT_SIZE
#    define ERROR_INCORRECT_SIZE ERROR_BAD_ARGUMENTS
#   endif
# endif

namespace Core
{
namespace Error
{

const char * SystemErrorCategory::name() const
{
    return "system";
}

///returns error condition corresponding to error code
ErrorCondition SystemErrorCategory::defaultErrorCondition( int ev ) const
{
    switch ( ev )
    {
    case 0:
        return makeErrorCondition( Generic::success );
# if defined(CORE_PLATFORM_POSIX)
        // POSIX-like O/S -> posix_errno decode table  ---------------------------//
    case E2BIG:
        return makeErrorCondition( Generic::argument_list_too_long );
    case EACCES:
        return makeErrorCondition( Generic::permission_denied );
    case EADDRINUSE:
        return makeErrorCondition( Generic::address_in_use );
    case EADDRNOTAVAIL:
        return makeErrorCondition( Generic::address_not_available );
    case EAFNOSUPPORT:
        return makeErrorCondition( Generic::address_family_not_supported );
    case EAGAIN:
        return makeErrorCondition( Generic::resource_unavailable_try_again );
#   if EALREADY != EBUSY  //  EALREADY and EBUSY are the same on QNX Neutrino
    case EALREADY:
        return makeErrorCondition( Generic::connection_already_in_progress );
#   endif
    case EBADF:
        return makeErrorCondition( Generic::bad_file_descriptor );
    case EBADMSG:
        return makeErrorCondition( Generic::bad_message );
    case EBUSY:
        return makeErrorCondition( Generic::device_or_resource_busy );
    case ECANCELED:
        return makeErrorCondition( Generic::operation_canceled );
    case ECHILD:
        return makeErrorCondition( Generic::no_child_process );
    case ECONNABORTED:
        return makeErrorCondition( Generic::connection_aborted );
    case ECONNREFUSED:
        return makeErrorCondition( Generic::connection_refused );
    case ECONNRESET:
        return makeErrorCondition( Generic::connection_reset );
    case EDEADLK:
        return makeErrorCondition( Generic::resource_deadlock_would_occur );
    case EDESTADDRREQ:
        return makeErrorCondition( Generic::destination_address_required );
    case EDOM:
        return makeErrorCondition( Generic::argument_out_of_domain );
    case EEXIST:
        return makeErrorCondition( Generic::file_exists );
    case EFAULT:
        return makeErrorCondition( Generic::bad_address );
    case EFBIG:
        return makeErrorCondition( Generic::file_too_large );
    case EHOSTUNREACH:
        return makeErrorCondition( Generic::host_unreachable );
    case EIDRM:
        return makeErrorCondition( Generic::identifier_removed );
    case EILSEQ:
        return makeErrorCondition( Generic::illegal_byte_sequence );
    case EINPROGRESS:
        return makeErrorCondition( Generic::operation_in_progress );
    case EINTR:
        return makeErrorCondition( Generic::interrupted );
    case EINVAL:
        return makeErrorCondition( Generic::invalid_argument );
    case EIO:
        return makeErrorCondition( Generic::io_error );
    case EISCONN:
        return makeErrorCondition( Generic::already_connected );
    case EISDIR:
        return makeErrorCondition( Generic::is_a_directory );
    case ELOOP:
        return makeErrorCondition( Generic::too_many_symbolic_link_levels );
    case EMFILE:
        return makeErrorCondition( Generic::too_many_files_open );
    case EMLINK:
        return makeErrorCondition( Generic::too_many_links );
    case EMSGSIZE:
        return makeErrorCondition( Generic::message_size );
    case ENAMETOOLONG:
        return makeErrorCondition( Generic::filename_too_long );
    case ENETDOWN:
        return makeErrorCondition( Generic::network_down );
    case ENETRESET:
        return makeErrorCondition( Generic::network_reset );
    case ENETUNREACH:
        return makeErrorCondition( Generic::network_unreachable );
    case ENFILE:
        return makeErrorCondition( Generic::too_many_files_open_in_system );
    case ENOBUFS:
        return makeErrorCondition( Generic::no_buffer_space );
    case ENODATA:
        return makeErrorCondition( Generic::no_message_available );
    case ENODEV:
        return makeErrorCondition( Generic::no_such_device );
    case ENOENT:
        return makeErrorCondition( Generic::no_such_file_or_directory );
    case ENOEXEC:
        return makeErrorCondition( Generic::executable_format_error );
    case ENOLCK:
        return makeErrorCondition( Generic::no_lock_available );
    case ENOLINK:
        return makeErrorCondition( Generic::no_link );
    case ENOMEM:
        return makeErrorCondition( Generic::not_enough_memory );
    case ENOMSG:
        return makeErrorCondition( Generic::no_message );
    case ENOPROTOOPT:
        return makeErrorCondition( Generic::no_protocol_option );
    case ENOSPC:
        return makeErrorCondition( Generic::no_space_on_device );
    case ENOSR:
        return makeErrorCondition( Generic::no_stream_resources );
    case ENOSTR:
        return makeErrorCondition( Generic::not_a_stream );
    case ENOSYS:
        return makeErrorCondition( Generic::function_not_supported );
    case ENOTCONN:
        return makeErrorCondition( Generic::not_connected );
    case ENOTDIR:
        return makeErrorCondition( Generic::not_a_directory );
# if ENOTEMPTY != EEXIST // AIX treats ENOTEMPTY and EEXIST as the same value
    case ENOTEMPTY:
        return makeErrorCondition( Generic::directory_not_empty );
# endif // ENOTEMPTY != EEXIST
    case ENOTRECOVERABLE:
        return makeErrorCondition( Generic::state_not_recoverable );
    case ENOTSOCK:
        return makeErrorCondition( Generic::not_a_socket );
    case ENOTSUP:
        return makeErrorCondition( Generic::not_supported );
    case ENOTTY:
        return makeErrorCondition( Generic::inappropriate_io_control_operation );
    case ENXIO:
        return makeErrorCondition( Generic::no_such_device_or_address );
# if EOPNOTSUPP != ENOTSUP
    case EOPNOTSUPP:
        return makeErrorCondition( Generic::operation_not_supported );
# endif // EOPNOTSUPP != ENOTSUP
    case EOVERFLOW:
        return makeErrorCondition( Generic::value_too_large );
    case EOWNERDEAD:
        return makeErrorCondition( Generic::owner_dead );
    case EPERM:
        return makeErrorCondition( Generic::operation_not_permitted );
    case EPIPE:
        return makeErrorCondition( Generic::broken_pipe );
    case EPROTO:
        return makeErrorCondition( Generic::protocol_error );
    case EPROTONOSUPPORT:
        return makeErrorCondition( Generic::protocol_not_supported );
    case EPROTOTYPE:
        return makeErrorCondition( Generic::wrong_protocol_type );
    case ERANGE:
        return makeErrorCondition( Generic::result_out_of_range );
    case EROFS:
        return makeErrorCondition( Generic::read_only_file_system );
    case ESPIPE:
        return makeErrorCondition( Generic::invalid_seek );
    case ESRCH:
        return makeErrorCondition( Generic::no_such_process );
    case ETIME:
        return makeErrorCondition( Generic::stream_timeout );
    case ETIMEDOUT:
        return makeErrorCondition( Generic::timed_out );
    case ETXTBSY:
        return makeErrorCondition( Generic::text_file_busy );
# if EAGAIN != EWOULDBLOCK
    case EWOULDBLOCK:
        return makeErrorCondition( Generic::operation_would_block );
# endif // EAGAIN != EWOULDBLOCK
    case EXDEV:
        return makeErrorCondition( Generic::cross_device_link );
#elif defined (CORE_PLATFORM_WIN32)
        // Windows system -> posix_errno decode table  ---------------------------//
        // see WinError.h comments for descriptions of errors
    case ERROR_ACCESS_DENIED:
        return makeErrorCondition( Generic::permission_denied );
    case ERROR_ALREADY_EXISTS:
        return makeErrorCondition( Generic::file_exists );
    case ERROR_BAD_UNIT:
        return makeErrorCondition( Generic::no_such_device );
    case ERROR_BUFFER_OVERFLOW:
        return makeErrorCondition( Generic::filename_too_long );
    case ERROR_BUSY:
        return makeErrorCondition( Generic::device_or_resource_busy );
    case ERROR_BUSY_DRIVE:
        return makeErrorCondition( Generic::device_or_resource_busy );
    case ERROR_CANNOT_MAKE:
        return makeErrorCondition( Generic::permission_denied );
    case ERROR_CANTOPEN:
        return makeErrorCondition( Generic::io_error );
    case ERROR_CANTREAD:
        return makeErrorCondition( Generic::io_error );
    case ERROR_CANTWRITE:
        return makeErrorCondition( Generic::io_error );
    case ERROR_CURRENT_DIRECTORY:
        return makeErrorCondition( Generic::permission_denied );
    case ERROR_DEV_NOT_EXIST:
        return makeErrorCondition( Generic::no_such_device );
    case ERROR_DEVICE_IN_USE:
        return makeErrorCondition( Generic::device_or_resource_busy );
    case ERROR_DIR_NOT_EMPTY:
        return makeErrorCondition( Generic::directory_not_empty );
    case ERROR_DIRECTORY:
        return makeErrorCondition( Generic::invalid_argument ); // WinError.h: "The directory name is invalid"
    case ERROR_DISK_FULL:
        return makeErrorCondition( Generic::no_space_on_device );
    case ERROR_FILE_EXISTS:
        return makeErrorCondition( Generic::file_exists );
    case ERROR_FILE_NOT_FOUND:
        return makeErrorCondition( Generic::no_such_file_or_directory );
    case ERROR_HANDLE_DISK_FULL:
        return makeErrorCondition( Generic::no_space_on_device );
    case ERROR_INVALID_ACCESS:
        return makeErrorCondition( Generic::permission_denied );
    case ERROR_INVALID_DRIVE:
        return makeErrorCondition( Generic::no_such_device );
    case ERROR_INVALID_FUNCTION:
        return makeErrorCondition( Generic::function_not_supported );
    case ERROR_INVALID_HANDLE:
        return makeErrorCondition( Generic::invalid_argument );
    case ERROR_INVALID_NAME:
        return makeErrorCondition( Generic::invalid_argument );
    case ERROR_LOCK_VIOLATION:
        return makeErrorCondition( Generic::no_lock_available );
    case ERROR_LOCKED:
        return makeErrorCondition( Generic::no_lock_available );
    case ERROR_NEGATIVE_SEEK:
        return makeErrorCondition( Generic::invalid_argument );
    case ERROR_NOACCESS:
        return makeErrorCondition( Generic::permission_denied );
    case ERROR_NOT_ENOUGH_MEMORY:
        return makeErrorCondition( Generic::not_enough_memory );
    case ERROR_NOT_READY:
        return makeErrorCondition( Generic::resource_unavailable_try_again );
    case ERROR_NOT_SAME_DEVICE:
        return makeErrorCondition( Generic::cross_device_link );
    case ERROR_OPEN_FAILED:
        return makeErrorCondition( Generic::io_error );
    case ERROR_OPEN_FILES:
        return makeErrorCondition( Generic::device_or_resource_busy );
    case ERROR_OPERATION_ABORTED:
        return makeErrorCondition( Generic::operation_canceled );
    case ERROR_OUTOFMEMORY:
        return makeErrorCondition( Generic::not_enough_memory );
    case ERROR_PATH_NOT_FOUND:
        return makeErrorCondition( Generic::no_such_file_or_directory );
    case ERROR_READ_FAULT:
        return makeErrorCondition( Generic::io_error );
    case ERROR_RETRY:
        return makeErrorCondition( Generic::resource_unavailable_try_again );
    case ERROR_SEEK:
        return makeErrorCondition( Generic::io_error );
    case ERROR_SHARING_VIOLATION:
        return makeErrorCondition( Generic::permission_denied );
    case ERROR_TOO_MANY_OPEN_FILES:
        return makeErrorCondition( Generic::too_many_files_open );
    case ERROR_WRITE_FAULT:
        return makeErrorCondition( Generic::io_error );
    case ERROR_WRITE_PROTECT:
        return makeErrorCondition( Generic::permission_denied );
    case WSAEACCES:
        return makeErrorCondition( Generic::permission_denied );
    case WSAEADDRINUSE:
        return makeErrorCondition( Generic::address_in_use );
    case WSAEADDRNOTAVAIL:
        return makeErrorCondition( Generic::address_not_available );
    case WSAEAFNOSUPPORT:
        return makeErrorCondition( Generic::address_family_not_supported );
    case WSAEALREADY:
        return makeErrorCondition( Generic::connection_already_in_progress );
    case WSAEBADF:
        return makeErrorCondition( Generic::bad_file_descriptor );
    case WSAECONNABORTED:
        return makeErrorCondition( Generic::connection_aborted );
    case WSAECONNREFUSED:
        return makeErrorCondition( Generic::connection_refused );
    case WSAECONNRESET:
        return makeErrorCondition( Generic::connection_reset );
    case WSAEDESTADDRREQ:
        return makeErrorCondition( Generic::destination_address_required );
    case WSAEFAULT:
        return makeErrorCondition( Generic::bad_address );
    case WSAEHOSTUNREACH:
        return makeErrorCondition( Generic::host_unreachable );
    case WSAEINPROGRESS:
        return makeErrorCondition( Generic::operation_in_progress );
    case WSAEINTR:
        return makeErrorCondition( Generic::interrupted );
    case WSAEINVAL:
        return makeErrorCondition( Generic::invalid_argument );
    case WSAEISCONN:
        return makeErrorCondition( Generic::already_connected );
    case WSAEMFILE:
        return makeErrorCondition( Generic::too_many_files_open );
    case WSAEMSGSIZE:
        return makeErrorCondition( Generic::message_size );
    case WSAENAMETOOLONG:
        return makeErrorCondition( Generic::filename_too_long );
    case WSAENETDOWN:
        return makeErrorCondition( Generic::network_down );
    case WSAENETRESET:
        return makeErrorCondition( Generic::network_reset );
    case WSAENETUNREACH:
        return makeErrorCondition( Generic::network_unreachable );
    case WSAENOBUFS:
        return makeErrorCondition( Generic::no_buffer_space );
    case WSAENOPROTOOPT:
        return makeErrorCondition( Generic::no_protocol_option );
    case WSAENOTCONN:
        return makeErrorCondition( Generic::not_connected );
    case WSAENOTSOCK:
        return makeErrorCondition( Generic::not_a_socket );
    case WSAEOPNOTSUPP:
        return makeErrorCondition( Generic::operation_not_supported );
    case WSAEPROTONOSUPPORT:
        return makeErrorCondition( Generic::protocol_not_supported );
    case WSAEPROTOTYPE:
        return makeErrorCondition( Generic::wrong_protocol_type );
    case WSAETIMEDOUT:
        return makeErrorCondition( Generic::timed_out );
    case WSAEWOULDBLOCK:
        return makeErrorCondition( Generic::operation_would_block );
#endif
    default:
        return ErrorCondition( ev, SystemCategory );
    }
}

# if !defined( BOOST_WINDOWS_API )

std::string SystemErrorCategory::message( int ev ) const
{
    return GenericCategory.message( ev );
}
# else
// TODO:

//Some quick notes on the implementation (sorry for the noise if
//someone has already mentioned them):
//
//- The ::LocalFree() usage isn't exception safe.
//
//See:
//
//<http://boost.cvs.sourceforge.net/boost/boost/boost/asio/system_exception.hpp?revision=1.1&view=markup>
//
//in the implementation of what() for an example.
//
//Cheers,
//Chris
std::string SystemErrorCategory::message( int ev ) const
{
# ifndef BOOST_NO_ANSI_APIS
    LPVOID lpMsgBuf;
    DWORD retval = ::FormatMessageA(
                       FORMAT_MESSAGE_ALLOCATE_BUFFER |
                       FORMAT_MESSAGE_FROM_SYSTEM |
                       FORMAT_MESSAGE_IGNORE_INSERTS,
                       NULL,
                       ev,
                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                       (LPSTR) &lpMsgBuf,
                       0,
                       NULL
                   );
    if (retval == 0)
        return std::string("Unknown error");

    std::string str( static_cast<LPCSTR>(lpMsgBuf) );
    ::LocalFree( lpMsgBuf ); // free the buffer
# else  // WinCE workaround
LPVOID lpMsgBuf;
DWORD retval = ::FormatMessageW(
                   FORMAT_MESSAGE_ALLOCATE_BUFFER |
                   FORMAT_MESSAGE_FROM_SYSTEM |
                   FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL,
                   ev,
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                   (LPWSTR) &lpMsgBuf,
                   0,
                   NULL
               );
if (retval == 0)
    return std::string("Unknown error");

int num_chars = (wcslen( static_cast<LPCWSTR>(lpMsgBuf) ) + 1) * 2;
LPSTR narrow_buffer = (LPSTR)_alloca( num_chars );
if (::WideCharToMultiByte(CP_ACP, 0, static_cast<LPCWSTR>(lpMsgBuf), -1, narrow_buffer, num_chars, NULL, NULL) == 0)
    return std::string("Unknown error");

std::string str( narrow_buffer );
::LocalFree( lpMsgBuf ); // free the buffer
# endif
    while ( str.size()
            && (str[str.size()-1] == '\n' || str[str.size()-1] == '\r') )
        str.erase( str.size()-1 );
    if ( str.size() && str[str.size()-1] == '.' )
    {
        str.erase( str.size()-1 );
    }
    return str;
}
# endif

CORE_API const ErrorCategory & getSystemCategory()
{
    static const SystemErrorCategory  SystemCategoryConst;
    return SystemCategoryConst;
}

} //namespace Error
} // namespace Core
