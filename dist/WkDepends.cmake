# 
# Copyright (c) 2009-2010, Asmodehn's Corp.
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
message ( STATUS "== Loading WkDepends.cmake ..." )

if ( CMAKE_BACKWARDS_COMPATIBILITY LESS 2.6 )
	message ( FATAL_ERROR " CMAKE MINIMUM BACKWARD COMPATIBILITY REQUIRED : 2.6 !" )
endif( CMAKE_BACKWARDS_COMPATIBILITY LESS 2.6 )

# using useful Macros
include ( "${WKCMAKE_DIR}/WkUtils.cmake" )

#
# Find a dependency built in an external WK hierarchy
# Defines ${PROJECT_NAME_DEPENDS} containing all found dependencies
#
# WkDepends( dependency_name [QUIET / REQUIRED] )

macro (WkDepends package_name)
CMAKE_POLICY(PUSH)
CMAKE_POLICY(VERSION 2.6)
	
	#
	# First check if the package is installed already , quietly
	#
	
	# if possible by using WkFind modules
	# maybe belongs somewhere else
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/${WKCMAKE_DIR}/Modules/")
	
	SetPackageVarName( package_var_name ${package_name} )
	#message ( "${package_name} -> ${package_var_name}" )

	#Here to avoid redefinition of library target if already found by a dependency using my dependency as well...
	if ( NOT ${package_var_name}_FOUND )
		find_package( ${package_name} ${ARGN} )
	endif ( NOT ${package_var_name}_FOUND )
	
	if ( ${package_var_name}_FOUND )
	
		#hiding the original cmake Module variable, displaying the WkCMake later on
		mark_as_advanced ( FORCE ${package_var_name}_INCLUDE_DIR )

#		# to handle cmake modules who dont have exactly the same standard as WkModules
#		if ( NOT ${package_var_name}_INCLUDE_DIRS )
#			set ( ${package_var_name}_INCLUDE_DIRS ${${package_var_name}_INCLUDE_DIR} CACHE PATH "${package_name} Headers directories")
#		endif ( NOT ${package_var_name}_INCLUDE_DIRS )

#		#dependencies headers ( need to be included after project's own headers )
#		include_directories(${${package_var_name}_INCLUDE_DIRS})
#		message ( STATUS "== Binary Dependency ${package_name} include : ${${package_var_name}_INCLUDE_DIRS} OK !")

		set ( WK_${package_var_name}_FOUND ON )
		#this is not necessary if WkPlatform does the job as it should
		#add_definitions(-D WK_${package_var_name}_FOUND)

		message ( STATUS "== Binary Dependency ${package_name} : FOUND ! " )

		#Simple if structure to avoid oddities when setting ${PROJECT_NAME}_DEPENDS for the first time
		if ( ${PROJECT_NAME}_DEPENDS)
			set ( ${PROJECT_NAME}_DEPENDS "${PROJECT_NAME_DEPENDS}" "${package_var_name}" ) 
		else ( ${PROJECT_NAME}_DEPENDS)
			set ( ${PROJECT_NAME}_DEPENDS "${package_var_name}" ) 
		endif ( ${PROJECT_NAME}_DEPENDS)
		
	else ( ${package_var_name}_FOUND )	
		message ( STATUS "== Binary Dependency ${package_name} : NOT FOUND ! " )
	endif ( ${package_var_name}_FOUND )

CMAKE_POLICY(POP)
endmacro (WkDepends package_name)

