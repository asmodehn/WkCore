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

#ifndef ADAPTER_HH
#define ADAPTER_HH

/**
 * Adaptors can be used to pass a callback to a legacy C library that might call it later on.
 * One adaptor for one set of arguments, ie. for one call of the callback associated with that adaptor
 */

#include "Callback.hh"

#include <stdexcept>
#include <iostream>

namespace Core
{

template<class Result=void>
class Adapter1
{
    Callback1Base<void*,Result> * m_cb;
    std::pair< Adapter1<Result>*,void*> arguments;
	Result m_r;

public:
    Adapter1( Callback1Base<void*,Result> * cb )
        : m_cb(cb)
    {}

	Adapter1(const Adapter1<Result> & ad)
		: m_cb(ad.m_cb)
	{}

	virtual ~Adapter1()
	{}

	//TODO : check this
	//as per http://stackoverflow.com/questions/2068022/in-c-is-it-safe-portable-to-use-static-member-function-pointer-for-c-api-call
	//extern "C"
	//{
    static Result adapt( void * v )
    {
        Result r;
        try
        {
            std::pair< Adapter1<Result>*,void*>* vpair = static_cast< std::pair< Adapter1<Result>*,void*>*> (v);
            Adapter1<Result>* pad = reinterpret_cast<Adapter1<Result>*>(vpair->first);
			pad->m_r = pad->m_cb->call(vpair->second);
			r = pad->m_r;
        }
        catch ( std::exception & e)
        {
            //TODO : manage exceptions here
            std::cerr << e.what();
        }
        return r;
    }
	//}

    void* build_voidP_args(void* vp)
    {
        arguments = std::make_pair(this,vp);
        return static_cast<void*>( &arguments);
    }

	Result getResult()
	{
		return m_r;
	}
};

template<class Result=void>
class Adapter1const
{
    Callback1constBase<void*,Result> * m_cb;
    std::pair< Adapter1const<Result>*,void*> arguments;
	Result m_r;

public:
    Adapter1const( Callback1constBase<void*,Result> * cb )
        : m_cb(cb)
    {}

	Adapter1const(const Adapter1const<Result> & ad)
		: m_cb(ad.m_cb)
	{}

	virtual ~Adapter1const()
	{}

    static Result adapt( void * v )
    {
        Result r;
        try
        {
            std::pair< Adapter1const<Result>*,void*>* vpair = static_cast< std::pair< Adapter1const<Result>*,void*>*> (v);
            Adapter1const<Result>* pad = reinterpret_cast<Adapter1const<Result>*>(vpair->first);
            pad->m_r = pad->m_cb->call(vpair->second);
			r = pad->m_r;
        }
        catch ( std::exception & e)
        {
            //TODO : manage exceptions here
            std::cerr << e.what();
        }
        return r;
    }

    void* build_voidP_args(void* vp)
    {
        arguments = std::make_pair(this,vp);
        return static_cast<void*>( &arguments);
    }
	
	Result getResult()
	{
		return m_r;
	}
};

/// 2 arguments
template<class Arg1, class Result=void>
class Adapter2
{
    Callback2Base<Arg1,void*,Result> * m_cb;
    std::pair< Adapter2<Arg1,Result>*,void*> arguments;
	Result m_r;

public:
    Adapter2( Callback2Base<Arg1,void*,Result> * cb )
        : m_cb (cb)
    {}

	Adapter2(const Adapter2<Arg1,Result> & ad)
		: m_cb(ad.m_cb)
	{}

	virtual ~Adapter2()
	{}

    static Result adapt(Arg1 a, void * v )
    {
        Result r;
        try
        {
            std::pair< Adapter2<Arg1,Result>*,void*>* vpair = static_cast< std::pair< Adapter2<Arg1,Result>*,void*>*> (v);
            Adapter2<Arg1,Result>* pad = reinterpret_cast<Adapter2<Arg1,Result>*>(vpair->first);
            pad->m_r = pad->m_cb->call(a,vpair->second);
			r = pad->m_r;
        }
        catch ( std::exception & e)
        {
            //TODO : manage exceptions here
            std::cerr << e.what();
        }
        return r;
    }

