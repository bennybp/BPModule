/*! \file
 * \brief General-purpose assertions (header)
 */

#pragma once

#include <utility> // for std::forward

#include "pulsar/exception/Exceptions.hpp"


namespace pulsar {

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
 * that the function only throws exceptions via the `psr_assert` and
 * `psr_assert_ptr` functions.
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


/*! \brief Asserts that a pointer is not null
 *
 * If \p ptr is null, then a NullPointerException is thrown with
 * the given message
 */
inline void psr_assert_ptr(void * ptr, const char * message)
{
    if(ptr == nullptr)
        throw NullPointerException(message);    
}

#else

#define ASSERTIONS_ONLY noexcept

template<typename EX, typename... Targs>
void psr_assert(bool, Targs &&...) noexcept
{
}

inline void psr_assert_ptr(void * ptr, const char * msg)
{
}

#endif

} // close namespace pulsar
