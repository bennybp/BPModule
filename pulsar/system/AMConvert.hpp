/*! \file
 *
 * \brief Conversion of angular momentum info to human-readable forms (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_SYSTEM__AMCONVERT_HPP_
#define PULSAR_GUARD_SYSTEM__AMCONVERT_HPP_

#include <string>

namespace pulsar{
namespace system {


/*! \brief Converts a string ("s", "p", etc) to an integer angular momentum
 *
 * The string is not case sensitive
 *
 * \throw pulsar::BasisSetException if the string is not found
 */
int string_to_am(const std::string & s);



/*! \brief Converts an integer angular momentum (0, 1, 2) to a string string ("s", "p", "d")
 *
 * The string is always lowercase
 *
 * \throw pulsar::BasisSetException if the integer is not found
 */
std::string am_to_string(int am);



} // close namespace system
} // close namespace pulsar

#endif
