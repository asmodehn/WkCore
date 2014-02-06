#ifndef ERRORCODE_HH
#define ERRORCODE_HH

#include "ErrorCategory.hh"

#include "ErrorCondition.hh"

namespace Core
{

namespace Error
{


/**
 * Error Code : System specific Error class.
 * We want errorCode to be a value type that can be copied without slicing
 * and without requiring heap allocation, but we also want it to have
 * polymorphic behavior based on the error category. This is achieved by
 * abstract base class error_category supplying the polymorphic behavior,
 * and ErrorCode containing a pointer to an object of a type derived
 * from ErrorCategory.
 *
 * Design from Boost/system 1.41
 *
 */
	/*
class ErrorCode
{

public:

    // constructors:
    ErrorCode() : m_val(0), m_cat(&getSystemCategory()) {}
    ErrorCode( int val, const ErrorCategory & cat ) : m_val(val), m_cat(&cat) {}

    template <class ErrorCodeEnum>
    ErrorCode(ErrorCodeEnum e, ErrorCodeEnum* = 0)
    {
        *this = makeErrorCode(e);
    }

    // modifiers:
    void assign( int val, const ErrorCategory & cat )
    {
        m_val = val;
        m_cat = &cat;
    }

    //TODO : should be enabled only if the type is an enum for any system's error code
    template<typename ErrorCodeEnum>
    ErrorCode & operator=( ErrorCodeEnum val )
    {
        *this = makeErrorCode(val);
        return *this;
    }

    void clear()
    {
        m_val = 0;
        m_cat = &getSystemCategory();
    }

    // observers:
    int                     value() const
    {
        return m_val;
    }
    const ErrorCategory &  category() const
    {
        return *m_cat;
    }
    ErrorCondition         defaultErrorCondition() const
    {
        return m_cat->defaultErrorCondition(value());
    }
    std::string             message() const
    {
        return m_cat->message(value());
    }

    operator bool() const  // true if error
    {
        return m_val == 0 ? false : true;
    }

    bool operator!() const  // true if no error
    {
        return m_val == 0;
    }

    // relationals:
    inline friend bool operator==( const ErrorCode & lhs, const ErrorCode & rhs )
    //  the more symmetrical non-member syntax allows enum
    //  conversions work for both rhs and lhs.
    {
        return lhs.m_cat == rhs.m_cat && lhs.m_val == rhs.m_val;
    }

    inline friend bool operator<( const ErrorCode & lhs, const ErrorCode & rhs )
    //  the more symmetrical non-member syntax allows enum
    //  conversions work for both rhs and lhs.
    {
        return lhs.m_cat < rhs.m_cat || (lhs.m_cat == rhs.m_cat && lhs.m_val < rhs.m_val);
    }

private:

    int                     m_val;
    const ErrorCategory *  m_cat;

};


//non member functions
template <class charT, class traits>
inline std::basic_ostream<charT,traits>& operator<< (std::basic_ostream<charT,traits>& os, ErrorCode ec)
{
    os << ec.category().name() << ':' << ec.value();
    return os;
}


inline std::size_t hash_value( const ErrorCode & ec )
{
    return static_cast<std::size_t>(ec.value()) + reinterpret_cast<std::size_t>(&ec.category());
}


*/

} //Error

} // Core



#endif
