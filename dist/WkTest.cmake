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


#
# Defining test rules based on a well known hierarchy
# You need at least the test subdirectory in you hierarchy for this to work.
# One source file can be used for a set of tests.
#

#debug
message ( STATUS "== Loading WkTest.cmake ..." )

if ( CMAKE_BACKWARDS_COMPATIBILITY LESS 2.6.3 )
	message ( FATAL_ERROR " CMAKE MINIMUM BACKWARD COMPATIBILITY REQUIRED : 2.6.3 !" )
endif( CMAKE_BACKWARDS_COMPATIBILITY LESS 2.6.3 )

set ( WKCMAKE_TEST_DIR "test" CACHE PATH "Test directory for WkCMake build products" )
mark_as_advanced ( WKCMAKE_TEST_DIR )

macro(WkTestDir dir)
	set ( WKCMAKE_TEST_DIR ${dir} CACHE PATH "Test directory for WkCMake build products" FORCE )
	mark_as_advanced ( WKCMAKE_TEST_DIR )
endmacro(WkTestDir dir)

#WkTestBuild( test_name [test_source [...] ] )

MACRO(WkTestBuild test_name)

	option(${PROJECT_NAME}_ENABLE_TESTS "Wether or not you want the project to include the tests and enable automatic testing for ${PROJECT_NAME}" OFF)

	IF(${PROJECT_NAME}_ENABLE_TESTS)
		ENABLE_TESTING()
		
		IF ( ${ARGC} EQUAL 1 )
			FILE(GLOB testsource RELATIVE ${PROJECT_SOURCE_DIR} ${WKCMAKE_TEST_DIR}/${test_name}.c ${WKCMAKE_TEST_DIR}/${test_name}.cc ${WKCMAKE_TEST_DIR}/${test_name}.cpp )
			MESSAGE ( STATUS "== Detected ${test_name} Source : ${testsource}" )
		ELSE ( ${ARGC} EQUAL 1 )
			set( testsource "" )
			#To make sure the sources file exists
			foreach( testsrc ${ARGN} )
				set ( testsrc ${PROJECT_SOURCE_DIR}/${WKCMAKE_TEST_DIR}/${testsrc} )
				IF (NOT EXISTS ${testsrc})
					MESSAGE ( FATAL_ERROR "${testsrc} Not Found !" )
				ELSE ( NOT EXISTS ${testsrc})
					SET (testsource ${testsource} ${testsrc})
				ENDIF (NOT EXISTS ${testsrc})
			endforeach( testsrc ${ARGN} )
			MESSAGE ( STATUS "== Detected ${test_name} Source : ${testsource}" )
		ENDIF ( ${ARGC} EQUAL 1 )

		IF (testsource)

			#Format test source code if formatting project
			IF ( ${PROJECT_NAME}_CODE_FORMAT )
				WkWhitespaceSplit( testsource testsource_spc )
				#message ( "Sources :  ${testsource_spc}" )
				set ( cmdline " ${ASTYLE_EXECUTABLE} --style=${${PROJECT_NAME}_CODE_FORMAT_STYLE} ${testsource_spc}" )
				#message ( "CMD : ${cmdline} " )
				ADD_CUSTOM_TARGET(${PROJECT_NAME}_${test_name}_format ALL sh -c ${cmdline} WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}" VERBATIM )
			ENDIF ( ${PROJECT_NAME}_CODE_FORMAT )

			#Create output directories
			IF ( NOT EXISTS ${PROJECT_BINARY_DIR}/${WKCMAKE_TEST_DIR} )
				FILE ( MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/${WKCMAKE_TEST_DIR} )
			ENDIF ( NOT EXISTS ${PROJECT_BINARY_DIR}/${WKCMAKE_TEST_DIR} )
		
			#Set where test executables should be found
			SET(${PROJECT_NAME}_TESTS_OUTPUT_PATH ${PROJECT_BINARY_DIR}/${WKCMAKE_TEST_DIR} CACHE PATH "Ouput directory for ${Project} tests.")
			mark_as_advanced(FORCE ${PROJECT_NAME}_TESTS_OUTPUT_PATH)
			SET(EXECUTABLE_OUTPUT_PATH "${${PROJECT_NAME}_TESTS_OUTPUT_PATH}" CACHE INTERNAL "Internal CMake executables output directory. Do not edit." FORCE)
		
			#build
			ADD_EXECUTABLE(${test_name} ${testsource} )
			TARGET_LINK_LIBRARIES(${test_name} ${PROJECT_NAME})
			ADD_DEPENDENCIES(${test_name} ${PROJECT_NAME})
			
			#We need to move project libraries and dependencies to the test target location after build.
			#We need to do that everytime to make sure we have the latest version
			
			GET_TARGET_PROPERTY(${test_name}_LOCATION ${test_name} LOCATION)
			get_filename_component(${test_name}_PATH ${${test_name}_LOCATION} PATH)

			#Moving project if library or module
			
			if ( ${${PROJECT_NAME}_TYPE} STREQUAL "SHARED_LIBRARY" OR ${${PROJECT_NAME}_TYPE} STREQUAL "MODULE_LIBRARY")
				GET_TARGET_PROPERTY(${PROJECT_NAME}_LOCATION ${PROJECT_NAME} LOCATION)
				ADD_CUSTOM_COMMAND( TARGET ${test_name} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${${PROJECT_NAME}_LOCATION} ${${test_name}_PATH}
														COMMENT "Copying ${${PROJECT_NAME}_LOCATION} to ${${test_name}_PATH}" )
			endif ( ${${PROJECT_NAME}_TYPE} STREQUAL "SHARED_LIBRARY" OR ${${PROJECT_NAME}_TYPE} STREQUAL "MODULE_LIBRARY")
			
			message ( STATUS "== Detected external dependencies for ${test_name} : ${${PROJECT_NAME}_DEPENDS}" )
			#if win32, moving all dependencies' run libraries
			if ( WIN32 )
				#needed for each run library dependency as well
				foreach ( looparg ${${PROJECT_NAME}_DEPENDS} )
					# we might have multiplke libs in one dependency
					foreach ( libarg ${${looparg}_RUN_LIBRARIES} ) 
						if ( NOT libarg )
							message ( SEND_ERROR "Error with dependency, needed to run test : ${libarg}" )
						endif ( NOT libarg )
						ADD_CUSTOM_COMMAND( TARGET ${test_name} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${libarg} ${${test_name}_PATH}
															COMMENT "Copying ${libarg} to ${${test_name}_PATH}" )
					endforeach ( libarg ${looparg} )
				endforeach ( looparg ${${PROJECT_NAME}_DEPENDS} )
			endif ( WIN32 )
			
		ENDIF (testsource)
	ENDIF(${PROJECT_NAME}_ENABLE_TESTS)

