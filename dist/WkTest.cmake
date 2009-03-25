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

if ( CMAKE_BACKWARDS_COMPATIBILITY LESS 2.6.3 )
	message ( FATAL_ERROR " CMAKE MINIMUM BACKWARD COMPATIBILITY REQUIRED : 2.6.3 !" )
endif( CMAKE_BACKWARDS_COMPATIBILITY LESS 2.6.3 )

#WkTestBuild( test_name )

MACRO(WkTestBuild )

	option(${PROJECT_NAME}_ENABLE_TESTS "Wether or not you want the project to include the tests and enable automatic testing for ${PROJECT_NAME}" OFF)

	IF(${PROJECT_NAME}_ENABLE_TESTS)
		ENABLE_TESTING()
		
		foreach( test_name ${ARGN})
			FILE(GLOB testsource RELATIVE ${PROJECT_SOURCE_DIR} test/${test_name}.c test/${test_name}.cc test/${test_name}.cpp )
			#MESSAGE ( STATUS "Detected ${test_name} Source : ${testsource}" )
		
			#To make sure the source file exists
			IF (testsource)
				#Create output directories
				IF ( NOT EXISTS ${PROJECT_BINARY_DIR}/test )
					FILE ( MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/test )
				ENDIF ( NOT EXISTS ${PROJECT_BINARY_DIR}/test )
			
				#Set where test executables should be found
				SET(${PROJECT_NAME}_TESTS_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test CACHE PATH "Ouput directory for ${Project} tests.")
				mark_as_advanced(FORCE ${PROJECT_NAME}_TESTS_OUTPUT_PATH)
				SET(EXECUTABLE_OUTPUT_PATH "${${PROJECT_NAME}_TESTS_OUTPUT_PATH}" CACHE INTERNAL "Internal CMake executables output directory. Do not edit." FORCE)
			
				#build
				ADD_EXECUTABLE(${test_name} ${testsource})
				TARGET_LINK_LIBRARIES(${test_name} ${PROJECT_NAME})
				ADD_DEPENDENCIES(${test_name} ${PROJECT_NAME})
				
				#We need to move project libraries and dependencies to the test target location after build.
				#We need to do that everytime to make sure we have the latest version
				
				GET_TARGET_PROPERTY(${test_name}_LOCATION ${test_name} LOCATION)
				get_filename_component(${test_name}_PATH ${${test_name}_LOCATION} PATH)

				if ( ${${PROJECT_NAME}_TYPE} STREQUAL "SHARED_LIBRARY" OR ${${PROJECT_NAME}_TYPE} STREQUAL "MODULE_LIBRARY")
					GET_TARGET_PROPERTY(${PROJECT_NAME}_LOCATION ${PROJECT_NAME} LOCATION)
					ADD_CUSTOM_COMMAND( TARGET ${test_name} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${${PROJECT_NAME}_LOCATION} ${${test_name}_PATH}
															COMMENT "Copying ${${PROJECT_NAME}_LOCATION} to ${${test_name}_PATH}" )
				endif ( ${${PROJECT_NAME}_TYPE} STREQUAL "SHARED_LIBRARY" OR ${${PROJECT_NAME}_TYPE} STREQUAL "MODULE_LIBRARY")
				#needed for each source dependency
				foreach ( looparg ${${PROJECT_NAME}_source_depends} )
					if ( ${${looparg}_TYPE} STREQUAL "SHARED_LIBRARY" OR ${${looparg}_TYPE} STREQUAL "MODULE_LIBRARY")
						ADD_CUSTOM_COMMAND( TARGET ${test_name} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${${looparg}_LOCATION} ${${test_name}_PATH}
																COMMENT "Copying ${${PROJECT_NAME}_LOCATION} to ${${test_name}_PATH}" )
					endif ( ${${looparg}_TYPE} STREQUAL "SHARED_LIBRARY" OR ${${looparg}_TYPE} STREQUAL "MODULE_LIBRARY")				
				endforeach ( looparg ${${PROJECT_NAME}_source_depends} )
				#needed for each run library dependency as well
				#message ( STATUS "Detected run libraries to copy : ${${PROJECT_NAME}_RUN_LIBRARIES}" )
				foreach ( looparg ${${PROJECT_NAME}_RUN_LIBRARIES} )
					message ( "	ADD_CUSTOM_COMMAND( TARGET ${test_name} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${looparg} ${${test_name}_PATH}
														COMMENT "Copying ${looparg} to ${${test_name}_PATH}" )
							")
				
					ADD_CUSTOM_COMMAND( TARGET ${test_name} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${looparg} ${${test_name}_PATH}
														COMMENT "Copying ${looparg} to ${${test_name}_PATH}" )
				
				endforeach ( looparg ${${PROJECT_NAME}_RUN_LIBRARIES} )
			ENDIF (testsource)
		endforeach ( test_name ${ARGN})
	ENDIF(${PROJECT_NAME}_ENABLE_TESTS)

ENDMACRO(WkTestBuild)

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
				ADD_TEST(${test_name}_${looparg} ${PROJECT_BINARY_DIR}/test/${test_name} ${looparg})
			ENDFOREACH ( looparg )
		ELSE ( ${ARGC} GREATER 2  )
			ADD_TEST(${test_name} ${PROJECT_BINARY_DIR}/test/${test_name})
		ENDIF ( ${ARGC} GREATER 2  )
	
	ENDIF(${PROJECT_NAME}_ENABLE_TESTS)

ENDMACRO(WkTestRun)
