#ifndef ERRORCATEGORY_HH
#define ERRORCATEGORY_HH

//TEMPORARY
#ifndef CORE_TEST
#define CORE_BUILD
#endif

#include "Config.hh" // for library configuration

#include <string>
#include <map>

#include "GenericError.hh"

namespace Core
{

namespace Error
{


/**
 * ErrorCategory : What kind of Error is it ?
 * Where does it comes from ? Which component does it belongs to ?
 * A client can extend ErrorCategory to specify it s own errors.
 * It will be used in ErrorCode ( System specific ) and ErrorCondition ( portable )
 */
/*
class ErrorCategory
{
    //to prevent copy
private:
    ErrorCategory( const ErrorCategory & ) {}
    const ErrorCategory& operator=( const ErrorCategory & )
    {
        return *this;
    }

protected:
    ErrorCategory() {}; //?

public:
    virtual ~ErrorCategory() {}

    virtual const char *     name() const = 0;
    virtual std::string      message( int ev ) const = 0;
    virtual ErrorCondition  defaultErrorCondition( int ev ) const;
    virtual bool             equivalent( int code,
                                         const ErrorCondition & condition ) const;
    virtual bool             equivalent( const ErrorCode & code,
                                         int condition ) const;

    bool operator==(const ErrorCategory & rhs) const
    {
        return this == &rhs;
    }
    bool operator!=(const ErrorCategory & rhs) const
    {
        return this != &rhs;
    }
    bool operator<( const ErrorCategory & rhs ) const
    {
        return std::less<const ErrorCategory*>()( this, &rhs );
    }
};

//  predefined error categories  -----------------------------------------//

CORE_API const ErrorCategory &  getSystemCategory();
CORE_API const ErrorCategory &  getGenericCategory();

static const ErrorCategory &  SystemCategory = getSystemCategory();
static const ErrorCategory &  GenericCategory = getGenericCategory();

class GenericErrorCategory : public ErrorCategory
{
public:
    GenericErrorCategory() {}
    const char *   name() const;
    std::string    message( int ev ) const;
};


class SystemErrorCategory : public ErrorCategory
{
public:
    SystemErrorCategory() {}
    const char *        name() const;
    std::string         message( int ev ) const;
    ErrorCondition      defaultErrorCondition( int ev ) const;
};

*/
} // Error

} // Core

#endif
