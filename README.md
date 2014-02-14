WkCore [![Build Status](https://travis-ci.org/asmodehn/WkCore.png?branch=master)](https://travis-ci.org/asmodehn/WkCore)
======

|    WkCore        |
--------------------
|  C++  |  WkCore  |
--------------------
|   C   |     C    |


C is widely used and has a lots of different implementation on different platforms. WkCore library fills the missing space in C++ to use the old C calls in a new "C++" way.

WkCore aim to be completely cross-platform and portable on any system, with any compiler.It can also sometime provide implementation of recent feature in old standard with old compilers.It is widely and shamelessly based on boost, gathering useful components.

With C++11 standard now being implemented in compiler, the master branch of WkCore will continue moving forward to provide the foundation for multi-platform development on top of C++11 standard. Other branches will be created for older compiler support when needed.

The main platform we aim to support are :

- Win32, cygwin, ndk-build
- Linux, ndk-build
- MacOSX, ios-build

The goal is to have a very simple one line string input, and one line string output (log), yet using all possible features of the OS, the C++ way. Be ready for heavy OOD, templates, streams, threads, etc.

However what can be usable in ISO C and is doable in C should be done in C.
Also specific features that are usable and doable in older C++ standards should be done in older C++ standard.
More libraries can spawn from WkCore code to cover these scenarios.
As of now, these libraries used only on some specific configurations by WkCore are :

- [TinyThread++](https://github.com/asmodehn/tinythreadpp)

Also, to keep it maintainable, we should restrain ourselves to cover less features than [POCO](http://pocoproject.org/)
More especially : 

- No XML Support ( this should be a separated library )
- No Zip Support ( this should be a separated library )
- No Database Support ( this should be a separated library )
- add more thing to NOT do here...