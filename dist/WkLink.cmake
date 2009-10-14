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
message ( STATUS "== Loading WkLink.cmake ..." )

if ( CMAKE_BACKWARDS_COMPATIBILITY LESS 2.6 )
	message ( FATAL_ERROR " CMAKE MINIMUM BACKWARD COMPATIBILITY REQUIRED : 2.6 !" )
endif( CMAKE_BACKWARDS_COMPATIBILITY LESS 2.6 )

#test to make sure necessary variables have been set.

if ( NOT WKCMAKE_DIR OR NOT WKCMAKE_BIN_DIR OR NOT WKCMAKE_LIB_DIR ) 
	message( FATAL_ERROR "You need to include WkCMake.cmake in your CMakeLists.txt, and call WkCMakeDir(<path_to WkCMake scripts> )" )
endif ( NOT WKCMAKE_DIR OR NOT WKCMAKE_BIN_DIR OR NOT WKCMAKE_LIB_DIR ) 

#
# Declaring Dependencies at link stage
# The dependencies used here must be all dependencies needed
# both by this project and its dependencies as well
# They will all be forwarded to client projects
#
macro(WkDependsLink package_name)
CMAKE_POLICY(PUSH)
CMAKE_POLICY(VERSION 2.6)
	
	SetPackageVarName( package_var_name ${package_name} )
	#message ( "${package_name} -> ${package_var_name}" )

	if ( ${package_var_name}_FOUND )

		#hiding the original cmake Module variable, displaying the WkCMake later on
		mark_as_advanced ( FORCE ${package_var_name}_LIBRARY ) 

		# to handle cmake moule who dont have exactly the same standard as WkModules
		if ( NOT ${package_var_name}_LIBRARIES )
			set ( ${package_var_name}_LIBRARIES ${${package_var_name}_LIBRARY} CACHE FILEPATH "${package_name} Libraries ")
		endif ( NOT ${package_var_name}_LIBRARIES )
		#todo : maybe we need a complete layer over that, Wk Modules handling Wk fetures such as run_libraries and correct variable name...

		target_link_libraries(${PROJECT_NAME} ${${package_var_name}_LIBRARIES})
		message ( STATUS "== Binary Dependency ${package_name} libs : ${${package_var_name}_LIBRARIES} OK !")
		#if the find module also defines the runtime libraries ( Wk find module standard  NOT CMAKE itself !)
		set( ${PROJECT_NAME}_RUN_LIBRARIES ${${PROJECT_NAME}_RUN_LIBRARIES} ${${package_var_name}_RUN_LIBRARIES} CACHE FILEPATH " ${package_name} libraries needed to run ${PROJECT_NAME} " )
		mark_as_advanced( FORCE ${PROJECT_NAME}_RUN_LIBRARIES )
		IF ( WIN32 )
			message ( STATUS "== Binary Dependency ${package_name} runlibs : ${${package_var_name}_RUN_LIBRARIES} OK !")
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
mark_as_advanced( ${package_name}_INCLUDE_DIR )
mark_as_advanced( ${package_name}_LIBRARY )

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
endmacro(WkDependsLink package_name)


#
# WkLink () express the link procedure of the project, finalizing the build step
# gathering all dependencies settings in one simple place
# and creating the necessary lines in the config file for detection by other projects.
#
macro(WkLink )
	CMAKE_POLICY(PUSH)
	CMAKE_POLICY(VERSION 2.6)

	file( APPEND ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake "

#Displaying detected dependencies in interface, and storing in cache
set(${PROJECT_NAME}_INCLUDE_DIRS \"\${${PROJECT_NAME}_INCLUDE_DIRS}\" CACHE PATH \"${PROJECT_NAME} Headers\" )
set(${PROJECT_NAME}_LIBRARIES \"\${${PROJECT_NAME}_LIBRARIES}\" CACHE FILEPATH \"${PROJECT_NAME} Libraries\")
if ( WIN32 )
	set(${PROJECT_NAME}_RUN_LIBRARIES CACHE FILEPATH \"${PROJECT_NAME} DLLs\" )
endif ( WIN32 )	

CMAKE_POLICY(POP)
	
	")

	CMAKE_POLICY(POP)
endmacro(WkLink )

