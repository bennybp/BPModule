/*! \file
 *
 * \brief Memory accounting
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_UTIL__MEMWATCH_HPP_
#define PULSAR_GUARD_UTIL__MEMWATCH_HPP_


namespace pulsar{

/*! \brief See if memwatch is running (preloaded)
 */ 
bool memwatch_running(void) noexcept;


/*! \brief Query how much memory is in use (has been allocated already) in bytes
 */ 
size_t memwatch_allocated(void) noexcept;


/*! \brief Set a limit as to how much memory to use (in bytes)
 *
 * Allocating more than the limit causes the callback to be called
 *
 * \return The previous limit
 */
size_t memwatch_setlimit(size_t limit) noexcept;


/*! \brief Get the current memory limit (in bytes)
 */
size_t memwatch_getlimit(void) noexcept;

} // close namespace pulsar

#endif
