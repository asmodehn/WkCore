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

#ifndef __CORE_THREAD_RECURSIVEMUTEX_HH
#define __CORE_THREAD_RECURSIVEMUTEX_HH

#include "tinythreadpp/source/tinythread.h"

namespace Core
{
	namespace Thread
	{

/// Recursive mutex class.
/// This is a mutual exclusion object for synchronizing access to shared
/// memory areas for several threads. The mutex is recursive (i.e. a thread
/// may lock the mutex several times, as long as it unlocks the mutex the same
/// number of times).
/// @see mutex
class RecursiveMutex : public tthread::recursive_mutex
{
  public:
    RecursiveMutex() :  tthread::recursive_mutex()
    {}

	virtual ~RecursiveMutex()
	{}

  private:
	//forbidding copy
	RecursiveMutex(const RecursiveMutex&);
    RecursiveMutex& operator=(const RecursiveMutex&);

	
}; //class RecursiveMutex

	}; //namespace Thread

}; //namespace Core

#endif // __CORE_THREAD_RECURSIVEMUTEX_HH