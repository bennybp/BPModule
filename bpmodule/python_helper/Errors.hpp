/*! \file
 *
 * \brief Handling of python errors (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_ERRORS_HPP_
#define _GUARD_ERRORS_HPP_

#include <boost/python.hpp>

namespace bpmodule {
namespace python_helper {
namespace detail {


/*! \brief Get the current python exception description as a string
 */
std::string GetPyExceptionString(void);


} // close namespace detail
} // close namespace python_helper
} // close namespace bpmodule

#endif
