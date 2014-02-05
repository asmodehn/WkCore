Prerequisites
=============

To build the software, you need :
- CMake
- a build environment and compiler supported by CMake such as VS or GNUMake / GCC ( cf CMake supported build systems )

To build the doc you need :
- doxygen to generate the code documentation.
- XMindXmlEditor to edit and generate the user documentation from XML Docbook files.

Build
=====

As with every CMake build, you can use ccmake or CMakeSetup.exe to setup all your variables for your build.

Once your build files have been generated, just build the software as you are used to do.

Example : On Unix System for KDevelop:
> cd [Your SVN_AML_Checkout_Dir]
> mkdir build-kdev
> mkdir build-kdev/SDLut
> cd build-kdev/SDLut
> ccmake -KDevelop3 ../../SDLut

Then you setup up your variables, and generate build files.

> make

If everything has been built successfully, you can try :

> ctest
