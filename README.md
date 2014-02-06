WkCore
======

--------------------
  C++  |   WkCore  |
--------------------
         C         |
--------------------

C is widely used and has a lots of different implementation on different platforms.
WkCore library fills the missing space in C++ to use the old C calls in a new "C++" way.

WkCore aim to be completely cross-platform and portable on any system, with any compiler.
It can also sometime provide implementation of recent feature in old standard with old compilers.
It is widely and shamelessly based on boost, gathering useful components.

The main platform we aim to support are :
- Win32, cygwin, ndk-build
- Linux, ndk-build
- MacOSX, ios-build

The goal is to have a very simple one line string input, and one line string output (log), yet using all possible functionalities on the system, the C++ way. Be ready for heavy OOD, templates, streams, threads, etc.

However, to keep it maintainable, we should restrain ourselves to cover less features than [POCO](http://pocoproject.org/)
More especially : 
- No XML Support ( this should be a separated library )
- No Zip Support ( this should be a separated library )
- No Database Support ( this should be a separated library )
- add more thing to NOT do here...