CMAKE_MINIMUM_REQUIRED ( VERSION 2.6)
INCLUDE ( CMake/WkCMake.cmake )
WkCMakeDir ( CMake )

WkProject ( WkCore CXX)

#Temporary : STLport doesnt have enough support yet to allow us to play around with it.
#TODO : Optional loading of optional depends with check box in cmake-gui...
#WkDepends (STLPort )

WkDepends(Threads)

#TODO :
# remove sigC++ dependency because
# - delegate mecanism already implemented and working.
# For simpler signal slots, check http://code.google.com/p/nano-signal-slot/source/browse/nano_signal_slot.hpp
# - for more complex communication pattern : use 0mq

# TODO : Add Ncurses dependency for UNIX type platform
# maybe Add PDCurses dependency for Windows type platform
# check CMake Findcurses module
# see what we can do with it...

# TODO : Logger improve : Unable to find decent logging library.
# Gather ideas, remake teh wheel, and roll out my own...

# TODO : add a find module for unittest++ ( and cppunit ) so they are easy to integrate, even from source (WkCMake style )
add_subdirectory (depends/UnitTest++)
include_directories(depends/UnitTest++)

#TODO : Check if we need it or not ( since C++11 we do not need it )
# This means improve wkcmake
#add_subdirectory (depends/tinythreadpp)
#include_directories(depends)

WkBuild ( LIBRARY STATIC )

#TMP Workaround limitation : Threads depends doesnt follow standard depends and is not supported in wkcmake at the moment.
target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

WkDoc()
#WkSvn()

WkInstall()

#TestBed

# From most basic feature to most advanced :
# Logger
WkTestBuild( TestLogger )
WkTestRun( Logger TestLogger )
# Time
WkTestBuild( TestTime )
WkTestRun( Time TestTime )
# Randomizer
WkTestBuild ( TestRandomizer )
WkTestRun ( Randomizer TestRandomizer )

# Memory
WkTestBuild( TestSharedPtr )
#check if we have enabled tests
#TODO : improve WkTestBuild, or add more functions to support automatically external unit testing libs
if ( TARGET TestSharedPtr )
target_link_libraries( TestSharedPtr UnitTest++)
endif ( TARGET TestSharedPtr )
WkTestRun( SharedPtr TestSharedPtr)

#Thread
WkTestBuild( TestThread )
#check if we have enabled tests
#TODO : improve WkTestBuild, or add more functions to support automatically external unit testing libs
if ( TARGET TestThread )

#NOT NEEDED since VS2012
#target_link_libraries( TestThread UnitTest++ TinyThread++)

endif ( TARGET TestThread )
WkTestRun( Thread TestThread)

#others... to organise...
#disabled : doesnt build on all target platforms yet.
#WkTestBuild( TestError )
#WkTestBuild( TestDbgMem )
#WkTestRun( DbgMem TestDbgMem )
WkTestBuild( TestStaticAssert )
WkTestRun( StaticAssert TestStaticAssert )
WkTestBuild ( TestFunctor )
WkTestRun( Functor TestFunctor )
WkTestBuild ( TestCallback )
WkTestRun( Callback TestCallback )
WkTestBuild ( TestAdapter )
WkTestRun( Adapter TestAdapter )
