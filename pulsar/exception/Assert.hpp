/*! \file
 * \brief General-purpose assertion (header)
 */

#pragma once

#include <utility> // for std::forward

namespace pulsar {
namespace exception {

#ifndef NDEBUG


/*! \brief Will resolve to `noexcept` if assertions are disabled
 *         via the `NDEBUG` macro.
 *
 * This is used to label functions that will only throw exceptions
 * if assertions are not disabled. That is,
 *
 * \code{.cpp}
 *    void some_function(void) ASSERTIONS_ONLY
 * \endcode
 *
 * will be marked as noexcept if assertions are disabled. It is expected
 * that the function only throws exceptions via the `psr_assert` function.
 */
#define ASSERTIONS_ONLY

/*! \brief Asserts that a condition is true, throwing an exception if it is not
 *
 * If \p condition is false, an exception of type \p EX is thrown.
 * The remaining arguments are forwarded to the constructor of the exception.
 */
template<typename EX, typename... Targs>
void psr_assert(bool condition, Targs &&... args)
{
    if(!condition)
        throw EX(std::forward<Targs>(args)...);
}

#else

#define ASSERTIONS_ONLY noexcept

template<typename EX, typename... Targs>
void psr_assert(bool, Targs &&...) noexcept
{ }

#endif

} // close namespace exception
} // close namespace pulsar

