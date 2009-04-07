macro( WKDoc Doxygen_config)
find_package(Doxygen)

set(DOXYFILE_FOUND false)

if(EXISTS ${PROJECT_SOURCE_DIR}/${Doxygen_config})
	set(DOXYFILE_FOUND true)
endif(EXISTS ${PROJECT_SOURCE_DIR}/${Doxygen_config})


if(DOXYGEN_FOUND)
	if(DOXYFILE_FOUND)
		#Add target
		ADD_CUSTOM_TARGET( doc ALL ${DOXYGEN_EXECUTABLE} "${PROJECT_SOURCE_DIR}/${Doxygen_config}" )
		
		#Add .tag file and generated documentation to the list of files we must erase when distcleaning
		
		#Read dosygen configuration file
		FILE( READ ${PROJECT_SOURCE_DIR}/${Doxygen_config} DOXYFILE_CONTENTS )
		STRING( REGEX REPLACE "\n" ";" DOXYFILE_LINES ${DOXYFILES_CONTENTS} )
		
		#Parse .tag filename and add to list of files to delete if it exists
		foreach( DOXYLINE ${DOXYFILE_CONTENTS} )
			STRING (REGEX REPLACE ".*GENERATE_TAGFILE *= *([^^\n]+).*" "\\1" DOXYGEN_TAG_FILE ${DOXYLINE} )
		endforeach( DOXYLINE )
		
		ADD_TO_DISTCLEAN( ${PROJECT_BINARY_DIR}/${DOXYGEN_TAG_FILE} )
		
		# Parse doxygen output doc dir and add to list of filees to delete if it exists
		foreach( DOXYLINE ${DOXYFILE_CONTENTS} )
			STRING( REGEX REPLACE ".*OUTPUT_DIRECTORY *= *([^^\n]+).*" "\\1" DOXYGEN_DOC_DIR ${DOXYLINE} )
		endforeach( DOXYLINE )

		ADD_TO_DISTCLEAN( ${PROJECT_BINARY_DIR}/${DOXYGEN_DOC_DIR} )
		ADD_TO_DISTCLEAN( ${PROJECT_BINARY_DIR}/${DOXYGEN_DOC_DIR}.dir )
		
	else (DOXYFILE_FOUND)
		message ( STATUS "Doxygen configuration file not found - Documentation will not be generated" )
	endif(DOXYFILE_FOUND)

else( DOXYGEN_FOUND)
	message( STATUS "Doxygen not found - Documentation will not be generated" )
endif(DOXYGEN_FOUND)

endmacro( WKDoc Doxygen_config)
