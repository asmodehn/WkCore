#ifndef CORE_NULLSTREAMBUF_HH
#define CORE_NULLSTREAMBUF_HH

#include <streambuf>

namespace Core
{

namespace stdext
{

template <class cT, class traits = std::char_traits<cT> >
class basic_nullstreambuf: public std::basic_streambuf<cT, traits>
{
public:
    basic_nullstreambuf();
    ~basic_nullstreambuf();

    typename traits::int_type overflow(typename traits::int_type c)
    {
        return traits::not_eof(c); // indicate success
    }
};


template <class _CharT, class _Traits>
basic_nullstreambuf<_CharT, _Traits>::basic_nullstreambuf()
        : std::basic_streambuf<_CharT, _Traits>()
{
}

template <class _CharT, class _Traits>
basic_nullstreambuf<_CharT, _Traits>::~basic_nullstreambuf()
{
}


}



}





#endif


