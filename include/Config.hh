/* *
 *  Copyright (c) 2009, Asmodehn's Corp.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *      - Redistributions of source code must retain the above copyright notice,
 * 	    this list of conditions and the following disclaimer.
 *      - Redistributions in binary form must reproduce the above copyright
 * 		notice, this list of conditions and the following disclaimer in the
 * 	    documentation and/or other materials provided with the distribution.
 *      - either the name of the Asmodehn's Corp. nor the names of its
 * 	    contributors may be used to endorse or promote products derived
 * 	    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 *  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 *  THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef CONFIG_HH
#define CONFIG_HH

//We are built using WkCMake, we can use WkPlatform Detection

#ifdef WK_BUILD

# include "WkPlatform_WkCore.h"

# if defined ( WK_WkCore_PLATFORM_IS_WIN32 )
#  define CORE_PLATFORM_WIN32
# elif defined ( WK_WkCore_PLATFORM_POSIX )
#  define CORE_PLATFORM_POSIX
# endif

# define CORE_API WK_WkCore_API
# define CORE_LOCAL WK_WkCore_LOCAL

#else // WK_BUILD
//using another build system

#endif // WK_BUILD

# define CORE_API
# define CORE_LOCAL

#endif // CONFIG _HH