    void* build_voidP_args(void* vp)
    {
        arguments = std::make_pair(this,vp);
        return static_cast<void*>( &arguments);
    }

	Result getResult()
	{
		return m_r;
	}
};


template<class Arg1,class Result=void>
class Adapter2const
{
    Callback2constBase<Arg1,void*,Result> * m_cb;
    std::pair< Adapter2const<Arg1,Result>*,void*> arguments;
	Result m_r;

public:
    Adapter2const( Callback2constBase<Arg1,void*,Result> * cb )
        : m_cb(cb)
    {}

	Adapter2const(const Adapter2const<Arg1,Result> & ad)
		: m_cb(ad.m_cb)
	{}

	virtual ~Adapter2const()
	{}

    static Result adapt( Arg1 a, void * v )
    {
        Result r;
        try
        {
            std::pair< Adapter2const<Arg1,Result>*,void*>* vpair = static_cast< std::pair< Adapter2const<Arg1,Result>*,void*>*> (v);
            Adapter2const<Arg1,Result>* pad = reinterpret_cast<Adapter2const<Arg1,Result>*>(vpair->first);
            pad->m_r = pad->m_cb->call(a,vpair->second);
			r = pad->m_r;
        }
        catch ( std::exception & e)
        {
            //TODO : manage exceptions here
            std::cerr << e.what();
        }
        return r;
    }

    void* build_voidP_args(void* vp)
    {
        arguments = std::make_pair(this,vp);
        return static_cast<void*>( &arguments);
    }

	Result getResult()
	{
		return m_r;
	}
};


/// 3 arguments
template<class Arg1, class Arg2, class Result=void>
class Adapter3
{
    Callback3Base<Arg1,Arg2,void*,Result> * m_cb;
    std::pair< Adapter3<Arg1,Arg2,Result>*,void*> arguments;

public:
    Adapter3( Callback3Base<Arg1,Arg2,void*,Result> * cb )
        : m_cb (cb)
    {}

    static Result adapt(Arg1 a,Arg2 b,void * v )
    {
        Result r;
        try
        {
            std::pair< Adapter3<Arg1,Arg2,Result>*,void*>* vpair = static_cast< std::pair< Adapter3<Arg1,Arg2,Result>*,void*>*> (v);
            Adapter3<Arg1,Arg2,Result>* pad = reinterpret_cast<Adapter3<Arg1,Arg2,Result>*>(vpair->first);
            r=pad->m_cb->call(a,b,vpair->second);
        }
        catch ( std::exception & e)
        {
            //TODO : manage exceptions here
            std::cerr << e.what();
        }
        return r;
    }

    void* build_voidP_args(void* vp)
    {
        arguments = std::make_pair(this,vp);
        return static_cast<void*>( &arguments);
    }
};


template<class Arg1,class Arg2,class Result=void>
class Adapter3const
{
    Callback3constBase<Arg1,Arg2,void*,Result> * m_cb;
    std::pair< Adapter3const<Arg1,Arg2,Result>*,void*> arguments;

public:
    Adapter3const( Callback3constBase<Arg1,Arg2,void*,Result> * cb )
        : m_cb(cb)
    {}

    static Result adapt( Arg1 a, Arg2 b, void * v )
    {
        Result r;
        try
        {
            std::pair< Adapter3const<Arg1,Arg2,Result>*,void*>* vpair = static_cast< std::pair< Adapter3const<Arg1,Arg2,Result>*,void*>*> (v);
            Adapter3const<Arg1,Arg2,Result>* pad = reinterpret_cast<Adapter3const<Arg1,Arg2,Result>*>(vpair->first);
            r=pad->m_cb->call(a,b,vpair->second);
        }
        catch ( std::exception & e)
        {
            //TODO : manage exceptions here
            std::cerr << e.what();
        }
        return r;
    }
    void* build_voidP_args(void* vp)
    {
        arguments = std::make_pair(this,vp);
        return static_cast<void*>( &arguments);
    }
};

/// 4 arguments

template<class Arg1, class Arg2, class Arg3, class Result=void>
class Adapter4
{
    Callback4Base<Arg1,Arg2,Arg3,void*,Result> * m_cb;
    std::pair< Adapter4<Arg1,Arg2,Arg3,Result>*,void*> arguments;

public:
    Adapter4( Callback4Base<Arg1,Arg2,Arg3,void*,Result> * cb )
        : m_cb (cb)
    {}

