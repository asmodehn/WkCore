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

#ifndef __CORE_THREAD_THREAD_HH
#define __CORE_THREAD_THREAD_HH


#include "Core/DateTime/ChronoDuration.hh"

//TODO : add an actual check for this
#define NO_TLS

//TODO : Check if we need tinythread or not ( if no <thread> )
#ifdef TINYTHREAD_REQUIRED
# include "tinythreadpp/source/tinythread.h"
# define CORE_THREAD_PACKAGE tthread
#else
# include <thread>
# define CORE_THREAD_PACKAGE std
#endif

//to allow callback usage in threads call
#include "Callback.hh"
#include "Adapter.hh"

namespace Core
{
	namespace Thread
	{

		/// Thread class.
		/// This is only a wrapper over TinyThread class or std::thread (if available).
		/// It integrates with callback and adaptors from Core or std <functional> ( if available )
		class Thread : public CORE_THREAD_PACKAGE::thread
		{
		  public:
			Thread() :  CORE_THREAD_PACKAGE::thread()
			{}

			//template<class Fn>
			//explicit Thread(Fn&& f)
			//	: CORE_THREAD_PACKAGE::thread(f)
			//{}

			template<class Fn>
			explicit Thread(Fn f)
				: CORE_THREAD_PACKAGE::thread(std::move(f))
			{}

///TODO Check for variadics template
#ifdef VARIADICS
			template<class Fn, class... Args>
			explicit Thread(Fn&& f, Args&&... a)
				: CORE_THREAD_PACKAGE::thread(f,a)
			{}

			//in case of buggy move arguments (vs2012)
			template<class Fn, class... Args>
			explicit Thread(Fn f, Args... a)
				: CORE_THREAD_PACKAGE::thread(std::move(f),std::move(a))
			{}
#else
			//official per standard definition
			//template<class Fn, class Arg1>
			//explicit Thread(Fn&& f, Arg1&& a)
			//	: CORE_THREAD_PACKAGE::thread(f,a)
			//{}

			//for compilers that dont handle move arguments properly (vs2012)
			template<class Fn, class Arg1>
			explicit Thread(Fn f, Arg1 a)
				: CORE_THREAD_PACKAGE::thread(std::move(f),std::move(a))
			{}

			//template<class Fn, class Arg1, class Arg2>
			//explicit Thread(Fn&& f, Arg1&& a1, Arg2&& a2)
			//	: CORE_THREAD_PACKAGE::thread(f,a1,a2)
			//{}

			template<class Fn, class Arg1, class Arg2>
			explicit Thread(Fn f, Arg1 a1, Arg2 a2)
				: CORE_THREAD_PACKAGE::thread(std::move(f),std::move(a1),std::move(a2))
			{}

			template<class Fn, class Arg1, class Arg2, class Arg3>
			explicit Thread(Fn&& f, Arg1&& a1, Arg2&& a2, Arg3&& a3)
				: CORE_THREAD_PACKAGE::thread(f,a1,a2,a3)
			{}

			template<class Fn, class Arg1, class Arg2, class Arg3, class Arg4>
			explicit Thread(Fn&& f, Arg1&& a1, Arg2&& a2, Arg3&& a3, Arg4&& a4)
				: CORE_THREAD_PACKAGE::thread(f,a1,a2,a3,a4)
			{}

			template<class Fn, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
			explicit Thread(Fn&& f, Arg1&& a1, Arg2&& a2, Arg3&& a3, Arg4&& a4, Arg5&& a5)
				: CORE_THREAD_PACKAGE::thread(f,a1,a2,a3,a4,a5)
			{}

			template<class Fn, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
			explicit Thread(Fn&& f, Arg1&& a1, Arg2&& a2, Arg3&& a3, Arg4&& a4, Arg5&& a5, Arg6&& a6)
				: CORE_THREAD_PACKAGE::thread(f,a1,a2,a3,a4,a5,a6)
			{}
#endif

			//TODO : check if move constructors are available
			Thread(Thread&& other) : CORE_THREAD_PACKAGE::thread(std::move(other))
			{ }

			Thread& operator=(Thread&& other)
			{ 
				swap(std::move(other)); 
				return *this;
			}

			virtual ~Thread()
			{}

			typedef CORE_THREAD_PACKAGE::thread::id Id;
			
			/// Determine the number of threads which can possibly execute concurrently.
			/// This function is useful for determining the optimal number of threads to
			/// use for a task.
			/// @return The number of hardware thread contexts in the system.
			/// @note If this value is not defined, the function returns zero (0).
			static unsigned hardwareConcurrency() 
			{
				return CORE_THREAD_PACKAGE::thread::hardware_concurrency();
			}

		private:
			//forbidding copy
			Thread(const Thread&);
			Thread& operator=(const Thread&);

		}; //class Thread
		

		namespace ThisThread
		{
			/// Return the thread ID of the calling thread.
			inline Thread::Id get_id()
			{
				return CORE_THREAD_PACKAGE::this_thread::get_id();
			}

			/// Yield execution to another thread.
			/// Offers the operating system the opportunity to schedule another thread
			/// that is ready to run on the current processor.
			inline void yield()
			{
				CORE_THREAD_PACKAGE::this_thread::yield();
			}

			/// Blocks the calling thread for a period of time.
			/// @param[in] aTime Minimum time to put the thread to sleep.
			/// Example usage:
			/// @code
			/// // Sleep for 100 milliseconds
			/// this_thread::sleep_for(chrono::milliseconds(100));
			/// @endcode
			/// @note Supported duration types are: nanoseconds, microseconds,
			/// milliseconds, seconds, minutes and hours.
			template <class _Rep, class _Period> void sleep_for(const Core::DateTime::chrono::duration<_Rep, _Period>& aTime)
			{
				CORE_THREAD_PACKAGE::this_thread::sleep_for(aTime);
			}
		}; //namespace ThisThread
	
	}; //namespace Thread

}; //namespace Core

#endif // __CORE_THREAD_THREAD_HH