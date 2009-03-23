WkCMake is aimed at defining a stable hierarchy to build projects,
along with cmake files able to build them and link them together in different ways

WkCMake currently supports :

 - only one main target per project hierarchy 
 - Few simple executable tests ( in test/ subdirectory ) that dont require additional dependencies
 - target can be executable, library shared or static (module not tested yet, need a Mac...)
 - source dependency ( in ext/ subdirectory )
 - binary dependency (in another Wk build directory )

Released under BSD License. Please check the LICENSE.txt file.