#
# Joining Dependencies for build
# They will all be forwarded to client projects
#
macro(WkLinkDepends package_name)
CMAKE_POLICY(PUSH)
CMAKE_POLICY(VERSION 2.6)
	
	SetPackageVarName( package_var_name ${package_name} )
	#message ( "${package_name} -> ${package_var_name}" )

	if ( ${package_var_name}_FOUND )

		# to handle cmake modules who dont have exactly the same standard as WkModules
		if ( NOT ${package_var_name}_INCLUDE_DIRS )
			set ( ${package_var_name}_INCLUDE_DIRS ${${package_var_name}_INCLUDE_DIR} CACHE PATH "${package_name} Headers directories")
		endif ( NOT ${package_var_name}_INCLUDE_DIRS )

		#dependencies headers ( need to be included after project's own headers )
		include_directories(${${package_var_name}_INCLUDE_DIRS})
		message ( STATUS "== Binary Dependency ${package_name} include : ${${package_var_name}_INCLUDE_DIRS} OK !")

		#hiding the original cmake Module variable, displaying the WkCMake later on
		mark_as_advanced ( FORCE ${package_var_name}_LIBRARY ) 
		
		# to handle cmake moule who dont have exactly the same standard as WkModules
		if ( NOT ${package_var_name}_LIBRARIES )
			set ( ${package_var_name}_LIBRARIES ${${package_var_name}_LIBRARY} CACHE FILEPATH "${package_name} Libraries ")
		endif ( NOT ${package_var_name}_LIBRARIES )
		#todo : maybe we need a complete layer over that, Wk Modules handling Wk fetures such as run_libraries and correct variable name...

		target_link_libraries(${PROJECT_NAME} ${${package_var_name}_LIBRARIES})
		message ( STATUS "== Binary Dependency ${package_name} libs : ${${package_var_name}_LIBRARIES} OK !")
		
		IF ( WIN32 )
			message ( STATUS "== Binary Dependency ${package_name} runlibs : ${${package_var_name}_RUN_LIBRARIES} OK !")
			#if the find module also defines the runtime libraries ( Wk find module standard  NOT CMAKE itself !)
			# then we need to add the run libraries of the dependency to the current project myrun libraries
			#set( ${PROJECT_NAME}_RUN_LIBRARIES "${${PROJECT_NAME}_RUN_LIBRARIES}" "${${package_var_name}_RUN_LIBRARIES}" CACHE FILEPATH "blabla")
			#message( "Project run lib WkDepends : ${${PROJECT_NAME}_RUN_LIBRARIES} " )


		ENDIF ( WIN32 )
		
		# Once the project is built with it, the dependency becomes mandatory
		# However we need to propagate the location of Custom Wk-dependencies, to make it easier for later
		if ( ${package_name}_DIR )
			get_filename_component( ${package_name}_FDIR ${${package_name}_DIR} ABSOLUTE )
		endif ( ${package_name}_DIR )

		# we append to the config cmake script
		file( APPEND ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake "

### External Dependency ${package_name} ###

		")

		#If it s a custom Wk-dependency we can propagate the build directory
		if ( ${package_name}_DIR )
			file( APPEND ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake "

#However we already know where ${package_name} is going to be
set ( ${package_name}_DIR ${${package_name}_FDIR} CACHE PATH \"Imported location of ${package_name} from ${PROJECT_NAME}\" )

			")
		endif ( ${package_name}_DIR )

		file( APPEND ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake "

find_package( ${package_name} )
#hiding depends variables from interface
mark_as_advanced( ${package_var_name}_DIR )
mark_as_advanced( ${package_var_name}_INCLUDE_DIRS )
mark_as_advanced( ${package_var_name}_LIBRARIES )
mark_as_advanced( ${package_var_name}_RUN_LIBRARIES )

# Include directory might be needed by upper project if ${PROJECT_NAME} doesn totally encapsulate it.
# NB : It shouldnt hurt if the upper project also define it as its own dependency
set(${PROJECT_NAME}_INCLUDE_DIRS \${${PROJECT_NAME}_INCLUDE_DIRS} \"${${package_var_name}_INCLUDE_DIRS}\" )
set(${PROJECT_NAME}_LIBRARIES \${${PROJECT_NAME}_LIBRARIES} \"${${package_var_name}_LIBRARIES}\")
if ( WIN32 )
	set(${PROJECT_NAME}_RUN_LIBRARIES \${${PROJECT_NAME}_RUN_LIBRARIES} \"${${package_var_name}_RUN_LIBRARIES}\")
endif ( WIN32 )	

		")
		
	else ( ${package_var_name}_FOUND )	
		message ( STATUS "== Binary Dependency ${package_name} : FAILED ! " )
	endif ( ${package_var_name}_FOUND )
	
CMAKE_POLICY(POP)
endmacro(WkLinkDepends package_name)





