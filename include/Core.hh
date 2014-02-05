#ifndef CORE_HH
#define CORE_HH


///Main include files to load Core features needed for virtual environments applications.

/**
 * The library "Core" includes :
 * Date and Time ( designed from on Boost Date_Time )
 * Random Number Generation
 * Functor and Callback mechanisms ( custom design, based on STL )
 * Portable Threads ( designed from on Boost Threads )
 * Thread safe Logger
 * Assertions
 * Static Assertion
 * C / C++ Memory Debugger
 * Other useful low level portable C++ features to come...
 *
 * It is aimed at providing simple and easy to use low-level features,
 * that are often useful, even when they are not performant, waiting for a standardized implementation
 * It also aims at being much lighter than Boost or even POCO...
 * and relies on C99 compliant compilers.
 */

//since Core relies on STLport
#ifdef WK_Core_FOUND_STLPORT

//we enable debug in STLport if we are in debug mode
#ifdef _DEBUG
#define _STLP_DEBUG 1
#endif

#endif


///Assertions
#include "Assert.hh"

///Date and Time
#include "Time.hh"

///Random Number Generation
#include "Randomizer.hh"
//normal gaussian, and poisson distribution to do
#include "Distribution.hh"

///Functor and Callback headers
#include "Functor.hh"
#include "Callback.hh"
#include "Adapter.hh"
///Optional alternative ... TODO
//#ifdef WK_SigCxx_FOUND
//#include "sigc++.h"
//#endif

///Portable Threads

///Thread-aware Logger
#include "Logger.hh"
#include "Core/NewLogger.hh"

///Static Assertion
#include "StaticAssert.hh"

///Memory Debugger
#include "dbgmem.h"


///Others ? or is that all ??






#endif
