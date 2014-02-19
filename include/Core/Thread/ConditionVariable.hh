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

#ifndef __CORE_THREAD_CONDITIONVARIABLE_HH
#define __CORE_THREAD_CONDITIONVARIABLE_HH

#ifdef TINYTHREAD_REQUIRED
# include "tinythreadpp/source/tinythread.h"
# define CORE_THREAD_PACKAGE tthread
#else
# include <condition_variable>
# define CORE_THREAD_PACKAGE std
#endif

namespace Core
{
	namespace Thread
	{

/// Condition variable class.
/// This is a signalling object for synchronizing the execution flow for
/// several threads. Example usage:
/// @code
/// // Shared data and associated mutex and condition variable objects
/// int count;
/// mutex m;
/// condition_variable cond;
///
/// // Wait for the counter to reach a certain number
/// void wait_counter(int targetCount)
/// {
///   lock_guard<mutex> guard(m);
///   while(count < targetCount)
///     cond.wait(m);
/// }
///
/// // Increment the counter, and notify waiting threads
/// void increment()
/// {
///   lock_guard<mutex> guard(m);
///   ++ count;
///   cond.notify_all();
/// }
/// @endcode
class ConditionVariable : public CORE_THREAD_PACKAGE::condition_variable
{
  public:
    ConditionVariable() :  CORE_THREAD_PACKAGE::condition_variable()
    {}

    virtual ~ConditionVariable()
    {}

	void wait( Mutex& lock)
	{
		CORE_THREAD_PACKAGE::condition_variable::wait( std::unique_lock<CORE_THREAD_PACKAGE::mutex>(lock) );
	}

	template< class Predicate >
	void wait( Mutex& lock, Predicate pred )
	{
		CORE_THREAD_PACKAGE::condition_variable::wait( std::unique_lock<CORE_THREAD_PACKAGE::mutex>(lock),pred);
	}

  private:
	//forbidding copy
	ConditionVariable(const ConditionVariable&);
    ConditionVariable& operator=(const ConditionVariable&);

	
}; //class ConditionVariable

	}; //namespace Thread

}; //namespace Core

//cleanup
#undef CORE_THREAD_PACKAGE

#endif // __CORE_THREAD_CONDITIONVARIABLE_HH