ENDMACRO(WkTestBuild test_name)

#
# WkTestData( test_name [ datafile1 [ datafile2 [ ... ] ] ] )
# Copy the data associated to test from the test directory in the source_path,
# to the binary_path, at the root
#
MACRO (WkTestData test_name )

	foreach ( test_data ${ARGN} )
		ADD_CUSTOM_COMMAND( TARGET ${test_name} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${PROJECT_SOURCE_DIR}/${WKCMAKE_TEST_DIR}/${test_data} ${PROJECT_BINARY_DIR}/${WKCMAKE_TEST_DIR}/${test_data} COMMENT "Copying ${PROJECT_SOURCE_DIR}/${test_data} to ${PROJECT_BINARY_DIR}/${test_data}" )
#
# Not this since in the IDE the tests is run from the project root...
#
#		ADD_CUSTOM_COMMAND( TARGET ${test_name} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${PROJECT_SOURCE_DIR}/${WKCMAKE_TEST_DIR}/${test_data} ${PROJECT_BINARY_DIR}/${WKCMAKE_TEST_DIR}/${test_data} COMMENT "Copying ${PROJECT_SOURCE_DIR}/${WKCMAKE_TEST_DIR}/${test_data} to ${PROJECT_BINARY_DIR}/${WKCMAKE_TEST_DIR}/${test_data}" )
	endforeach ( test_data ${ARGN} )
	
ENDMACRO (WkTestData data_path)

#
# Calls the same test executable multiple times, eachtime with 1 argument
#
# WkTestRun( test_name [ arguments [...] ] )
#

MACRO(WkTestRun test_name )

	IF(${PROJECT_NAME}_ENABLE_TESTS)
		ENABLE_TESTING()
	
		#if test arguments
		IF ( ${ARGC} GREATER 2 )
			FOREACH ( looparg ${ARGN} )
				ADD_TEST(${test_name}_${looparg} ${PROJECT_BINARY_DIR}/${WKCMAKE_TEST_DIR}/${test_name} ${looparg})
			ENDFOREACH ( looparg )
		ELSE ( ${ARGC} GREATER 2  )
			ADD_TEST(${test_name} ${PROJECT_BINARY_DIR}/${WKCMAKE_TEST_DIR}/${test_name})
		ENDIF ( ${ARGC} GREATER 2  )
	
	ENDIF(${PROJECT_NAME}_ENABLE_TESTS)

ENDMACRO(WkTestRun)

