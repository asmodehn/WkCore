# 
# Copyright (c) 2009, Asmodehn's Corp.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions are met:
# 
#     * Redistributions of source code must retain the above copyright notice, 
#	    this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#		notice, this list of conditions and the following disclaimer in the 
#	    documentation and/or other materials provided with the distribution.
#     * Neither the name of the Asmodehn's Corp. nor the names of its 
#	    contributors may be used to endorse or promote products derived
#	    from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
# BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGE.
#

if ( CMAKE_BACKWARDS_COMPATIBILITY LESS 2.6 )
	message ( FATAL_ERROR " CMAKE MINIMUM BACKWARD COMPATIBILITY REQUIRED : 2.6 !" )
endif( CMAKE_BACKWARDS_COMPATIBILITY LESS 2.6 )

#Putting a useful default for backwards compatibility
set ( WKCMAKE_DIR "CMake" )

macro(WkCMakeDir dir)
	set ( WKCMAKE_DIR ${dir} )
#setting defaults for directories
	set ( WKCMAKE_INCLUDE_DIR "include" )
	set ( WKCMAKE_SRC_DIR "src" )
	set ( WKCMAKE_BIN_DIR "bin" )
	set ( WKCMAKE_LIB_DIR "lib" )
	set ( WKCMAKE_STATIC_DIR "lib/static")
#including other useful files
	include ( ${dir}/WkBuild.cmake )
	include ( ${dir}/WkTest.cmake )
	include ( ${dir}/WkDoc.cmake )
endmacro(WkCMakeDir dir)

macro(WkIncludeDir dir)
	set ( WKCMAKE_INCLUDE_DIR ${dir} )

endmacro(WkIncludeDir dir)

macro(WkSrcDir dir)
	set ( WKCMAKE_SRC_DIR ${dir} )

endmacro(WkSrcDir dir)

macro(WkBinDir dir)
	set ( WKCMAKE_BIN_DIR ${dir} )

endmacro(WkBinDir dir)

macro(WkLibDir dir)
	set ( WKCMAKE_LIB_DIR ${dir} )

endmacro(WkLibDir dir)

macro(WkStaticDir dir)
	set ( WKCMAKE_STATIC_DIR ${dir} )

endmacro(WkStaticDir dir)

