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

#ifndef CORE_FUNCTOREXT_HH
#define CORE_FUNCTOREXT_HH


#include <functional>

/**
 * Implements extension to functional objects already existing in STL
 *
 */

namespace Core
{

//need to extend std:: to support mem_fun, ptr_fun for more arguments
namespace stdext
{

template <class Result>
struct nullary_function
{
    typedef Result result_type;
};

///Extension for 3 arguments

template <class Arg1, class Arg2, class Arg3, class Result>
struct ternary_function
{
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Arg3 third_argument_type;
    typedef Result result_type;
};

template <class Predicate> class ternary_negate
    : public ternary_function <typename Predicate::first_argument_type,
      typename Predicate::second_argument_type,
      typename Predicate::third_argument_type, bool>
{
protected:
    Predicate fn;
public:
    explicit ternary_negate ( const Predicate& pred ) : fn (pred) {}
    bool operator() (const typename Predicate::first_argument_type& x,
                     const typename Predicate::second_argument_type& y,
                     const typename Predicate::third_argument_type& z) const
    {
        return !fn(x,y,z);
    }
};


template <class Arg1, class Arg2, class Arg3, class Result>
class pointer_to_ternary_function : public ternary_function <Arg1,Arg2,Arg3,Result>
{
protected:
    Result(*pfunc)(Arg1,Arg2,Arg3);
public:
    explicit pointer_to_ternary_function ( Result (*f)(Arg1,Arg2,Arg3) ) : pfunc (f) {}
    Result operator() (Arg1 x, Arg2 y, Arg3 z) const
    {
        return pfunc(x,y,z);
    }
};

template <class S, class T, class A, class B>
class mem_fun2_t : public ternary_function <T*,A,B,S>
{
    S (T::*pmem)(A,B);
public:
    explicit mem_fun2_t ( S (T::*p)(A,B) ) : pmem (p) {}
    S operator() (T* p, A x, B y) const
    {
        return (p->*pmem)(x,y);
    }
};



template <class S, class T, class A, class B>
class const_mem_fun2_t : public ternary_function <T*,A,B,S>
{
    S (T::*pmem)(A,B) const;
public:
    explicit const_mem_fun2_t ( S (T::*p)(A,B) const ) : pmem (p) {}
    S operator() (T* p, A x, B y) const
    {
        return (p->*pmem)(x,y);
    }
};

template <class S, class T, class A, class B> mem_fun2_t<S,T,A,B> mem_fun (S (T::*f)(A,B))
{
    return mem_fun2_t<S,T,A,B>(f);
}


template <class S, class T, class A, class B> const_mem_fun2_t<S,T,A,B> mem_fun (S (T::*f)(A,B) const)
{
    return const_mem_fun2_t<S,T,A,B>(f);
}

template <class S, class T, class A, class B>
class mem_fun2_ref_t : public ternary_function <T,A,B,S>
{
    S (T::*pmem)(A,B);
public:
    explicit mem_fun2_ref_t ( S (T::*p)(A,B) ) : pmem (p) {}
    S operator() (T& p, A x, B y) const
    {
        return (p.*pmem)(x,y);
    }
};



template <class S, class T, class A, class B>
class const_mem_fun2_ref_t : public ternary_function <T,A,B,S>
{
    S (T::*pmem)(A,B) const;
public:
    explicit const_mem_fun2_ref_t ( S (T::*p)(A,B) const ) : pmem (p) {}
    S operator() (const T& p, A x, B y) const
    {
        return (p.*pmem)(x,y);
    }
};

template <class S, class T, class A, class B> mem_fun2_ref_t<S,T,A,B> mem_fun_ref (S (T::*f)(A,B))
{
    return mem_fun2_ref_t<S,T,A,B>(f);
}


template <class S, class T, class A, class B> const_mem_fun2_ref_t<S,T,A,B> mem_fun_ref (S (T::*f)(A,B) const)
{
    return const_mem_fun2_ref_t<S,T,A,B>(f);
}

/*
template <class Operation> class binder1st
            : public binary_function <typename Operation::second_argument_type,
            typename Operation::third_argument_type,
            typename Operation::result_type>
{
protected:
    Operation op;
    typename Operation::first_argument_type value;
public:
    binder1st ( const Operation& x,
                const typename Operation::first_argument_type& y) : op (x), value(y) {}
    typename Operation::result_type
    operator() (const typename Operation::second_argument_type& x,
                const typename Operation::thirdargument_type & y) const
    {
        return op(value,x,y);
    }
};
*/

///Extension for 4 arguments

template <class Arg1, class Arg2, class Arg3, class Arg4, class Result>
struct quaternary_function
{
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Arg3 third_argument_type;
    typedef Arg4 forth_argument_type;
    typedef Result result_type;
};


template <class Predicate> class quaternary_negate
    : public quaternary_function <typename Predicate::first_argument_type,
      typename Predicate::second_argument_type,
      typename Predicate::third_argument_type,
      typename Predicate::forth_argument_type, bool>
{
protected:
    Predicate fn;
public:
    explicit quaternary_negate ( const Predicate& pred ) : fn (pred) {}
    bool operator() (const typename Predicate::first_argument_type& x,
                     const typename Predicate::second_argument_type& y,
                     const typename Predicate::third_argument_type& z,
                     const typename Predicate::forth_argument_type& t) const
    {
        return !fn(x,y,z,t);
    }
};


template <class Arg1, class Arg2, class Arg3, class Arg4, class Result>
class pointer_to_quaternary_function : public quaternary_function <Arg1,Arg2,Arg3,Arg4,Result>
{
protected:
    Result(*pfunc)(Arg1,Arg2,Arg3,Arg4);
public:
    explicit pointer_to_quaternary_function ( Result (*f)(Arg1,Arg2,Arg3,Arg4) ) : pfunc (f) {}
    Result operator() (Arg1 x, Arg2 y, Arg3 z, Arg4 t) const
    {
        return pfunc(x,y,z,t);
    }
};


template <class S, class T, class A, class B, class C>
class mem_fun3_t : public quaternary_function <T*,A,B,C,S>
{
    S (T::*pmem)(A,B,C);
public:
    explicit mem_fun3_t ( S (T::*p)(A,B,C) ) : pmem (p) {}
    S operator() (T* p, A x, B y, C z) const
    {
        return (p->*pmem)(x,y,z);
    }
};


template <class S, class T, class A, class B, class C>
class const_mem_fun3_t : public quaternary_function <T*,A,B,C,S>
{
    S (T::*pmem)(A,B,C)const;
public:
    explicit const_mem_fun3_t ( S (T::*p)(A,B,C) const ) : pmem (p) {}
    S operator() (const T* p, A x, B y, C z) const
    {
        return (p->*pmem)(x,y,z);
    }
};

template <class S, class T, class A, class B, class C> const_mem_fun3_t<S,T,A,B,C> mem_fun (S (T::*f)(A,B,C)const)
{
    return const_mem_fun3_t<S,T,A,B,C>(f);
}

template <class S, class T, class A, class B, class C> mem_fun3_t<S,T,A,B,C> mem_fun (S (T::*f)(A,B,C))
{
    return mem_fun3_t<S,T,A,B,C>(f);
}



template <class S, class T, class A, class B, class C>
class mem_fun3_ref_t : public quaternary_function <T,A,B,C,S>
{
    S (T::*pmem)(A,B,C);
public:
    explicit mem_fun3_ref_t ( S (T::*p)(A,B,C) ) : pmem (p) {}
    S operator() (T& p, A x, B y, C z) const
    {
        return (p.*pmem)(x,y,z);
    }
};


template <class S, class T, class A, class B, class C>
class const_mem_fun3_ref_t : public quaternary_function <T,A,B,C,S>
{
    S (T::*pmem)(A,B,C)const;
public:
    explicit const_mem_fun3_ref_t ( S (T::*p)(A,B,C) const ) : pmem (p) {}
    S operator() (const T& p, A x, B y, C z) const
    {
        return (p.*pmem)(x,y,z);
    }
};

template <class S, class T, class A, class B, class C> const_mem_fun3_ref_t<S,T,A,B,C> mem_fun_ref (S (T::*f)(A,B,C)const)
{
    return const_mem_fun3_ref_t<S,T,A,B,C>(f);
}

template <class S, class T, class A, class B, class C> mem_fun3_ref_t<S,T,A,B,C> mem_fun_ref (S (T::*f)(A,B,C))
{
    return mem_fun3_ref_t<S,T,A,B,C>(f);
}

/*
template <class Operation> class binder1st
            : public binary_function <typename Operation::second_argument_type,
            typename Operation::third_argument_type,
            typename Operation::forth_argument_type,
            typename Operation::result_type>
{
protected:
    Operation op;
    typename Operation::first_argument_type value;
public:
    binder1st ( const Operation& x,
                const typename Operation::first_argument_type& y) : op (x), value(y) {}
    typename Operation::result_type
    operator() (const typename Operation::second_argument_type& x,
                const typename Operation::third_argument_type & y,
                const typename Operation::forth_argument_type & z) const
    {
        return op(value,x,y,z);
    }
};
*/

///Extension for 5 arguments

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Result>
struct quinary_function
{
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Arg3 third_argument_type;
    typedef Arg4 forth_argument_type;
    typedef Arg5 fifth_argument_type;
    typedef Result result_type;
};


template <class Predicate> class quinary_negate
    : public quinary_function <typename Predicate::first_argument_type,
      typename Predicate::second_argument_type,
      typename Predicate::third_argument_type,
      typename Predicate::forth_argument_type,
      typename Predicate::fifth_argument_type, bool>
{
protected:
    Predicate fn;
public:
    explicit quinary_negate ( const Predicate& pred ) : fn (pred) {}
    bool operator() (const typename Predicate::first_argument_type& x,
                     const typename Predicate::second_argument_type& y,
                     const typename Predicate::third_argument_type& z,
                     const typename Predicate::forth_argument_type & t,
                     const typename Predicate::fifth_argument_type & u) const
    {
        return !fn(x,y,z,t,u);
    }
};



template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Result>
class pointer_to_quinary_function : public quinary_function <Arg1,Arg2,Arg3,Arg4,Arg5,Result>
{
protected:
    Result(*pfunc)(Arg1,Arg2,Arg3,Arg4,Arg5);
public:
    explicit pointer_to_quinary_function ( Result (*f)(Arg1,Arg2,Arg3,Arg4,Arg5) ) : pfunc (f) {}
    Result operator() (Arg1 x, Arg2 y, Arg3 z, Arg4 t, Arg5 u) const
    {
        return pfunc(x,y,z,t,u);
    }
};



template <class S, class T, class A, class B, class C, class D>
class mem_fun4_t : public quinary_function <T*,A,B,C,D,S>
{
    S (T::*pmem)(A,B,C,D);
public:
    explicit mem_fun4_t ( S (T::*p)(A,B,C,D) ) : pmem (p) {}
    S operator() (T* p, A x, B y, C z, D t) const
    {
        return (p->*pmem)(x,y,z,t);
    }
};
template <class S, class T, class A, class B, class C, class D>
class const_mem_fun4_t : public quinary_function <T*,A,B,C,D,S>
{
    S (T::*pmem)(A,B,C,D)const;
public:
    explicit const_mem_fun4_t ( S (T::*p)(A,B,C,D) const ) : pmem (p) {}
    S operator() (const T* p, A x, B y, C z, D t) const
    {
        return (p->*pmem)(x,y,z,t);
    }
};

template <class S, class T, class A, class B, class C, class D> const_mem_fun4_t<S,T,A,B,C,D> mem_fun (S (T::*f)(A,B,C,D)const )
{
    return const_mem_fun4_t<S,T,A,B,C,D>(f);
}

template <class S, class T, class A, class B, class C, class D> mem_fun4_t<S,T,A,B,C,D> mem_fun (S (T::*f)(A,B,C,D))
{
    return mem_fun4_t<S,T,A,B,C,D>(f);
}


template <class S, class T, class A, class B, class C, class D>
class mem_fun4_ref_t : public quinary_function <T,A,B,C,D,S>
{
    S (T::*pmem)(A,B,C,D);
public:
    explicit mem_fun4_ref_t ( S (T::*p)(A,B,C,D) ) : pmem (p) {}
    S operator() (T& p, A x, B y, C z, D t) const
    {
        return (p.*pmem)(x,y,z,t);
    }
};
template <class S, class T, class A, class B, class C, class D>
class const_mem_fun4_ref_t : public quinary_function <T,A,B,C,D,S>
{
    S (T::*pmem)(A,B,C,D)const;
public:
    explicit const_mem_fun4_ref_t ( S (T::*p)(A,B,C,D) const ) : pmem (p) {}
    S operator() (const T& p, A x, B y, C z, D t) const
    {
        return (p.*pmem)(x,y,z,t);
    }
};

template <class S, class T, class A, class B, class C, class D> const_mem_fun4_ref_t<S,T,A,B,C,D> mem_fun_ref (S (T::*f)(A,B,C,D)const )
{
    return const_mem_fun4_ref_t<S,T,A,B,C,D>(f);
}

template <class S, class T, class A, class B, class C, class D> mem_fun4_ref_t<S,T,A,B,C,D> mem_fun_ref (S (T::*f)(A,B,C,D))
{
    return mem_fun4_ref_t<S,T,A,B,C,D>(f);
}

/*
template <class Operation> class binder1st
            : public quaternary_function <typename Operation::second_argument_type,
            typename Operation::third_argument_type,
            typename Operation::forth_argument_type,
            typename Operation::fifth_argument_type,
            typename Operation::result_type>
{
protected:
    Operation op;
    typename Operation::first_argument_type value;
public:
    binder1st ( const Operation& x,
                const typename Operation::first_argument_type& y) : op (x), value(y) {}
    typename Operation::result_type
    operator() (const typename Operation::second_argument_type& x,
                const typename Operation::third_argument_type & y,
                const typename Operation::forth_argument_type & z
                const typename Operation::fifth_argument_type & t) const
    {
        return op(value,x,y,z,t);
    }
};
*/


template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Result>
struct senary_function
{
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Arg3 third_argument_type;
    typedef Arg4 forth_argument_type;
    typedef Arg5 fifth_argument_type;
    typedef Arg6 sixth_argument_type;
    typedef Result result_type;
};


template <class Predicate> class senary_negate
    : public senary_function <typename Predicate::first_argument_type,
      typename Predicate::second_argument_type,
      typename Predicate::third_argument_type,
      typename Predicate::forth_argument_type,
      typename Predicate::fifth_argument_type,
      typename Predicate::sixth_argument_type, bool>
{
protected:
    Predicate fn;
public:
    explicit senary_negate ( const Predicate& pred ) : fn (pred) {}
    bool operator() (const typename Predicate::first_argument_type& x,
                     const typename Predicate::second_argument_type& y,
                     const typename Predicate::third_argument_type& z,
                     const typename Predicate::forth_argument_type & t,
                     const typename Predicate::fifth_argument_type & u,
                     const typename Predicate::sixth_argument_type & v) const
    {
        return !fn(x,y,z,t,u,v);
    }
};



template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Result>
class pointer_to_senary_function : public senary_function <Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>
{
protected:
    Result(*pfunc)(Arg1,Arg2,Arg3,Arg4,Arg5,Arg6);
public:
    explicit pointer_to_senary_function ( Result (*f)(Arg1,Arg2,Arg3,Arg4,Arg5,Arg6) ) : pfunc (f) {}
    Result operator() (Arg1 x, Arg2 y, Arg3 z, Arg4 t, Arg5 u, Arg6 v) const
    {
        return pfunc(x,y,z,t,u,v);
    }
};



template <class S, class T, class A, class B, class C, class D, class E>
class mem_fun5_t : public senary_function <T*,A,B,C,D,E,S>
{
    S (T::*pmem)(A,B,C,D,E);
public:
    explicit mem_fun5_t ( S (T::*p)(A,B,C,D,E) ) : pmem (p) {}
    S operator() (T* p, A x, B y, C z, D t, E u) const
    {
        return (p->*pmem)(x,y,z,t,u);
    }
};
template <class S, class T, class A, class B, class C, class D, class E>
class const_mem_fun5_t : public senary_function <T*,A,B,C,D,E,S>
{
    S (T::*pmem)(A,B,C,D,E)const;
public:
    explicit const_mem_fun5_t ( S (T::*p)(A,B,C,D,E) const ) : pmem (p) {}
    S operator() (const T* p, A x, B y, C z, D t,E u) const
    {
        return (p->*pmem)(x,y,z,t,u);
    }
};

template <class S, class T, class A, class B, class C, class D, class E> const_mem_fun5_t<S,T,A,B,C,D,E> mem_fun (S (T::*f)(A,B,C,D,E)const )
{
    return const_mem_fun5_t<S,T,A,B,C,D,E>(f);
}

template <class S, class T, class A, class B, class C, class D, class E> mem_fun5_t<S,T,A,B,C,D,E> mem_fun (S (T::*f)(A,B,C,D,E))
{
    return mem_fun5_t<S,T,A,B,C,D,E>(f);
}


template <class S, class T, class A, class B, class C, class D, class E>
class mem_fun5_ref_t : public senary_function <T,A,B,C,D,E,S>
{
    S (T::*pmem)(A,B,C,D,E);
public:
    explicit mem_fun5_ref_t ( S (T::*p)(A,B,C,D,E) ) : pmem (p) {}
    S operator() (T& p, A x, B y, C z, D t,E u) const
    {
        return (p.*pmem)(x,y,z,t,u);
    }
};
template <class S, class T, class A, class B, class C, class D, class E>
class const_mem_fun5_ref_t : public senary_function <T,A,B,C,D,E,S>
{
    S (T::*pmem)(A,B,C,D,E)const;
public:
    explicit const_mem_fun5_ref_t ( S (T::*p)(A,B,C,D,E) const ) : pmem (p) {}
    S operator() (const T& p, A x, B y, C z, D t, E u) const
    {
        return (p.*pmem)(x,y,z,t,u);
    }
};

template <class S, class T, class A, class B, class C, class D, class E> const_mem_fun5_ref_t<S,T,A,B,C,D,E> mem_fun_ref (S (T::*f)(A,B,C,D,E)const )
{
    return const_mem_fun5_ref_t<S,T,A,B,C,D,E>(f);
}

template <class S, class T, class A, class B, class C, class D, class E> mem_fun5_ref_t<S,T,A,B,C,D,E> mem_fun_ref (S (T::*f)(A,B,C,D,E))
{
    return mem_fun5_ref_t<S,T,A,B,C,D,E>(f);
}

/* TODO !!
template <class Operation> class binder1st
            : public quaternary_function <typename Operation::second_argument_type,
            typename Operation::third_argument_type,
            typename Operation::forth_argument_type,
            typename Operation::fifth_argument_type,
            typename Operation::result_type>
{
protected:
    Operation op;
    typename Operation::first_argument_type value;
public:
    binder1st ( const Operation& x,
                const typename Operation::first_argument_type& y) : op (x), value(y) {}
    typename Operation::result_type
    operator() (const typename Operation::second_argument_type& x,
                const typename Operation::third_argument_type & y,
                const typename Operation::forth_argument_type & z
                const typename Operation::fifth_argument_type & t) const
    {
        return op(value,x,y,z,t);
    }
};
*/


template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Result>
struct septenary_function
{
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Arg3 third_argument_type;
    typedef Arg4 forth_argument_type;
    typedef Arg5 fifth_argument_type;
    typedef Arg6 sixth_argument_type;
    typedef Arg7 seventh_argument_type;
    typedef Result result_type;
};


template <class Predicate> class septenary_negate
    : public septenary_function <typename Predicate::first_argument_type,
      typename Predicate::second_argument_type,
      typename Predicate::third_argument_type,
      typename Predicate::forth_argument_type,
      typename Predicate::fifth_argument_type,
      typename Predicate::sixth_argument_type,
      typename Predicate::sieventh_argument_type, bool>
{
protected:
    Predicate fn;
public:
    explicit septenary_negate ( const Predicate& pred ) : fn (pred) {}
    bool operator() (const typename Predicate::first_argument_type& x,
                     const typename Predicate::second_argument_type& y,
                     const typename Predicate::third_argument_type& z,
                     const typename Predicate::forth_argument_type & t,
                     const typename Predicate::fifth_argument_type & u,
                     const typename Predicate::sixth_argument_type & v,
                     const typename Predicate::seventh_argument_type & w) const
    {
        return !fn(x,y,z,t,u,v,w);
    }
};



template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Result>
class pointer_to_septenary_function : public septenary_function <Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>
{
protected:
    Result(*pfunc)(Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7);
public:
    explicit pointer_to_septenary_function ( Result (*f)(Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7) ) : pfunc (f) {}
    Result operator() (Arg1 x, Arg2 y, Arg3 z, Arg4 t, Arg5 u, Arg6 v,Arg7 w) const
    {
        return pfunc(x,y,z,t,u,v,w);
    }
};



template <class S, class T, class A, class B, class C, class D, class E, class F>
class mem_fun6_t : public septenary_function <T*,A,B,C,D,E,F,S>
{
    S (T::*pmem)(A,B,C,D,E,F);
public:
    explicit mem_fun6_t ( S (T::*p)(A,B,C,D,E,F) ) : pmem (p) {}
    S operator() (T* p, A x, B y, C z, D t, E u, F v) const
    {
        return (p->*pmem)(x,y,z,t,u,v);
    }
};
template <class S, class T, class A, class B, class C, class D, class E, class F>
class const_mem_fun6_t : public septenary_function <T*,A,B,C,D,E,F,S>
{
    S (T::*pmem)(A,B,C,D,E,F)const;
public:
    explicit const_mem_fun6_t ( S (T::*p)(A,B,C,D,E,F) const ) : pmem (p) {}
    S operator() (const T* p, A x, B y, C z, D t, E u, F v) const
    {
        return (p->*pmem)(x,y,z,t,u,v);
    }
};

template <class S, class T, class A, class B, class C, class D, class E, class F> const_mem_fun6_t<S,T,A,B,C,D,E,F> mem_fun (S (T::*f)(A,B,C,D,E,F)const )
{
    return const_mem_fun6_t<S,T,A,B,C,D,E,F>(f);
}

template <class S, class T, class A, class B, class C, class D, class E, class F> mem_fun6_t<S,T,A,B,C,D,E,F> mem_fun (S (T::*f)(A,B,C,D,E,F))
{
    return mem_fun6_t<S,T,A,B,C,D,E,F>(f);
}


template <class S, class T, class A, class B, class C, class D, class E, class F>
class mem_fun6_ref_t : public septenary_function <T,A,B,C,D,E,F,S>
{
    S (T::*pmem)(A,B,C,D,E,F);
public:
    explicit mem_fun6_ref_t ( S (T::*p)(A,B,C,D,E,F) ) : pmem (p) {}
    S operator() (T& p, A x, B y, C z, D t, E u, F v) const
    {
        return (p.*pmem)(x,y,z,t,u,v);
    }
};
template <class S, class T, class A, class B, class C, class D, class E, class F>
class const_mem_fun6_ref_t : public septenary_function <T,A,B,C,D,E,F,S>
{
    S (T::*pmem)(A,B,C,D,E,F)const;
public:
    explicit const_mem_fun6_ref_t ( S (T::*p)(A,B,C,D,E,F) const ) : pmem (p) {}
    S operator() (const T& p, A x, B y, C z, D t, E u, F v) const
    {
        return (p.*pmem)(x,y,z,t,u,v);
    }
};

template <class S, class T, class A, class B, class C, class D, class E, class F> const_mem_fun6_ref_t<S,T,A,B,C,D,E,F> mem_fun_ref (S (T::*f)(A,B,C,D,E,F)const )
{
    return const_mem_fun6_ref_t<S,T,A,B,C,D,E,F>(f);
}

template <class S, class T, class A, class B, class C, class D, class E, class F> mem_fun6_ref_t<S,T,A,B,C,D,E,F> mem_fun_ref (S (T::*f)(A,B,C,D,E,F))
{
    return mem_fun6_ref_t<S,T,A,B,C,D,E,F>(f);
}

} // stdext

} //Core

#endif
