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

#debug
message ( STATUS "== Loading WkCMake.cmake ... ")

if ( CMAKE_BACKWARDS_COMPATIBILITY LESS 2.6 )
	message ( FATAL_ERROR " CMAKE MINIMUM BACKWARD COMPATIBILITY REQUIRED : 2.6 !" )
endif( CMAKE_BACKWARDS_COMPATIBILITY LESS 2.6 )

#Putting a useful default for backwards compatibility
set ( WKCMAKE_DIR "CMake" CACHE PATH "WkCMake Scripts path" )
mark_as_advanced ( WKCMAKE_DIR )

macro(WkCMakeDir dir)
	set ( WKCMAKE_DIR ${dir} CACHE PATH "WkCMake Scripts path" FORCE )
#setting defaults for directories
	set ( WKCMAKE_INCLUDE_DIR "include" CACHE PATH "Headers directory for autodetection by WkCMake" FORCE )
	set ( WKCMAKE_SRC_DIR "src" CACHE PATH "Sources directory for autodetection by WkCMake" FORCE )
	set ( WKCMAKE_BIN_DIR "bin" CACHE PATH "Binary directory for WkCMake build products" FORCE )
	set ( WKCMAKE_LIB_DIR "lib" CACHE PATH "Library directory for WkCMake build products" FORCE )
	set ( WKCMAKE_DATA_DIR "data" CACHE PATH "Data directory for WkCMake build products" FORCE )
	mark_as_advanced ( WKCMAKE_DIR WKCMAKE_INCLUDE_DIR WKCMAKE_SRC_DIR WKCMAKE_BIN_DIR WKCMAKE_LIB_DIR WKCMAKE_DATA_DIR)
	
	# regroup original CMAKE_MODULE_PATH and WkCmake Module path in order for the find_package to look inside both locations
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/${WKCMAKE_DIR}/Modules/")
	
#including other useful files
	include ( "${WKCMAKE_DIR}/WkBuild.cmake" RESULT_VARIABLE WKCMAKEBUILD_FOUND )
	IF ( NOT WKCMAKEBUILD_FOUND )
		message ( FATAL_ERROR "${WKCMAKE_DIR}/WkBuild.cmake Not Found !!!" )
	ENDIF ( NOT WKCMAKEBUILD_FOUND )
	include ( "${WKCMAKE_DIR}/WkDepends.cmake" RESULT_VARIABLE WKCMAKEDEPENDS_FOUND )
	IF ( NOT WKCMAKEDEPENDS_FOUND )
		message ( FATAL_ERROR "${WKCMAKE_DIR}/WkDepends.cmake Not Found !!!" )
	ENDIF ( NOT WKCMAKEDEPENDS_FOUND )
	include ( "${WKCMAKE_DIR}/WkTest.cmake" RESULT_VARIABLE WKCMAKETEST_FOUND )
	IF ( NOT WKCMAKETEST_FOUND )
		message ( FATAL_ERROR "${WKCMAKE_DIR}/WkTest.cmake Not Found !!!" )
	ENDIF ( NOT WKCMAKETEST_FOUND )
	include ( "${WKCMAKE_DIR}/WkDoc.cmake" RESULT_VARIABLE WKCMAKEDOC_FOUND )
	IF ( NOT WKCMAKEDOC_FOUND )
		message ( FATAL_ERROR "${WKCMAKE_DIR}/WkDoc.cmake Not Found !!!" )
	ENDIF ( NOT WKCMAKEDOC_FOUND )
	include ( "${WKCMAKE_DIR}/WkSubversion.cmake" RESULT_VARIABLE WKCMAKESVN_FOUND )
	IF ( NOT WKCMAKESVN_FOUND )
		message ( FATAL_ERROR "${WKCMAKE_DIR}/WkSubversion.cmake Not Found !!!" )
	ENDIF ( NOT WKCMAKESVN_FOUND )
	include ( "${WKCMAKE_DIR}/WkInstall.cmake" RESULT_VARIABLE WKCMAKEINSTALL_FOUND )
	IF ( NOT WKCMAKEINSTALL_FOUND )
		message ( FATAL_ERROR "${WKCMAKE_DIR}/WkInstall.cmake Not Found !!!" )
	ENDIF ( NOT WKCMAKEINSTALL_FOUND )
	include ( "${WKCMAKE_DIR}/WkPack.cmake" RESULT_VARIABLE WKCMAKEPACK_FOUND )
	IF ( NOT WKCMAKEPACK_FOUND )
		message ( FATAL_ERROR "${WKCMAKE_DIR}/WkPack.cmake Not Found !!!" )
	ENDIF ( NOT WKCMAKEPACK_FOUND )
endmacro(WkCMakeDir dir)

macro(WkIncludeDir dir)
	set ( WKCMAKE_INCLUDE_DIR ${dir} CACHE PATH "Headers directory for autodetection by WkCMake" FORCE )
	mark_as_advanced ( WKCMAKE_INCLUDE_DIR )
endmacro(WkIncludeDir dir)

macro(WkModulesDir dir)
	set ( WKCMAKE_MODULES_DIR ${dir} CACHE PATH "Modules directory for package autodetection by WkCMake" FORCE )
	#TODO : detect if absolute or relative...
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/${WKCMAKE_MODULES_DIR}")
	mark_as_advanced ( WKCMAKE_MODULES_DIR )
endmacro(WkModulesDir dir)

macro(WkSrcDir dir)
	set ( WKCMAKE_SRC_DIR ${dir} CACHE PATH "Sources directory for autodetection by WkCMake" FORCE )
	mark_as_advanced ( WKCMAKE_SRC_DIR )
endmacro(WkSrcDir dir)

macro(WkBinDir dir)
	set ( WKCMAKE_BIN_DIR ${dir} CACHE PATH "Binary directory for WkCMake build products" FORCE )
	mark_as_advanced ( WKCMAKE_BIN_DIR )
endmacro(WkBinDir dir)

macro(WkLibDir dir)
	set ( WKCMAKE_LIB_DIR ${dir} CACHE PATH "Library directory for WkCMake build products" FORCE )
	mark_as_advanced ( WKCMAKE_LIB_DIR )
endmacro(WkLibDir dir)

macro(WkDataDir dir)
	set ( WKCMAKE_DATA_DIR ${dir} CACHE PATH "Data directory for WkCMake build products" FORCE )
	mark_as_advanced ( WKCMAKE_DATA_DIR )
endmacro(WkDataDir dir)

