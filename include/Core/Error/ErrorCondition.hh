#ifndef ERRORCONDITION_HH
#define ERRORCONDITION_HH

#include "ErrorCategory.hh"

namespace Core
{

namespace Error
{

/**
 * ErrorCondition : portable Error class
 * design from Boost 1.41
 */

class ErrorCondition
{
public:

    // constructors:
    ErrorCondition() : m_val(0), m_cat(&getGenericCategory()) {}
    ErrorCondition( int val, const ErrorCategory & cat ) : m_val(val), m_cat(&cat) {}

    template <class ErrorConditionEnum>
    ErrorCondition( ErrorConditionEnum e, ErrorConditionEnum* = 0)
    {
        *this = makeErrorCondition(e);
    }

    // modifiers:

    void assign( int val, const ErrorCategory & cat )
    {
        m_val = val;
        m_cat = &cat;
    }

    template<typename ErrorConditionEnum>
    ErrorConditionEnum &
    operator=( ErrorConditionEnum val )
    {
        *this = makeErrorCondition(val);
        return *this;
    }

    void clear()
    {
        m_val = 0;
        m_cat = &getGenericCategory();
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
    //  the more symmetrical non-member syntax allows enum
    //  conversions work for both rhs and lhs.
    inline friend bool operator==( const ErrorCondition & lhs,
                                   const ErrorCondition & rhs )
    {
        return lhs.m_cat == rhs.m_cat && lhs.m_val == rhs.m_val;
    }

    inline friend bool operator<( const ErrorCondition & lhs,
                                  const ErrorCondition & rhs )
    //  the more symmetrical non-member syntax allows enum
    //  conversions work for both rhs and lhs.
    {
        return lhs.m_cat < rhs.m_cat
               || (lhs.m_cat == rhs.m_cat && lhs.m_val < rhs.m_val);
    }

private:
    int                     m_val;
    const ErrorCategory *  m_cat;

};


}// Error

} // Core


#endif
