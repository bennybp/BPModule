/*! \file
 *
 * \brief Formatting of python exceptions (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_PYTHON__ERRORS_HPP_
#define PULSAR_GUARD_PYTHON__ERRORS_HPP_

#include <string>
#include <utility>

namespace pulsar{
namespace python {
namespace detail {


/*! \brief Get the current python exception and convert to a string
 *
 * Can handle pulsar::GeneralException and python exceptions that are
 * convertible to a string
 *
 * \return Pair of strings. The first is the description, the second is
 *         the python traceback
 */
std::pair<std::string, std::string> get_py_exception(void);



} // close namespace detail
} // close namespace python
} // close namespace pulsar

#endif
