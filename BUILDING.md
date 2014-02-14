Prerequisites
=============

To build the software, you need :
- CMake
- a build environment and compiler supported by CMake such as VS or GNUMake / GCC ( cf CMake supported build systems ) supporting the new C++11 standard.
Since very few compilers implement the full C++11 specifications, latest compilers should be used.
For older compilers versions, other branches of WkCore exist (NO_Cpp11_std_bind) and more can be created if necessary.

To build the doc you need :
- doxygen to generate the code documentation.
- XMindXmlEditor to edit and generate the user documentation from XML Docbook files.

Build
=====

An online build is nicely provided by [Travis-CI](https://travis-ci.org/asmodehn/WkCore)

As with every CMake build, you can use ccmake or CMakeSetup.exe to setup all your variables for your build.

Once your build files have been generated, just build the software as you are used to do.

Example : On Unix System for KDevelop:
	cd [Your SVN_AML_Checkout_Dir]
	mkdir build-kdev
	mkdir build-kdev/SDLut
	cd build-kdev/SDLut
	ccmake -KDevelop3 ../../SDLut

Then you setup up your variables, and generate build files.

	make

If everything has been built successfully, you can try :

	ctest
