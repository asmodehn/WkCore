//
// Assert Function for use in Core library
// No include guards, just like in cassert and assert.h
//


#if defined(CORE_DISABLE_ASSERTS)

# define CORE_ASSERT(expr) ((void)0)

#elif defined(CORE_ENABLE_ASSERT_HANDLER)

# include "CurrentFunction.hh"

//To be able to redefine assertion failed behavior
namespace Core
{

void assertion_failed(char const * expr, char const * function, char const * file, long line);

} // namespace Core

#define CORE_ASSERT(expr) ((expr)? ((void)0): ::Core::assertion_failed(#expr, CORE_CURRENT_FUNCTION, __FILE__, __LINE__))

#else
# include <cassert>
# define CORE_ASSERT(expr) assert(expr)
#endif

#undef CORE_VERIFY

#if defined(CORE_DISABLE_ASSERTS) || ( !defined(CORE_ENABLE_ASSERT_HANDLER) && defined(NDEBUG) )

# define CORE_VERIFY(expr) ((void)(expr))

#else

# define CORE_VERIFY(expr) CORE_ASSERT(expr)

#endif
