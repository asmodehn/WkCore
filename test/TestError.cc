#include "Config.hh"

//TEMPORARY, while build not proper
#define CORE_TEST

#include "TestCommon.hh"

//specific includes for this test... maybe we could change some old C functions with new C++ equivalent
#include <sstream>
#include <cstring>

////////////////////////////
//Testing Error Code //
////////////////////////////
#include "Core/Error/Error.hh"


# if defined( CORE_PLATFORM_WIN32 )
#   include "winerror.h"
#   define CORE_ACCESS_ERROR_MACRO ERROR_ACCESS_DENIED
# elif defined( CORE_PLATFORM_POSIX )
#   define CORE_ACCESS_ERROR_MACRO EACCES
# else
#   error "Only supported for POSIX and Windows"
# endif

using namespace Core::Error;

namespace
{

// Initialization test
//  This test verifiies that the error_category vtable does not suffer from
//  order-of-initialization problems.

struct foo
{
    foo()
    {
        ErrorCode ec;

        ec == makeErrorCode(Generic::permission_denied); //makeErrorCode here should be implicit... TODO : compare with make ErrorCondition, one explicit one implicit.
    }
} f;

// Checking outstream
void check_ostream( Core::Error::ErrorCode ec, const char * expected )
{
    std::stringstream ss;
    std::string s;

    ss << ec;
    ss >> s;
    TEST( s == expected );
}
}

