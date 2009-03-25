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


#To setup the compiler
include ( CMake/WkCompilerSetup.cmake )
include ( CMake/WkFind.cmake )

macro(WKProject project_name_arg)
CMAKE_POLICY(PUSH)
CMAKE_POLICY(VERSION 2.6)
	project(${project_name_arg} ${ARGN})
	WkCompilerSetup( )
CMAKE_POLICY(POP)
endmacro(WKProject PROJECT_NAME)

MACRO(MERGE ALIST BLIST OUTPUT)
   SET(BTEMP ${BLIST})
   FOREACH(A ${ALIST})
       SET(SORTED)
       SET(UNINSERTED 1)
       FOREACH(B ${BTEMP})
           IF(${UNINSERTED})
               IF(${A} STRLESS ${B})
                   SET(SORTED ${SORTED} ${A})
                   SET(UNINSERTED 0)
               ENDIF(${A} STRLESS ${B})
           ENDIF(${UNINSERTED})
           SET(SORTED ${SORTED} ${B})
       ENDFOREACH(B ${BLIST})
       IF(${UNINSERTED})
           SET(SORTED ${SORTED} ${A})
       ENDIF(${UNINSERTED})
       SET(BTEMP ${SORTED})
   ENDFOREACH(A ${ALIST})
   SET(${OUTPUT} ${BTEMP})
ENDMACRO(MERGE ALIST BLIST OUTPUT)

#
# Configure and Build process based on well-known hierarchy
# You need include and src in your hierarchy at least for this to work correctly
# You also need MergeLists.txt 
#

#WkBuild( target_name EXECUTABLE | LIBRARY [ STATIC|SHARED|MODULE ]  )

