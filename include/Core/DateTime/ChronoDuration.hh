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

#ifndef __CORE_DATETIME_CHRONODURATION_HH
#define __CORE_DATETIME_CHRONODURATION_HH

#ifdef TINYTHREAD_REQUIRED
# include "tinythreadpp/source/tinythread.h"
# define CORE_CHRONO_PACKAGE tthread::chrono
# define CORE_RATIO_PACKAGE tthread::ratio
#else
# include <chrono>
# define CORE_CHRONO_PACKAGE std::chrono
//required for chrono and ratio package to match
# include <ratio>
# define CORE_RATIO_PACKAGE std::ratio
#endif

#include "Core/Numerics/Ratio.hh"

namespace Core
{
	namespace DateTime
	{
		namespace chrono
		{
			/// Duration template class. This class provides enough functionality to
			/// implement @c this_thread::sleep_for().
			template <class _Rep, class _Period = Core::Numerics::ratio<1, 1> > class duration : public CORE_CHRONO_PACKAGE::duration<_Rep,_Period>
			{
			public:
				/// Construct a duration object with the given duration.
				template <class _Rep2>
				explicit duration(const _Rep2& r) : CORE_CHRONO_PACKAGE::duration<_Rep,_Period>(r) {};



				virtual ~duration() {};

			}; //class duration

			// Standard duration types.
			typedef duration<Core::Numerics::__intmax_t, Core::Numerics::ratio<1, 1000000000> > nanoseconds; ///< Duration with the unit nanoseconds.
			typedef duration<Core::Numerics::__intmax_t, Core::Numerics::ratio<1, 1000000> > microseconds;   ///< Duration with the unit microseconds.
			typedef duration<Core::Numerics::__intmax_t, Core::Numerics::ratio<1, 1000> > milliseconds;      ///< Duration with the unit milliseconds.
			typedef duration<Core::Numerics::__intmax_t, Core::Numerics::ratio<1, 1> > seconds;			     ///< Duration with the unit seconds.
			typedef duration<Core::Numerics::__intmax_t, Core::Numerics::ratio<60> > minutes;                ///< Duration with the unit minutes.
			typedef duration<Core::Numerics::__intmax_t, Core::Numerics::ratio<3600> > hours;                ///< Duration with the unit hours.

		}; //namespace chrono

	}; //namespace DateTime

}; //namespace Core

//cleanups
#undef CORE_CHRONO_PACKAGE
#undef CORE_RATIO_PACKAGE

#endif // __CORE_DATETIME_CHRONODURATION_HH
