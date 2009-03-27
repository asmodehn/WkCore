This is an example hierarchy to be used along with WkCMake framework.

In this Hierarchy A depends on B and B depends on C

C is always a shared library or a module

B is a library. it is up to the builder to make it a shared library, or a static one, and have it embedded in A

The dependency mechanism should always detect which library are needed to link and run with A