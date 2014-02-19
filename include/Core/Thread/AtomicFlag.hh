/**
 * Copyright (c) 2009-2014, Asmodehn's Corp.
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

#ifndef __CORE_THREAD_ATOMICFLAG_HH
#define __CORE_THREAD_ATOMICFLAG_HH

#ifdef TINYTHREAD_REQUIRED
# include "tinythreadpp/source/tinythread.h"
# define CORE_THREAD_PACKAGE tthread
#else
# include <atomic>
# define CORE_THREAD_PACKAGE std
#endif

namespace Core
{
	namespace Thread
	{

#define CORE_ATOMIC_FLAG_INIT 0

/// Atomic flag class.
/// This is an atomic boolean object that provides methods for atmically
/// testing, setting and clearing the state of the object. It can be used
/// for implementing user space spin-locks, for instance.
class AtomicFlag : public CORE_THREAD_PACKAGE::atomic_flag
{
  public:

	//constructor to simulate value construction via flag init
	//also default constructor because of default arguments
	AtomicFlag( int atomic_flag_init = CORE_ATOMIC_FLAG_INIT) :  CORE_THREAD_PACKAGE::atomic_flag()
	{
		_My_flag = atomic_flag_init;
	}
	
	virtual ~AtomicFlag()
	{}

  private:
	//forbidding copy

//TODO : check if C++ 11
	AtomicFlag(const AtomicFlag&);
    AtomicFlag& operator=(const AtomicFlag&);
	
}; //class AtomicFlag

	}; //namespace Thread

}; //namespace Core

#endif // __CORE_THREAD_ATOMICFLAG_HH