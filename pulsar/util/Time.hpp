/*! \file
 *
 * \brief Some miscellaneous time functions (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_UTIL__TIME_HPP_
#define PULSAR_GUARD_UTIL__TIME_HPP_

#include <string>

namespace pulsar {
namespace util {


/*! \brief Obtains the current time as a nice, formatted string
 */
std::string timestamp_string(void);


} // close namespace util
} // close namespace pulsar


#endif
