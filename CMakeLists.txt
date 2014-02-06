CMAKE_MINIMUM_REQUIRED ( VERSION 2.6)
INCLUDE ( CMake/WkCMake.cmake )
WkCMakeDir ( CMake )

WkProject ( WkCore CXX)

#Temporary : STLport doesnt have enough support yet to allow us to play around with it.
#TODO : Optional loading of optional depends with check box in cmake-gui...
#WkDepends (STLPort )

#TODO :
# remove sigC++ dependency because
# - delegate mecanism already implemented and working.
# - for more complex communication pattern : use 0mq

# TODO : Add Ncurses dependency for UNIX type platform
# maybe Add PDCurses dependency for Windows type platform
# check CMake Findcurses module
# see what we can do with it...

# TODO : Logger improve : Unable to find decent logging library.
# Gather ideas, remake teh wheel, and roll out my own...

WkBuild ( LIBRARY STATIC )

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

#others... to organise...
#disabled : doesnt build on all target platforms yet.
#WkTestBuild( TestError )
WkTestBuild( TestDbgMem )
WkTestRun( DbgMem TestDbgMem )
WkTestBuild( TestStaticAssert )
WkTestRun( StaticAssert TestStaticAssert )
WkTestBuild ( TestFunctor )
WkTestRun( Functor TestFunctor )
WkTestBuild ( TestCallback )
WkTestRun( Callback TestCallback )
WkTestBuild ( TestAdapter )
WkTestRun( Adapter TestAdapter )
