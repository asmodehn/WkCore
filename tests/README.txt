Project A - B - C
=================
This is an example hierarchy to be used along with WkCMake framework.
In this Hierarchy A depends on B and B depends on C
C is always a shared library or a module
B is a library. it is up to the builder to make it a shared library, or a static one, and have it embedded in A
The dependency mechanism should always detect which library are needed to link and run with A

Modules tests
=============
MySql is a test project for the detection of libmysqlclient-dev library
MySQL++ is a test project for the detection of libmysql++-dev library

TODO
====
Read the TODO.txt and give us a hand :)
