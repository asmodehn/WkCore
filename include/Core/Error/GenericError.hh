#ifndef GENERICERROR_HH
#define GENERICERROR_HH

#include "CError.hh" // for missing generic errors in some operating systems

#include <iostream>

namespace Core
{

namespace Error
{

	/*
//further references
class ErrorCategory;
class ErrorCondition;
class ErrorCode;


namespace Generic
{

///These are generic error conditions
enum error_code_t
{
    success = 0,
    address_family_not_supported = EAFNOSUPPORT,
    address_in_use = EADDRINUSE,
    address_not_available = EADDRNOTAVAIL,
    already_connected = EISCONN,
    argument_list_too_long = E2BIG,
    argument_out_of_domain = EDOM,
    bad_address = EFAULT,
    bad_file_descriptor = EBADF,
    bad_message = EBADMSG,
    broken_pipe = EPIPE,
    connection_aborted = ECONNABORTED,
    connection_already_in_progress = EALREADY,
    connection_refused = ECONNREFUSED,
    connection_reset = ECONNRESET,
    cross_device_link = EXDEV,
    destination_address_required = EDESTADDRREQ,
    device_or_resource_busy = EBUSY,
    directory_not_empty = ENOTEMPTY,
    executable_format_error = ENOEXEC,
    file_exists = EEXIST,
    file_too_large = EFBIG,
    filename_too_long = ENAMETOOLONG,
    function_not_supported = ENOSYS,
    host_unreachable = EHOSTUNREACH,
    identifier_removed = EIDRM,
    illegal_byte_sequence = EILSEQ,
    inappropriate_io_control_operation = ENOTTY,
    interrupted = EINTR,
    invalid_argument = EINVAL,
    invalid_seek = ESPIPE,
    io_error = EIO,
    is_a_directory = EISDIR,
    message_size = EMSGSIZE,
    network_down = ENETDOWN,
    network_reset = ENETRESET,
    network_unreachable = ENETUNREACH,
    no_buffer_space = ENOBUFS,
    no_child_process = ECHILD,
    no_link = ENOLINK,
    no_lock_available = ENOLCK,
    no_message_available = ENODATA,
    no_message = ENOMSG,
    no_protocol_option = ENOPROTOOPT,
    no_space_on_device = ENOSPC,
    no_stream_resources = ENOSR,
    no_such_device_or_address = ENXIO,
    no_such_device = ENODEV,
    no_such_file_or_directory = ENOENT,
    no_such_process = ESRCH,
    not_a_directory = ENOTDIR,
    not_a_socket = ENOTSOCK,
    not_a_stream = ENOSTR,
    not_connected = ENOTCONN,
    not_enough_memory = ENOMEM,
    not_supported = ENOTSUP,
    operation_canceled = ECANCELED,
    operation_in_progress = EINPROGRESS,
    operation_not_permitted = EPERM,
    operation_not_supported = EOPNOTSUPP,
    operation_would_block = EWOULDBLOCK,
    owner_dead = EOWNERDEAD,
    permission_denied = EACCES,
    protocol_error = EPROTO,
    protocol_not_supported = EPROTONOSUPPORT,
    read_only_file_system = EROFS,
    resource_deadlock_would_occur = EDEADLK,
    resource_unavailable_try_again = EAGAIN,
    result_out_of_range = ERANGE,
    state_not_recoverable = ENOTRECOVERABLE,
    stream_timeout = ETIME,
    text_file_busy = ETXTBSY,
    timed_out = ETIMEDOUT,
    too_many_files_open_in_system = ENFILE,
    too_many_files_open = EMFILE,
    too_many_links = EMLINK,
    too_many_symbolic_link_levels = ELOOP,
    value_too_large = EOVERFLOW,
    wrong_protocol_type = EPROTOTYPE
};

//explicit conversion
ErrorCode makeErrorCode( Generic::error_code_t e );

//implicit conversion
ErrorCondition makeErrorCondition( Generic::error_code_t e );


} // Generic



//non members operators

//error codes operators

bool operator==( const ErrorCode & lhs, const ErrorCode & rhs );
bool operator==( const ErrorCode & code, const ErrorCondition & condition );
bool operator==( const ErrorCondition & condition, const ErrorCode & code );
bool operator==( const ErrorCondition & lhs, const ErrorCondition & rhs );

bool operator!=( const ErrorCode & lhs, const ErrorCode & rhs );
bool operator!=( const ErrorCode & code, const ErrorCondition & condition );
bool operator!=( const ErrorCondition & condition, const ErrorCode & code );
bool operator!=( const ErrorCondition & lhs, const ErrorCondition & rhs );

bool operator<( const ErrorCode & lhs, const ErrorCode & rhs );
bool operator<( const ErrorCondition & lhs, const ErrorCondition & rhs );


//  ----------------------------------------------------------------------//

//  Operating system specific interfaces  --------------------------------//


//  The interface is divided into general and system-specific portions to
//  meet these requirements:
//
//  * Code calling an operating system API can create an error_code with
//    a single category (system_category), even for POSIX-like operating
//    systems that return some POSIX errno values and some native errno
//    values. This code should not have to pay the cost of distinguishing
//    between categories, since it is not yet known if that is needed.
//
//  * Users wishing to write system-specific code should be given enums for
//    at least the common error cases.
//
//  * System specific code should fail at compile time if moved to another
//    operating system.

//  The system specific portions of the interface are located in headers
//  with names reflecting the operating system. For example,
//
//       <boost/system/cygwin_error.hpp>
//       <boost/system/linux_error.hpp>
//       <boost/system/windows_error.hpp>
//
//  These headers are effectively empty for compiles on operating systems
//  where they are not applicable.

//  ----------------------------------------------------------------------//


namespace internal
{
inline Core::Error::ErrorCode * throws()
{
    return 0;
}
}
//  Misuse of the ErrorCode object is turned into a noisy failure by
//  poisoning the reference. This particular implementation doesn't
//  produce warnings or errors from popular compilers, is very efficient
//  (as determined by inspecting generated code), and does not suffer
//  from order of initialization problems. In practice, it also seems
//  cause user function error handling implementation errors to be detected
//  very early in the development cycle.

inline Core::Error::ErrorCode & throws()
{
    return *internal::throws();
}

*/
} // Error

} // Core


#endif

