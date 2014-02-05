#include "Core/Error/GenericError.hh"

#include "Core/Error/ErrorCode.hh"
#include "Core/Error/ErrorCondition.hh"

namespace Core
{

namespace Error
{

//  make_* functions for errcode::errcode_t  -----------------------------//
namespace Generic
{

//  explicit conversion:
ErrorCode makeErrorCode( Generic::error_code_t e )
{
    return ErrorCode( e, getGenericCategory() );
}

//  implicit conversion:
ErrorCondition makeErrorCondition( Generic::error_code_t e )
{
    return ErrorCondition( e, getGenericCategory() );
}

} // Generic


bool operator!=( const ErrorCode & lhs, const ErrorCode & rhs )
{
    return !(lhs == rhs);
}

bool operator!=( const ErrorCondition & lhs, const ErrorCondition & rhs )
{
    return !(lhs == rhs);
}

bool operator==( const ErrorCode & code, const ErrorCondition & condition )
{
    return code.category().equivalent( code.value(), condition ) || condition.category().equivalent( code, condition.value() );
}

bool operator!=( const ErrorCode & lhs, const ErrorCondition & rhs )
{
    return !(lhs == rhs);
}

bool operator==( const ErrorCondition & condition, const ErrorCode & code )
{
    return condition.category().equivalent( code, condition.value() ) || code.category().equivalent( code.value(), condition );
}

bool operator!=( const ErrorCondition & lhs, const ErrorCode & rhs )
{
    return !(lhs == rhs);
}




} // Error

}// Core