macro (WkBuild project_type )
CMAKE_POLICY(PUSH)
CMAKE_POLICY(VERSION 2.6)

	if ( ${ARGC} GREATER 1 )
		set(load_type ${ARGV1} )
	endif ( ${ARGC} GREATER 1 )

	message ( STATUS "Configuring ${PROJECT_NAME}" )	
		
	# testing type
	if (NOT ${project_type} STREQUAL "EXECUTABLE" AND NOT ${project_type} STREQUAL "LIBRARY" )
		message ( FATAL_ERROR " Project type ${project_type} is not valid. Project type can be either EXECUTABLE or LIBRARY")
	endif (NOT ${project_type} STREQUAL "EXECUTABLE" AND NOT ${project_type} STREQUAL "LIBRARY" )
	if ( ${project_type} STREQUAL "LIBRARY" 
					AND NOT ${load_type} STREQUAL "STATIC"
					AND NOT ${load_type} STREQUAL "SHARED"
					AND NOT ${load_type} STREQUAL "MODULE"
		)
		message ( FATAL_ERROR " Project Load type ${load_type} is not valid. Project Load type can be either STATIC, SHARED or MODULE")
	endif  ( ${project_type} STREQUAL "LIBRARY" 
					AND NOT ${load_type} STREQUAL "STATIC"
					AND NOT ${load_type} STREQUAL "SHARED"
					AND NOT ${load_type} STREQUAL "MODULE"
		)
	

	
		#Building dependencies recursively ( not looking into hidden directories (beginning with '.' or '..') )
		
		file(GLOB ${PROJECT_NAME}_sourcedir_depends RELATIVE ${PROJECT_SOURCE_DIR} ext/[^.]* )
		#to get rid of the ext/ prefix
		set(${PROJECT_NAME}_source_depends)
		foreach ( looparg ${${PROJECT_NAME}_sourcedir_depends} )
			get_filename_component( dependency_project ${looparg} NAME)
			list( APPEND ${PROJECT_NAME}_source_depends ${dependency_project} )
		endforeach ( looparg )
		message ( STATUS "Dependencies detected : ${${PROJECT_NAME}_source_depends}" )
		
		if ( ${PROJECT_NAME}_source_depends )
			set(${PROJECT_NAME}_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
			#making sure that by default we build static libraries for source dependencies
			set(BUILD_SHARED_LIBS OFF)
			foreach ( looparg ${${PROJECT_NAME}_source_depends} )
				message ( STATUS "==" )
				message ( STATUS "Cmake'ing dependency ${looparg} : Started." )
				#building in separate build directory in case the father project is built in source...
				add_subdirectory(ext/${looparg} ext/${looparg}_build )
				message ( STATUS "Cmake'ing dependency ${looparg} : Done." )
				message ( STATUS "==" )
			endforeach ( looparg )
			#MESSAGE ( STATUS "Back to configuring ${PROJECT_NAME} build" )
			set( BUILD_SHARED_LIBS ${${PROJECT_NAME}_BUILD_SHARED_LIBS} )
		endif ( ${PROJECT_NAME}_source_depends )	
	
	#Verbose Makefile if not release build. Making them internal not to confuse user by appearing with values used only for one project.
	if (${${PROJECT_NAME}_BUILD_TYPE} STREQUAL Release)
		set(CMAKE_VERBOSE_MAKEFILE OFF CACHE INTERNAL "Verbose build commands disabled for Release build." FORCE)
		set(CMAKE_USE_RELATIVE_PATHS OFF CACHE INTERNAL "Absolute paths used in makefiles and projects for Release build." FORCE)
	else (${${PROJECT_NAME}_BUILD_TYPE} STREQUAL Release)
		message( STATUS "Non Release build detected : enabling verbose makefile" )
		# To get the actual commands used
		set(CMAKE_VERBOSE_MAKEFILE ON CACHE INTERNAL "Verbose build commands enabled for Non Release build." FORCE)
				#VLD
		set(CHECK_MEM_LEAKS OFF CACHE BOOL "On to check memory with VLD (must be installed)")
		if(CHECK_MEM_LEAKS)
			add_definitions(-DVLD)
		endif(CHECK_MEM_LEAKS)
	endif (${${PROJECT_NAME}_BUILD_TYPE} STREQUAL Release)

	#Defining target
	
	#VS workaround to display headers
	FILE(GLOB_RECURSE HEADERS RELATIVE ${PROJECT_SOURCE_DIR} include/*.h include/*.hh include/*.hpp)
	FILE(GLOB_RECURSE SOURCES RELATIVE ${PROJECT_SOURCE_DIR} src/*.c src/*.cpp src/*.cc)

	#Including configured headers (
	#	-binary for the configured header, 
	#	-Cmake for Wk headers
	#	-source for the unmodified ones, 
	#	-and in source/src for internal ones)
	INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/CMake ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)
	foreach ( looparg ${${PROJECT_NAME}_source_depends} )
		include_directories( ${PROJECT_BINARY_DIR}/ext/${looparg}_build/include )
	endforeach ( looparg)

	#TODO : find a simpler way than this complex merge...
	MERGE("${HEADERS}" "${SOURCES}" SOURCES)
	#MESSAGE ( STATUS "Sources : ${SOURCES}" )
	
	if(${project_type} STREQUAL "LIBRARY")
		add_library(${PROJECT_NAME} ${load_type} ${SOURCES})
		if ( load_type )
		if(${load_type} STREQUAL "SHARED")
			set_target_properties(${PROJECT_NAME} PROPERTIES DEFINE_SYMBOL "WK_SHAREDLIB_BUILD")
		endif(${load_type} STREQUAL "SHARED")
		endif (load_type)		
	endif(${project_type} STREQUAL "LIBRARY")
	if(${project_type} STREQUAL "EXECUTABLE")
		add_executable(${PROJECT_NAME} ${SOURCES})
	endif(${project_type} STREQUAL "EXECUTABLE")

	#
	#Linking dependencies
	#
	
	IF ( ${PROJECT_NAME}_source_depends )
		FOREACH ( looparg ${${PROJECT_NAME}_source_depends} )
			IF(${project_type} STREQUAL "LIBRARY")
				TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${looparg})
				ADD_DEPENDENCIES(${PROJECT_NAME} ${looparg})
			ENDIF(${project_type} STREQUAL "LIBRARY")
			IF(${project_type} STREQUAL "EXECUTABLE")
				TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${looparg})
				ADD_DEPENDENCIES(${PROJECT_NAME} ${looparg})
			ENDIF(${project_type} STREQUAL "EXECUTABLE")
		ENDFOREACH ( looparg )
	ENDIF ( ${PROJECT_NAME}_source_depends )
	
	#
	# Defining where to put what has been built
	#
	
	SET(${PROJECT_NAME}_LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib CACHE PATH "Ouput directory for ${Project} libraries." )
	mark_as_advanced(FORCE ${PROJECT_NAME}_LIBRARY_OUTPUT_PATH)
	SET(LIBRARY_OUTPUT_PATH "${${PROJECT_NAME}_LIBRARY_OUTPUT_PATH}" CACHE INTERNAL "Internal CMake libraries output directory. Do not edit." FORCE)
	
	SET(${PROJECT_NAME}_EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin CACHE PATH "Ouput directory for ${Project} executables." )
	mark_as_advanced(FORCE ${PROJECT_NAME}_EXECUTABLE_OUTPUT_PATH)
	SET(EXECUTABLE_OUTPUT_PATH "${${PROJECT_NAME}_EXECUTABLE_OUTPUT_PATH}" CACHE INTERNAL "Internal CMake executables output directory. Do not edit." FORCE)

	#
	# Copying include directory if needed after build ( for  use by another project later )
	# for library (and modules ? )
	#
	
	if(${project_type} STREQUAL "LIBRARY") 
		ADD_CUSTOM_COMMAND( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy_directory ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
													COMMENT "Copying ${PROJECT_SOURCE_DIR}/include to ${PROJECT_BINARY_DIR}" )
	endif(${project_type} STREQUAL "LIBRARY") 
	
	#
	#Exporting main targets (and source dependencies )
	#
	
	export(TARGETS ${PROJECT_NAME} ${${PROJECT_NAME}_source_depends} FILE Export.cmake)

	#
	# we must generate the findProject.cmake
	#
	
	WkGenFind( )
	
CMAKE_POLICY(POP)
endmacro (WkBuild)


#
# Find a dependency built in an external WK hierarchy
# Different than for a package because this dependency hasnt been installed yet.
#
# WkBinDepends( RELEASE | DEBUG  [ bin_depend [...] ] )

macro (WkBinDepends build_type )
CMAKE_POLICY(PUSH)
CMAKE_POLICY(VERSION 2.6)

foreach ( looparg ${ARGN} )
	
	if ( NOT ${looparg}_EXPORT_CMAKE )
		set(${looparg}_EXPORT_CMAKE CACHE FILEPATH " Export.cmake filepath for ${looparg} dependency " )
	endif ( NOT ${looparg}_EXPORT_CMAKE )
	
	if ( EXISTS ${${looparg}_EXPORT_CMAKE})
		include(${${looparg}_EXPORT_CMAKE})
		
		#saving project_bin_depends for future use
		set(${PROJECT_NAME}_bin_depends ${${PROJECT_NAME}_bin_depends} ${looparg})
		if ( ${build_type} STREQUAL "RELEASE")
			get_target_property(${looparg}_LOCATION ${looparg} IMPORTED_LOCATION_RELEASE)
		else ( ${build_type} STREQUAL "RELEASE")			
			get_target_property(${looparg}_LOCATION ${looparg} IMPORTED_LOCATION_DEBUG)
		endif ( ${build_type} STREQUAL "RELEASE")
		set(${looparg}_BIN_LOCATION ${${looparg}_LOCATION} CACHE FILEPATH "Location of the binary dependency - .lib or .dll" )

		if ( NOT EXISTS ${${looparg}_BIN_LOCATION} )
			message (FATAL_ERROR "Binary dependency ${${looparg}_LOCATION} NOT FOUND. Please correct ${looparg}_LOCATION")
		endif ( NOT EXISTS ${${looparg}_BIN_LOCATION} )
		
		#include if present
		get_filename_component(${looparg}_DEPEND_PATH ${${looparg}_EXPORT_CMAKE} PATH)
		if (EXISTS ${${looparg}_DEPEND_PATH}/include)
			include_directories( ${${looparg}_DEPEND_PATH}/include )
		endif (EXISTS ${${looparg}_DEPEND_PATH}/include)
		target_link_libraries( ${PROJECT_NAME} ${looparg})
		#reminder : imported targets wont be built by curent project, so no need to specify dependencies building priorities.
		
		#propagating exported target upwards...
		export(TARGETS ${looparg} APPEND FILE Export.cmake)
	
	else ( EXISTS ${${looparg}_EXPORT_CMAKE} )
		message ( FATAL_ERROR "${looparg} build not detected ( looking for Export.cmake ). Please correct ${looparg}_EXPORT_CMAKE" )
	endif ( EXISTS ${${looparg}_EXPORT_CMAKE} )



endforeach ( looparg ${ARGN} )

CMAKE_POLICY(POP)
endmacro (WkBinDepends build_type )