    static Result adapt(Arg1 a,Arg2 b,Arg3 c,void * v )
    {
        Result r;
        try
        {
            std::pair< Adapter4<Arg1,Arg2,Arg3,Result>*,void*>* vpair = static_cast< std::pair< Adapter4<Arg1,Arg2,Arg3,Result>*,void*>*> (v);
            Adapter4<Arg1,Arg2,Arg3,Result>* pad = reinterpret_cast<Adapter4<Arg1,Arg2,Arg3,Result>*>(vpair->first);
            r=pad->m_cb->call(a,b,c,vpair->second);
        }
        catch ( std::exception & e)
        {
            //TODO : manage exceptions here
            std::cerr << e.what();
        }
        return r;
    }

    void* build_voidP_args(void* vp)
    {
        arguments = std::make_pair(this,vp);
        return static_cast<void*>( &arguments);
    }
};


template<class Arg1,class Arg2, class Arg3,class Result=void>
class Adapter4const
{
    Callback4constBase<Arg1,Arg2,Arg3,void*,Result> * m_cb;
    std::pair< Adapter4const<Arg1,Arg2,Arg3,Result>*,void*> arguments;

public:
    Adapter4const( Callback4constBase<Arg1,Arg2,Arg3,void*,Result> * cb )
        : m_cb(cb)
    {}

    static Result adapt( Arg1 a, Arg2 b, Arg3 c,void * v )
    {
        Result r;
        try
        {
            std::pair< Adapter4const<Arg1,Arg2,Arg3,Result>*,void*>* vpair = static_cast< std::pair< Adapter4const<Arg1,Arg2,Arg3,Result>*,void*>*> (v);
            Adapter4const<Arg1,Arg2,Arg3,Result>* pad = reinterpret_cast<Adapter4const<Arg1,Arg2,Arg3,Result>*>(vpair->first);
            r=pad->m_cb->call(a,b,c,vpair->second);
        }
        catch ( std::exception & e)
        {
            //TODO : manage exceptions here
            std::cerr << e.what();
        }
        return r;
    }
    void* build_voidP_args(void* vp)
    {
        arguments = std::make_pair(this,vp);
        return static_cast<void*>( &arguments);
    }
};

/// 5 arguments
template<class Arg1, class Arg2, class Arg3, class Arg4, class Result=void>
class Adapter5
{
    Callback5Base<Arg1,Arg2,Arg3,Arg4,void*,Result> * m_cb;
    std::pair< Adapter5<Arg1,Arg2,Arg3,Arg4,Result>*,void*> arguments;

public:
    Adapter5( Callback5Base<Arg1,Arg2,Arg3,Arg4,void*,Result> * cb )
        : m_cb (cb)
    {}

    static Result adapt(Arg1 a,Arg2 b,Arg3 c,Arg4 d,void * v )
    {
        Result r;
        try
        {
            std::pair< Adapter5<Arg1,Arg2,Arg3,Arg4,Result>*,void*>* vpair = static_cast< std::pair< Adapter5<Arg1,Arg2,Arg3,Arg4,Result>*,void*>*> (v);
            Adapter5<Arg1,Arg2,Arg3,Arg4,Result>* pad = reinterpret_cast<Adapter5<Arg1,Arg2,Arg3,Arg4,Result>*>(vpair->first);
            r=pad->m_cb->call(a,b,c,d,vpair->second);
        }
        catch ( std::exception & e)
        {
            //TODO : manage exceptions here
            std::cerr << e.what();
        }
        return r;
    }

    void* build_voidP_args(void* vp)
    {
        arguments = std::make_pair(this,vp);
        return static_cast<void*>( &arguments);
    }
};


template<class Arg1,class Arg2, class Arg3,class Arg4,class Result=void>
class Adapter5const
{
    Callback5constBase<Arg1,Arg2,Arg3,Arg4,void*,Result> * m_cb;
    std::pair< Adapter5const<Arg1,Arg2,Arg3,Arg4,Result>*,void*> arguments;

public:
    Adapter5const( Callback5constBase<Arg1,Arg2,Arg3,Arg4,void*,Result> * cb )
        : m_cb(cb)
    {}

