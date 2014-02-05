#ifndef CORE_NULLOSTREAM_HH
#define CORE_NULLOSTREAM_HH

#include "Core/Logging/nullstreambuf.hh"

#include <ostream>

namespace Core
{

namespace stdext
{

template <class cT, class traits = std::char_traits<cT> >
class basic_onullstream: public std::basic_ostream<cT, traits>
{
public:
    basic_onullstream():
            std::basic_ios<cT, traits>(),
            std::basic_ostream<cT, traits>(0),
            m_sbuf()
    {
        init(&m_sbuf);
    }

private:
    basic_nullstreambuf<cT, traits> m_sbuf;
};

typedef basic_onullstream<char> onullstream;
typedef basic_onullstream<wchar_t> wonullstream;


} //stdext

extern stdext::onullstream cnull;
extern stdext::wonullstream wcnull;


} //Core




#endif
