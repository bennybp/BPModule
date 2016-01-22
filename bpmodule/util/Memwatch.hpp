/*! \file
 *
 * \brief Memory accounting
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef BPMODULE_GUARD_UTIL__MEMWATCH_HPP_
#define BPMODULE_GUARD_UTIL__MEMWATCH_HPP_


namespace bpmodule {
namespace util {


/*! \brief See if memwatch is running (preloaded)
 */ 
bool Memwatch_running(void) noexcept;


/*! \brief Query how much memory is in use (has been allocated already) in bytes
 */ 
size_t Memwatch_allocated(void) noexcept;


/*! \brief Set a limit as to how much memory to use (in bytes)
 *
 * Allocating more than the limit causes the callback to be called
 *
 * \return The previous limit
 */
size_t Memwatch_setlimit(size_t limit) noexcept;


/*! \brief Get the current memory limit (in bytes)
 */
size_t Memwatch_getlimit(void) noexcept;



} // close namespace util
} // close namespace bpmodule

#endif