    static Result adapt( Arg1 a, Arg2 b, Arg3 c,Arg4 d,void * v )
    {
        Result r;
        try
        {
            std::pair< Adapter5const<Arg1,Arg2,Arg3,Arg4,Result>*,void*>* vpair = static_cast< std::pair< Adapter5const<Arg1,Arg2,Arg3,Arg4,Result>*,void*>*> (v);
            Adapter5const<Arg1,Arg2,Arg3,Arg4,Result>* pad = reinterpret_cast<Adapter5const<Arg1,Arg2,Arg3,Arg4,Result>*>(vpair->first);
            r=pad->m_cb->call(a,b,c,d,vpair->second);
        }
        catch ( std::exception & e)
        {
            //TODO : manage exceptions here
            std::cerr << e.what();
        }
        return r;
    }
    void* build_voidP_args(void* vp)
    {
        arguments = std::make_pair(this,vp);
        return static_cast<void*>( &arguments);
    }
};

/// 6 arguments const
template<class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Result=void>
class Adapter6
{
    Callback6Base<Arg1,Arg2,Arg3,Arg4,Arg5,void*,Result> * m_cb;
    std::pair< Adapter6<Arg1,Arg2,Arg3,Arg4,Arg5,Result>*,void*> arguments;

public:
    Adapter6( Callback6Base<Arg1,Arg2,Arg3,Arg4,Arg5,void*,Result> * cb )
        : m_cb (cb)
    {}

    static Result adapt(Arg1 a,Arg2 b,Arg3 c,Arg4 d,Arg5 e,void * v )
    {
        Result r;
        try
        {
            std::pair< Adapter6<Arg1,Arg2,Arg3,Arg4,Arg5,Result>*,void*>* vpair = static_cast< std::pair< Adapter6<Arg1,Arg2,Arg3,Arg4,Arg5,Result>*,void*>*> (v);
            Adapter6<Arg1,Arg2,Arg3,Arg4,Arg5,Result>* pad = reinterpret_cast<Adapter6<Arg1,Arg2,Arg3,Arg4,Arg5,Result>*>(vpair->first);
            r=pad->m_cb->call(a,b,c,d,e,vpair->second);
        }
        catch ( std::exception & e)
        {
            //TODO : manage exceptions here
            std::cerr << e.what();
        }
        return r;
    }

    void* build_voidP_args(void* vp)
    {
        arguments = std::make_pair(this,vp);
        return static_cast<void*>( &arguments);
    }
};


template<class Arg1,class Arg2, class Arg3,class Arg4,class Arg5,class Result=void>
class Adapter6const
{
    Callback6constBase<Arg1,Arg2,Arg3,Arg4,Arg5,void*,Result> * m_cb;
    std::pair< Adapter6const<Arg1,Arg2,Arg3,Arg4,Arg5,Result>*,void*> arguments;

public:
    Adapter6const( Callback6constBase<Arg1,Arg2,Arg3,Arg4,Arg5,void*,Result> * cb )
        : m_cb(cb)
    {}

    static Result adapt( Arg1 a, Arg2 b, Arg3 c,Arg4 d,Arg5 e,void * v )
    {
        Result r;
        try
        {
            std::pair< Adapter6const<Arg1,Arg2,Arg3,Arg4,Arg5,Result>*,void*>* vpair = static_cast< std::pair< Adapter6const<Arg1,Arg2,Arg3,Arg4,Arg5,Result>*,void*>*> (v);
            Adapter6const<Arg1,Arg2,Arg3,Arg4,Arg5,Result>* pad = reinterpret_cast<Adapter6const<Arg1,Arg2,Arg3,Arg4,Arg5,Result>*>(vpair->first);
            r=pad->m_cb->call(a,b,c,d,e,vpair->second);
        }
        catch ( std::exception & e)
        {
            //TODO : manage exceptions here
            std::cerr << e.what();
        }
        return r;
    }
    void* build_voidP_args(void* vp)
    {
        arguments = std::make_pair(this,vp);
        return static_cast<void*>( &arguments);
    }
};

} // Core

#endif