int main ( int argc, char * argv [] )
{

    std::cout << "General tests...\n";
    // unit tests:

    TEST( GenericCategory == GenericCategory );
    TEST( SystemCategory == SystemCategory );
    TEST( GenericCategory != SystemCategory );
    TEST( SystemCategory != GenericCategory );

    if ( std::less<const ErrorCategory*>()( &GenericCategory, &SystemCategory ) )
    {
        TEST( GenericCategory < SystemCategory );
        TEST( !(SystemCategory < GenericCategory) );
    }
    else
    {
        TEST( SystemCategory < GenericCategory );
        TEST( !(GenericCategory < SystemCategory) );
    }


    ErrorCode ec;
    ErrorCondition dec;
    TEST( !ec );
    TEST( ec.value() == 0 );
    dec = ec.defaultErrorCondition();
    TEST( dec.value() == 0 );
    TEST( dec.category() == GenericCategory );
    TEST( ec == makeErrorCode(Generic::success) ); //TODO makeErrorCode should be implicit ?
    TEST( ec.category() == SystemCategory );
    TEST( std::strcmp( ec.category().name(), "system") == 0 );
    TEST( !(ec < ErrorCode( 0, SystemCategory )) );
    TEST( !(ErrorCode( 0, SystemCategory ) < ec) );
    TEST( ec < ErrorCode( 1, SystemCategory ) );
    TEST( !(ErrorCode( 1, SystemCategory ) < ec) );

    ErrorCode ec_0_system( 0, SystemCategory );
    TEST( !ec_0_system );
    TEST( ec_0_system.value() == 0 );
    dec = ec_0_system.defaultErrorCondition();
    TEST( dec.value() == 0 );
    TEST( dec.category() == GenericCategory );
    TEST( ec_0_system == makeErrorCode(Generic::success) ); //TODO : make ErrorCode implicit ?
    TEST( ec_0_system.category() == SystemCategory );
    TEST( std::strcmp( ec_0_system.category().name(), "system") == 0 );
    check_ostream( ec_0_system, "system:0" );

    TEST( ec_0_system == ec );

    ErrorCode ec_1_system( 1, SystemCategory );
    TEST( ec_1_system );
    TEST( ec_1_system.value() == 1 );
    TEST( ec_1_system.value() != 0 );
    TEST( ec != ec_1_system );
    TEST( ec_0_system != ec_1_system );
    check_ostream( ec_1_system, "system:1" );

    ec = ErrorCode( CORE_ACCESS_ERROR_MACRO, SystemCategory );
    TEST( ec );
    TEST( ec.value() == CORE_ACCESS_ERROR_MACRO );
    dec = ec.defaultErrorCondition();
    TEST( dec.value() == static_cast<int>(Generic::permission_denied) );
    TEST( dec.category() == GenericCategory );
    TEST( dec == ErrorCondition( Generic::permission_denied, GenericCategory ) );
    TEST( dec == makeErrorCondition(Generic::permission_denied) );
    TEST( Generic::permission_denied == dec.value() ); //TODO : who should be casted here ??
    TEST( ec == makeErrorCode(Generic::permission_denied) );
    TEST( ec.category() == SystemCategory );
    TEST( std::strcmp( ec.category().name(), "system") == 0 );

    // test the explicit make_error_code conversion for generic
    ec = makeErrorCode( Generic::bad_message );
    TEST( ec );
    TEST( ec == makeErrorCode(Generic::bad_message) ); //TODO : this test here should test the implicit conversion
    TEST( Generic::bad_message == ec.value() ); //TODO : here value() should not be explicit
    TEST( ec != makeErrorCode(Generic::permission_denied) );//TODO : this test here should test the implicit conversion
    TEST( Generic::permission_denied != ec.value() ); //TODO : here value() should not be explicit
    TEST( ec.category() == GenericCategory );

    // test error_code and error_condition message();
    // see Boost.Filesystem operations_test for code specific message() tests
    ec = ErrorCode( -1, SystemCategory );
    std::cout << "error_code message for -1 is \"" << ec.message() << "\"\n";
#if defined(BOOST_WINDOWS_API)
    // Borland appends newline, so just check text
    TEST( ec.message().substr(0,13) == "Unknown error" );
#elif  defined(linux) || defined(__linux) || defined(__linux__)
    // Linux appends value to message as unsigned, so it varies with # of bits
    TEST( ec.message().substr(0,13) == "Unknown error" );
#elif defined(__hpux)
    TEST( ec.message() == "" );
#elif defined(__osf__)
    TEST( ec.message() == "Error -1 occurred." );
#elif defined(__vms)
    TEST( ec.message() == "error -1" );
#endif

    ec = ErrorCode( CORE_ACCESS_ERROR_MACRO, SystemCategory );
    TEST( ec.message() != "" );
    TEST( ec.message().substr( 0, 13) != "Unknown error" );

    dec = ErrorCondition( -1, GenericCategory );
    std::cout << "error_condition message for -1 is \"" << dec.message() << "\"\n";
#if defined(BOOST_WINDOWS_API)
    // Borland appends newline, so just check text
    TEST( dec.message().substr(0,13) == "Unknown error" );
#elif  defined(linux) || defined(__linux) || defined(__linux__)
    // Linux appends value to message as unsigned, so it varies with # of bits
    TEST( dec.message().substr(0,13) == "Unknown error" );
#elif defined(__hpux)
    TEST( dec.message() == "" );
#elif defined(__osf__)
    TEST( dec.message() == "Error -1 occurred." );
#elif defined(__vms)
    TEST( dec.message() == "error -1" );
#endif

    dec = ErrorCondition( CORE_ACCESS_ERROR_MACRO, GenericCategory );
    TEST( dec.message() != "" );
    TEST( dec.message().substr( 0, 13) != "Unknown error" );

#ifdef BOOST_WINDOWS_API
    std::cout << "Windows tests...\n";
    // these tests probe the Windows generic decoder
    //   test the first entry in the decoder table:
    ec = error_code( ERROR_ACCESS_DENIED, SystemCategory );
    TEST( ec.value() == ERROR_ACCESS_DENIED );
    TEST( ec == generic::permission_denied );
    TEST( ec.default_error_condition().value() == generic::permission_denied );
    TEST( ec.default_error_condition().category() == GenericCategory );

    //   test the second entry in the decoder table:
    ec = error_code( ERROR_ALREADY_EXISTS, SystemCategory );
    TEST( ec.value() == ERROR_ALREADY_EXISTS );
    TEST( ec == generic::file_exists );
    TEST( ec.default_error_condition().value() == generic::file_exists );
    TEST( ec.default_error_condition().category() == GenericCategory );

    //   test the third entry in the decoder table:
    ec = error_code( ERROR_BAD_UNIT, SystemCategory );
    TEST( ec.value() == ERROR_BAD_UNIT );
    TEST( ec == generic::no_such_device );
    TEST( ec.default_error_condition().value() == generic::no_such_device );
    TEST( ec.default_error_condition().category() == GenericCategory );

    //   test the last non-Winsock entry in the decoder table:
    ec = error_code( ERROR_WRITE_PROTECT, SystemCategory );
    TEST( ec.value() == ERROR_WRITE_PROTECT );
    TEST( ec == generic::permission_denied );
    TEST( ec.default_error_condition().value() == generic::permission_denied );
    TEST( ec.default_error_condition().category() == GenericCategory );

    //   test the last Winsock entry in the decoder table:
    ec = error_code( WSAEWOULDBLOCK, SystemCategory );
    TEST( ec.value() == WSAEWOULDBLOCK );
    TEST( ec == generic::operation_would_block );
    TEST( ec.default_error_condition().value() == generic::operation_would_block );
    TEST( ec.default_error_condition().category() == GenericCategory );

    //   test not-in-table condition:
    ec = error_code( 1234567890, SystemCategory );
    TEST( ec.value() == 1234567890 );
    TEST( ec.default_error_condition().value() == 1234567890 );
    TEST( ec.default_error_condition().category() == SystemCategory );

#else // POSIX -> changed to -> Generic

    std::cout << "POSIX tests...\n";
    ec = ErrorCode( EACCES, SystemCategory );
    TEST( ec == ErrorCode( Generic::permission_denied, SystemCategory ) );
    TEST( ErrorCode( Generic::permission_denied, SystemCategory ) == ec );
    TEST( ec == makeErrorCode(Generic::permission_denied) ); //makeErrorCode should b implicit here
    TEST( Generic::permission_denied == ec.value() ); //value() should be implicit here
    TEST( ec.defaultErrorCondition().value() == Generic::permission_denied );
    TEST( ec.defaultErrorCondition().category() == GenericCategory );

# ifdef __CYGWIN__

    std::cout << "Cygwin tests...\n";
    ec = cygwin_error::no_package;
    TEST( ec == cygwin_error::no_package );
    TEST( ec == error_code( ENOPKG, systemCategory ) );
    TEST( ec == error_code( cygwin_error::no_package, systemCategory ) );
    TEST( ec.defaultErrorCondition().category() == systemCategory );

# elif defined(linux) || defined(__linux) || defined(__linux__)

    std::cout << "Linux tests...\n";
    ec = linux_error::dot_dot_error;
    TEST( ec == makeErrorCode(linux_error::dot_dot_error) ); //TODO : make ErrorCode should be implicit here
    TEST( ec == ErrorCode( EDOTDOT, SystemCategory ) );
    TEST( ec == ErrorCode( linux_error::dot_dot_error, SystemCategory ) );
    TEST( ec.defaultErrorCondition().category() == SystemCategory );

# endif

#endif

    return ::reportErrors();


    return 0;
}
