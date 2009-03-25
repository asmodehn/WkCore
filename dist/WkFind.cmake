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


macro ( WkGenFind )

	#
	# First copy LibFindMacro there, to the module can actually work...
	#
	ADD_CUSTOM_COMMAND( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${PROJECT_SOURCE_DIR}/CMake/ext/LibFindMacros.cmake ${PROJECT_BINARY_DIR}/CMake/
													COMMENT "Copying ${PROJECT_SOURCE_DIR}/CMake/ext/LibFindMacros.cmake to ${PROJECT_BINARY_DIR}/CMake/" )
	
	#
	#start of the file + looking for dependencies
	#
	file( WRITE ${PROJECT_SOURCE_DIR}/CMake/ext/Find${PROJECT_NAME}.cmake "
# - Try to find ${PROJECT_NAME}
# Once done, this will define
#
#  ${PROJECT_NAME}_FOUND - system has ${PROJECT_NAME}
#  ${PROJECT_NAME}_INCLUDE_DIRS - the ${PROJECT_NAME} include directories
#  ${PROJECT_NAME}_LIBRARIES - link these to use ${PROJECT_NAME}

include(LibFindMacros)

	")
	
	#
	# finding dependencies
	#
	if ( ${PROJECT_NAME}_bin_depends )
	file ( APPEND ${PROJECT_SOURCE_DIR}/CMake/ext/Find${PROJECT_NAME}.cmake "
	
# Dependencies
libfind_package(${PROJECT_NAME} ${${PROJECT_NAME}_bin_depends})

	")
	endif ( ${PROJECT_NAME}_bin_depends )
	
	#
	#using pkg_config
	#
	file ( APPEND ${PROJECT_SOURCE_DIR}/CMake/ext/Find${PROJECT_NAME}.cmake "

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(${PROJECT_NAME}_PKGCONF ${PROJECT_NAME})

# Include dir
find_path(${PROJECT_NAME}_INCLUDE_DIR
  NAMES ${PROJECT_NAME}
  PATHS \${${PROJECT_NAME}_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
find_library(${PROJECT_NAME}_LIBRARY
  NAMES ${PROJECT_NAME}
  PATHS \${${PROJECT_NAME}_PKGCONF_LIBRARY_DIRS}
)
	")
	
	#
	# setting up variables
	#
	file ( APPEND ${PROJECT_SOURCE_DIR}/CMake/ext/Find${PROJECT_NAME}.cmake "
# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(${PROJECT_NAME}_PROCESS_INCLUDES ${PROJECT_NAME}_INCLUDE_DIR )
set(${PROJECT_NAME}_PROCESS_LIBS ${PROJECT_NAME}_LIBRARY )
	")
	
	#
	# Adding dependencies
	#
	foreach ( looparg ${${PROJECT_NAME}_bin_depends} )
		file ( APPEND ${PROJECT_SOURCE_DIR}/CMake/ext/Find${PROJECT_NAME}.cmake "
	set(${PROJECT_NAME}_PROCESS_INCLUDES ${PROJECT_NAME}_PROCESS_INCLUDES ${looparg}_INCLUDE_DIRS)
	set(${PROJECT_NAME}_PROCESS_LIBS ${PROJECT_NAME}_PROCESS_LIBS ${looparg}_LIBRARIES)
		")
	endforeach ( looparg ${${PROJECT_NAME}_bin_depends} )
	
	#
	# Finalising
	#
	file ( APPEND ${PROJECT_SOURCE_DIR}/CMake/ext/Find${PROJECT_NAME}.cmake "

libfind_process(${PROJECT_NAME})
	
	")

	ADD_CUSTOM_COMMAND( TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${PROJECT_SOURCE_DIR}/CMake/ext/Find${PROJECT_NAME}.cmake ${PROJECT_BINARY_DIR}/CMake/
													COMMENT "Copying ${PROJECT_SOURCE_DIR}/CMake/ext/LibFindMacros.cmake to ${PROJECT_BINARY_DIR}/CMake/" )
	
	
endmacro ( WkGenFind )