/**
 * Copyright (c) 2009-2010, Asmodehn's Corp.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *	    this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *		notice, this list of conditions and the following disclaimer in the
 *	    documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Asmodehn's Corp. nor the names of its
 *	    contributors may be used to endorse or promote products derived
 *	    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 * contact : alex@asmodehn.com
 */

#ifndef CALLBACK_HH
#define CALLBACK_HH

///Callbacks using STL functor.
///here we keep the object we need to callback

#include "Functor.hh"

namespace Core
{

/// 0 Arguments
template <class Result = void >
class Callback0Base
{
public:
    virtual Result operator()() = 0 ;
    virtual Result call() = 0;
};

template <class TClass,class Result = void >
class Callback0 : public Callback0Base<Result>
{
    TClass & m_c;
    std::mem_fun_ref_t<Result,TClass> m_f;
public:
    Callback0 ( TClass & c, Result (TClass::*f) () )
        : m_c(c),m_f(std::mem_fun_ref(f))
    {

    }
    virtual Result operator()()
    {
        return m_f(m_c);
    }
    virtual Result call()
    {
        return m_f(m_c);
    }
};

/// 0 Arguments const
template <class Result = void >
class Callback0constBase
{
public:
    virtual Result operator()() const = 0 ;
    virtual Result call() const = 0;
};


template <class TClass,class Result = void >
class Callback0const : public Callback0constBase<Result>
{
    TClass & m_c;
    std::const_mem_fun_ref_t<Result,TClass> m_f;
public:
    Callback0const ( TClass & c, Result (TClass::*f) () const )
        : m_c(c), m_f(std::mem_fun_ref(f))
    {
        m_f = std::mem_fun_ref(f);
    }
    virtual Result operator()() const
    {
        return m_f(m_c);
    }
    virtual Result call() const
    {
        return m_f(m_c);
    }
};


/// 1 Arguments

template <class Arg1,class Result = void >
class Callback1Base
{
public:
    virtual Result operator()( Arg1 ) = 0 ;
    virtual Result call( Arg1 ) = 0;
};


template <class TClass,class Arg1,class Result = void >
class Callback1 : public Callback1Base<Arg1,Result>
{
    TClass & m_c;
    std::mem_fun1_ref_t<Result,TClass,Arg1> m_f;
public:
    Callback1 ( TClass & c, Result (TClass::*f) (Arg1) )
        : m_c(c),m_f(std::mem_fun_ref(f))
    {

    }
    virtual Result operator()(Arg1 a1)
    {
        return m_f(m_c,a1);
    }
    virtual Result call(Arg1 a1)
    {
        return m_f(m_c,a1);
    }
};

/// 1 Arguments const

template <class Arg1,class Result = void >
class Callback1constBase
{
public:
    virtual Result operator()( Arg1 ) const = 0 ;
    virtual Result call( Arg1 ) const = 0;
};


template <class TClass,class Arg1,class Result = void >
class Callback1const : public Callback1constBase<Arg1,Result>
{
    const TClass & m_c;
    std::const_mem_fun1_ref_t<Result,TClass,Arg1> m_f;
public:
    Callback1const (const TClass & c, Result (TClass::*f) (Arg1) const )
        : m_c(c),m_f(std::mem_fun_ref(f))
    {}
    virtual Result operator()(Arg1 a1) const
    {
        return m_f(m_c,a1);
    }
    virtual Result call(Arg1 a1) const
    {
        return m_f(m_c,a1);
    }
};

/// 2 Arguments

template <class Arg1, class Arg2, class Result = void>
class Callback2Base
{
public:
    virtual Result operator()( Arg1, Arg2 ) = 0 ;
    virtual Result call( Arg1, Arg2 ) = 0;
};


template <class TClass,class Arg1, class Arg2, class Result = void >
class Callback2 : public Callback2Base<Arg1,Arg2,Result>
{
    TClass & m_c;
    Core::stdext::mem_fun2_ref_t<Result,TClass,Arg1,Arg2> m_f;
public:
    Callback2 ( TClass & c, Result (TClass::*f) (Arg1,Arg2) )
        : m_c(c),m_f(Core::stdext::mem_fun_ref(f))
    {}
    virtual Result operator()(Arg1 a1,Arg2 a2)
    {
        return m_f(m_c,a1,a2);
    }
    virtual Result call(Arg1 a1,Arg2 a2)
    {
        return m_f(m_c,a1,a2);
    }
};

/// 2 Arguments const

template <class Arg1,class Arg2,class Result = void >
class Callback2constBase
{
public:
    virtual Result operator()( Arg1, Arg2 ) const = 0 ;
    virtual Result call( Arg1, Arg2 ) const = 0;
};


template <class TClass,class Arg1,class Arg2,class Result = void >
class Callback2const : public Callback2constBase<Arg1,Arg2,Result>
{
    const TClass & m_c;
    Core::stdext::const_mem_fun2_ref_t<Result,TClass,Arg1,Arg2> m_f;
public:
    Callback2const (const TClass & c, Result (TClass::*f) (Arg1,Arg2) const )
        : m_c(c),m_f(Core::stdext::mem_fun_ref(f))
    {}
    virtual Result operator()(Arg1 a1, Arg2 a2) const
    {
        return m_f(m_c,a1,a2);
    }
    virtual Result call(Arg1 a1,Arg2 a2) const
    {
        return m_f(m_c,a1,a2);
    }
};

/// 3 Arguments

template <class Arg1, class Arg2, class Arg3, class Result = void>
class Callback3Base
{
public:
    virtual Result operator()(Arg1, Arg2, Arg3) = 0 ;
    virtual Result call(Arg1, Arg2, Arg3) = 0;
};

template <class TClass,class Arg1, class Arg2, class Arg3, class Result = void >
class Callback3 : public Callback3Base<Arg1,Arg2,Arg3,Result>
{
    TClass & m_c;
    Core::stdext::mem_fun3_ref_t<Result,TClass,Arg1,Arg2,Arg3> m_f;
public:
    Callback3 ( TClass & c, Result (TClass::*f) (Arg1,Arg2,Arg3) )
        : m_c(c),m_f(Core::stdext::mem_fun_ref(f))
    {}
    virtual Result operator()(Arg1 a1,Arg2 a2,Arg3 a3)
    {
        return m_f(m_c,a1,a2,a3);
    }
    virtual Result call(Arg1 a1,Arg2 a2,Arg3 a3)
    {
        return m_f(m_c,a1,a2,a3);
    }
};

/// 3 Arguments const

template <class Arg1,class Arg2,class Arg3,class Result = void >
class Callback3constBase
{
public:
    virtual Result operator()( Arg1, Arg2, Arg3 ) const = 0 ;
    virtual Result call( Arg1, Arg2, Arg3 ) const = 0;
};


template <class TClass,class Arg1,class Arg2,class Arg3,class Result = void >
class Callback3const : public Callback3constBase<Arg1,Arg2,Arg3,Result>
{
    const TClass & m_c;
    Core::stdext::const_mem_fun3_ref_t<Result,TClass,Arg1,Arg2,Arg3> m_f;
public:
    Callback3const (const TClass & c, Result (TClass::*f) (Arg1,Arg2,Arg3) const )
        : m_c(c),m_f(Core::stdext::mem_fun_ref(f))
    {}
    virtual Result operator()(Arg1 a1, Arg2 a2, Arg3 a3) const
    {
        return m_f(m_c,a1,a2,a3);
    }
    virtual Result call(Arg1 a1,Arg2 a2, Arg3 a3) const
    {
        return m_f(m_c,a1,a2,a3);
    }
};

/// 4 Arguments
template <class Arg1, class Arg2, class Arg3, class Arg4, class Result = void>
class Callback4Base
{
public:
    virtual Result operator()(Arg1, Arg2, Arg3, Arg4) = 0 ;
    virtual Result call(Arg1, Arg2, Arg3, Arg4) = 0;
};

template <class TClass, class Arg1, class Arg2, class Arg3, class Arg4, class Result = void >
class Callback4 : public Callback4Base<Arg1,Arg2,Arg3,Arg4,Result>
{
    TClass & m_c;
    Core::stdext::mem_fun4_ref_t<Result,TClass,Arg1,Arg2,Arg3,Arg4> m_f;
public:
    Callback4 ( TClass & c, Result (TClass::*f) (Arg1,Arg2,Arg3,Arg4) )
        : m_c(c),m_f(Core::stdext::mem_fun_ref(f))
    {}
    virtual Result operator()(Arg1 a1,Arg2 a2,Arg3 a3, Arg4 a4)
    {
        return m_f(m_c,a1,a2,a3,a4);
    }
    virtual Result call(Arg1 a1,Arg2 a2,Arg3 a3, Arg4 a4)
    {
        return m_f(m_c,a1,a2,a3,a4);
    }

};

/// 4 Arguments const

template <class Arg1,class Arg2,class Arg3,class Arg4,class Result = void >
class Callback4constBase
{
public:
    virtual Result operator()( Arg1, Arg2, Arg3, Arg4) const = 0 ;
    virtual Result call( Arg1, Arg2, Arg3, Arg4) const = 0;
};


template <class TClass,class Arg1,class Arg2,class Arg3,class Arg4,class Result = void >
class Callback4const : public Callback4constBase<Arg1,Arg2,Arg3,Arg4,Result>
{
    const TClass & m_c;
    Core::stdext::const_mem_fun4_ref_t<Result,TClass,Arg1,Arg2,Arg3,Arg4> m_f;
public:
    Callback4const (const TClass & c, Result (TClass::*f) (Arg1,Arg2,Arg3,Arg4) const )
        : m_c(c),m_f(Core::stdext::mem_fun_ref(f))
    {}
    virtual Result operator()(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4) const
    {
        return m_f(m_c,a1,a2,a3,a4);
    }
    virtual Result call(Arg1 a1,Arg2 a2, Arg3 a3,Arg4 a4) const
    {
        return m_f(m_c,a1,a2,a3,a4);
    }

};


/// 5 Arguments

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Result = void>
class Callback5Base
{
public:
    virtual Result operator()(Arg1, Arg2, Arg3, Arg4, Arg5) = 0 ;
    virtual Result call(Arg1, Arg2, Arg3, Arg4, Arg5) = 0;
};

template <class TClass, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Result = void >
class Callback5 : public Callback5Base<Arg1,Arg2,Arg3,Arg4,Arg5,Result>
{
    TClass & m_c;
    Core::stdext::mem_fun5_ref_t<Result,TClass,Arg1,Arg2,Arg3,Arg4,Arg5> m_f;
public:
    Callback5 ( TClass & c, Result (TClass::*f) (Arg1,Arg2,Arg3,Arg4,Arg5) )
        : m_c(c),m_f(Core::stdext::mem_fun_ref(f))
    {}
    virtual Result operator()(Arg1 a1,Arg2 a2,Arg3 a3, Arg4 a4, Arg5 a5)
    {
        return m_f(m_c,a1,a2,a3,a4,a5);
    }
    virtual Result call(Arg1 a1,Arg2 a2,Arg3 a3, Arg4 a4, Arg5 a5)
    {
        return m_f(m_c,a1,a2,a3,a4,a5);
    }

};

/// 5 Arguments const

template <class Arg1,class Arg2,class Arg3,class Arg4,class Arg5,class Result = void >
class Callback5constBase
{
public:
    virtual Result operator()( Arg1, Arg2, Arg3, Arg4, Arg5) const = 0 ;
    virtual Result call( Arg1, Arg2, Arg3, Arg4, Arg5) const = 0;
};


template <class TClass,class Arg1,class Arg2,class Arg3,class Arg4,class Arg5,class Result = void >
class Callback5const : public Callback5constBase<Arg1,Arg2,Arg3,Arg4,Arg5,Result>
{
    const TClass & m_c;
    Core::stdext::const_mem_fun5_ref_t<Result,TClass,Arg1,Arg2,Arg3,Arg4,Arg5> m_f;
public:
    Callback5const (const TClass & c, Result (TClass::*f) (Arg1,Arg2,Arg3,Arg4,Arg5) const )
        : m_c(c),m_f(Core::stdext::mem_fun_ref(f))
    {}
    virtual Result operator()(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5) const
    {
        return m_f(m_c,a1,a2,a3,a4,a5);
    }
    virtual Result call(Arg1 a1,Arg2 a2, Arg3 a3,Arg4 a4, Arg5 a5) const
    {
        return m_f(m_c,a1,a2,a3,a4,a5);
    }

};

/// 6 Arguments
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Result = void>
class Callback6Base
{
public:
    virtual Result operator()(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) = 0 ;
    virtual Result call(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) = 0;
};

template <class TClass, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Result = void >
class Callback6 : public Callback6Base<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>
{
    TClass & m_c;
    Core::stdext::mem_fun6_ref_t<Result,TClass,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6> m_f;
public:
    Callback6 ( TClass & c, Result (TClass::*f) (Arg1,Arg2,Arg3,Arg4,Arg5,Arg6) )
        : m_c(c),m_f(Core::stdext::mem_fun_ref(f))
    {}
    virtual Result operator()(Arg1 a1,Arg2 a2,Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6)
    {
        return m_f(m_c,a1,a2,a3,a4,a5,a6);
    }
    virtual Result call(Arg1 a1,Arg2 a2,Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6)
    {
        return m_f(m_c,a1,a2,a3,a4,a5,a6);
    }
};

/// 6 Arguments const

template <class Arg1,class Arg2,class Arg3,class Arg4,class Arg5,class Arg6,class Result = void >
class Callback6constBase
{
public:
    virtual Result operator()( Arg1, Arg2, Arg3, Arg4, Arg5, Arg6 ) const = 0 ;
    virtual Result call( Arg1, Arg2, Arg3, Arg4, Arg5, Arg6 ) const = 0;
};


template <class TClass,class Arg1,class Arg2,class Arg3,class Arg4,class Arg5,class Arg6,class Result = void >
class Callback6const : public Callback6constBase<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>
{
    const TClass & m_c;
    Core::stdext::const_mem_fun6_ref_t<Result,TClass,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6> m_f;
public:
    Callback6const (const TClass & c, Result (TClass::*f) (Arg1,Arg2,Arg3,Arg4,Arg5,Arg6) const )
        : m_c(c),m_f(Core::stdext::mem_fun_ref(f))
    {}
    virtual Result operator()(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6) const
    {
        return m_f(m_c,a1,a2,a3,a4,a5,a6);
    }
    virtual Result call(Arg1 a1,Arg2 a2, Arg3 a3,Arg4 a4, Arg5 a5, Arg6 a6) const
    {
        return m_f(m_c,a1,a2,a3,a4,a5,a6);
    }
};




} // Core

#endif
