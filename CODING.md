If you are reading this it means you are likely to code something in WkCore.
First of all thanks, and congratulations for your dedication and your help ;)

Overview
========

The rule is the content of the Git repository (on Github) should be able to be built (and test) successfully anytime, on any platform.
So one must only commit a source code when this one builds without errors.

There is no automated build at the moment, but this is planned.

Folder Structure
================

Each module has some build files :
- CMakeLists.txt : the CMake initial source file. This file should be modified with extreme care and testing

And also few subdirectories:

- CMake : Contains the FindModule.cmake necessary for others Modules to use this one.
- data : Contains useful data for its use in final environment ( not for testing as an example ). One should be careful that these data should be delivered somehow with the binaries
- doc : Contains the doxygen files and other xml, to be able to generate docbook and doxygen doc.
- include : Contains the header files for clients to use this module.
- src : Contains the implementation files and headers that should stay hidden from the client.
- test : Contains the implementation of useful tests. Building them depend on your CMake options, and running them can be done via CTest
- test/data : Contains the data useful for testing

We usually advise to build the source in a separate directory, so your source directory stays clean.

After a build some directories are present in the binary folder :
- bin : containing all the executables ( even tests )
- lib : containing libs that are usefull to the current lib, or the built executable
- test/data : contain a copy of the data to test.

Therefore by hand the test should be run from test :

	cd test
	../bin/myTest.exe

This scheme has been chosen because it works well also in Visual studio which has its own way...


SOURCE CODE
===========

The code around here is all in C++.

Generic principles
------------------

Ideas and concepts that we think everyone should to follow (even if we don't do half of it now, this is where we would like to go)

- KISS : Keep it Stupidly simple -> Dont add lots of complicated feature nobody wants now, just to get prepare for the future. You will loose time and this future might never come, so you will loose time again to undo everything you ve done.
- Resource Acquisition is Initialization -> Every object should be responsible for its members, except if there is a good reason to do otherwise, and in that case should be well documented.
- Proactive Safety -> Coding the tests first then code the feature. The feature doesn't get committed until it builds,and doesn't get delivered until all the tests are successful.
- Code in proper C++ / STL object for modularity, but don't forget that C++ is still raw imperative C code with some additions, therefore using less but bigger objects might be better than lots of small different ones. However proper OOD advise for one class for one purpose only. These must be balanced against each other before deciding on a specific design.
- When your code works, please, please clean it. There is nothing worse for maintainability later than unused dirty piece of code. Lets keep it clean. Along with that, document it, so everybody knows what this stuff should do.
- Add anything else here that seems useful and sensible to you ...

All these ideas might be applied in the code in different ways, as an example for safety, proactive constification of your code is really simple to do and can avoid lots of tricky bugs...


Last but not least : Enjoy ;)
