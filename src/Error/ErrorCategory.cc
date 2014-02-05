#include "Core/Error/ErrorCategory.hh"

#include "Core/Error/ErrorCode.hh"
#include "Core/Error/ErrorCondition.hh"

namespace Core
{

namespace Error
{


//  error_category default implementation  -------------------------------//

ErrorCondition ErrorCategory::defaultErrorCondition( int ev ) const
{
    return ErrorCondition( ev, *this );
}

bool ErrorCategory::equivalent( int code, const ErrorCondition & condition ) const
{
    return defaultErrorCondition( code ) == condition;
}

bool ErrorCategory::equivalent( const ErrorCode & code, int condition ) const
{
    return *this == code.category() && code.value() == condition;
}



} // Error

}// Core
