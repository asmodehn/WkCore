# 
# Copyright (c) 2010, Asmodehn's Corp.
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
message ( STATUS "== Loading WkPlatform.cmake ..." )

if ( CMAKE_BACKWARDS_COMPATIBILITY LESS 2.6 )
	message ( FATAL_ERROR " CMAKE MINIMUM BACKWARD COMPATIBILITY REQUIRED : 2.6 !" )
endif( CMAKE_BACKWARDS_COMPATIBILITY LESS 2.6 )

###################################################################
# Testing current platform                                        #
# This will set WK_PLATFORM to WIN32, APPLE, or UNIX in the code  #
###################################################################
if ( WIN32 )
# is TRUE on Windows, including CygWin 
 set( WK_PLATFORM "WIN32" )
 set( WK_PLATFORM_IS_WIN32 ON )
elseif ( APPLE )
# is TRUE on Apple systems. Note this does not imply the system is Mac OS X, only that __APPLE__ is #defined in C/C++ header files. Obtain more specific system information via CMAKE_SYSTEM_VERSION, i.e. IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin"), then it's Mac OS X. 
 set ( WK_PLATFORM "APPLE" )
 set ( WK_PLATFORM_IS_APPLE ON )
elseif ( UNIX )
# is TRUE on all UNIX-like OS's, including Apple OS X and CygWin 
 set( WK_PLATFORM "UNIX" )
 set( WK_PLATFORM_IS_UNIX ON )
else ()
 set( WK_PLATFORM "Unknown" )
endif()

###################################################################
#Testing System in details ( cmake > 2.6 )                        #
###################################################################
if ( CMAKE_SYSTEM )
 set ( WK_SYSTEM "${CMAKE_SYSTEM}" )
endif ( CMAKE_SYSTEM )
if ( CMAKE_SYSTEM_NAME )
 set ( WK_SYSTEM_NAME "${CMAKE_SYSTEM_NAME}" )
endif ( CMAKE_SYSTEM_NAME)
if ( CMAKE_SYSTEM_VERSION )
 set ( WK_SYSTEM_VERSION "${CMAKE_SYSTEM_VERSION}" )
endif ( CMAKE_SYSTEM_VERSION )
if ( CMAKE_SYSTEM_PROCESSOR )
 set ( WK_SYSTEM_PROCESSOR "${CMAKE_SYSTEM_PROCESSOR}" )
endif ( CMAKE_SYSTEM_PROCESSOR )


#Additional Testing for common system
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
# Mac OS X system
 set( WK_SYSTEM_IS_MACOSX ON )
elseif ( ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
# Windows system
 set ( WK_SYSTEM_IS_WINDOWS ON )
elseif ( ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
# Linux system
 set ( WK_SYSTEM_IS_LINUX ON )
endif()

############################################################################################
# Testing curent Compiler                                                                  #
# This will set                                                                            #
# WK_COMPILER to MINGW, MSYS, CYGWIN, BORLAND, WATCOM, MSVC90, MSVC80, MSVC71, MSVC70      #
# WK_COMPILER_IS_MSVC if the compiler is a MS compiler                                     #
# WK_COMPILER_IS_GNUCC if the compiler is a variant of GCC                                 #
# WK_COMPILER_IS_GNUCXX if the compiler is a variant of G++                                #
############################################################################################
if ( MINGW )
#    is TRUE when using the MinGW compiler in Windows 
 set ( WK_COMPILER "MINGW" )
elseif( MSYS )
#    is TRUE when using the MSYS developer environment in Windows 
 set ( WK_COMPILER "MSYS" )
elseif ( CYGWIN )
#    is TRUE on Windows when using the CygWin version of cmake 
 set ( WK_COMPILER "CYGWIN" )
elseif ( BORLAND )
#    is TRUE on Windows when using a Borland compiler 
 set ( WK_COMPILER "BORLAND" )
elseif( WATCOM )
#    is TRUE on Windows when using the Open Watcom compiler 
 set ( WK_COMPILER "WATCOM" )
elseif ( MSVC90 )
#    Microsoft Visual 9
 set ( WK_COMPILER "MSVC90" )
elseif ( MSVC80 )
#    Microsoft Visual 8
 set ( WK_COMPILER "MSVC80" )
elseif ( MSVC71 )
#    Microsoft Visual 7.1
 set ( WK_COMPILER "MSVC71" )
elseif ( MSVC70 )
#    Microsoft Visual 7.0
 set ( WK_COMPILER "MSVC70" )
elseif ( MSVC60 )
#    Microsoft Visual 6.0
 set ( WK_COMPILER "MSVC60" )
else ()
# Others
 set ( WK_COMPILER "Unknown" )
endif ()

if ( ${WK_COMPILER} MATCHES "MSVC" )
 set ( WK_COMPILER_IS_MSVC ON)
endif ( ${WK_COMPILER} MATCHES "MSVC" )

if ( CMAKE_COMPILER_IS_GNUCC )
#    is TRUE if the compiler is a variant of gcc 
 set ( WK_COMPILER_IS_GNUCC ON )
endif ( CMAKE_COMPILER_IS_GNUCC )

if ( CMAKE_COMPILER_IS_GNUCXX )
#    is TRUE if the compiler is a variant of g++ 
 set ( WK_COMPILER_IS_GNUCXX ON )
endif ( CMAKE_COMPILER_IS_GNUCXX )

########################################################
# Extensive tests on compilation environment           #
########################################################
INCLUDE (CheckIncludeFiles)
# usage: CHECK_INCLUDE_FILES (<header> <RESULT_VARIABLE> )

CHECK_INCLUDE_FILES (unistd.h WK_HAVE_UNISTD_H)

########################################################
# Macro to apply the variables to config header        #
########################################################
macro(PlatformCheck)
# configure the platform check component and copy it into the program sources.
# We can afford to do that because the platform check is simple.
# if it gets more complicated ( > 1 file), we need to make a custom library
# and make the link to the project optional.
	message ( STATUS "== Configuring WkPlatform : ${PROJECT_SOURCE_DIR}/${WKCMAKE_DIR}/WkPlatform.h.config -> ${PROJECT_BINARY_DIR}/CMakeFiles/WkPlatform.h" )
	configure_file(${PROJECT_SOURCE_DIR}/${WKCMAKE_DIR}/WkPlatform.h.config ${PROJECT_BINARY_DIR}/CMakeFiles/WkPlatform.h )
	configure_file(${PROJECT_SOURCE_DIR}/${WKCMAKE_DIR}/WkPlatform.c.config ${PROJECT_BINARY_DIR}/CMakeFiles/WkPlatform.c )
endmacro(PlatformCheck)

macro(AddPlatformCheckSrc VAR )
	SET( ${VAR} "${${VAR}}" "CMakeFiles/WkPlatform.c")
endmacro(AddPlatformCheckSrc VAR )
