#ifndef PULSAR_GUARD_EXCEPTION__ASSERT_HPP_
#define PULSAR_GUARD_EXCEPTION__ASSERT_HPP_

#include <string>

#include "pulsar/pragma.h"

namespace pulsar{
namespace exception {

PRAGMA_WARNING_PUSH
PRAGMA_WARNING_IGNORE_UNUSED_PARAMETERS

// Used for functions that only have assertions and are otherwise noexcept
#ifdef NDEBUG
  #define ASSERTIONS_ONLY noexcept
#else
  #define ASSERTIONS_ONLY
#endif

/*! \brief Asserts that a condition is true, throwing an exception if it is not
 *
 * If \p condition is false, an exception of type EX is thrown.
 * The \p description and \p args are forwarded to the constructor
 * of the exception.
 */
template<typename EX, typename ... Targs>
void Assert(bool condition, const std::string & description, Targs &&... args) ASSERTIONS_ONLY
{
    #ifndef NDEBUG
    if(!condition)
        throw EX(description, std::forward<Targs>(args)...);
    #endif
}

PRAGMA_WARNING_POP





} // close namespace exception
} // close namespace pulsar

#endif

