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

#include "tinythreadpp/source/tinythread.h"

#include "Core/DateTime/ChronoDuration.hh"

namespace Core
{
	namespace Thread
	{

		/// Thread class.
		/// This is only a wrapper over TinyThread class.
		/// For Advanced Thread Pattern using Core callbacks, see ThreadedCommand.
		class Thread : public tthread::thread
		{
		  public:
			Thread() :  tthread::thread()
    {}

			//This constructor doesnt exactly match C++11 constructor.
			Thread(void (*aFunction)(void *), void * aArg) :  tthread::thread(aFunction,aArg)
    {}

			virtual ~Thread()
	{}

			class Id : public tthread::thread::id
			{
			public:
				    Id() : id() {};
					Id(unsigned long int aId) : id(aId) {};
					Id(const Id& aId) : id(aId) {};
					//downward conversion constructor
					Id(const id& aId) : id(aId) {};
					virtual ~Id(){};
			};

			/// Determine the number of threads which can possibly execute concurrently.
			/// This function is useful for determining the optimal number of threads to
			/// use for a task.
			/// @return The number of hardware thread contexts in the system.
			/// @note If this value is not defined, the function returns zero (0).
			static unsigned hardwareConcurrency() 
	{
		return tthread::thread::hardware_concurrency();
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
				return tthread::this_thread::get_id();
			}

			/// Yield execution to another thread.
			/// Offers the operating system the opportunity to schedule another thread
			/// that is ready to run on the current processor.
			inline void yield()
			{
				tthread::this_thread::yield();
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
				tthread::this_thread::sleep_for(aTime);
			}
		}; //namespace ThisThread
	
	}; //namespace Thread

}; //namespace Core

#endif // __CORE_THREAD_THREAD_HH