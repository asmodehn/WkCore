#ifndef CURRENTFUNCTION_HH
#define CURRENTFUNCTION_HH

/**
 * Find the current function signature.
 * Compiler dependent before stdC 199901
 */

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)

# define CORE_CURRENT_FUNCTION __func__

#else

# define CORE_CURRENT_FUNCTION "(Unknown Func)"

#endif





#endif // CURRENTFUNCTION_HH
