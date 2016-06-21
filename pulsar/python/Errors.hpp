/*! \file
 *
 * \brief Handling of python errors (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_PYTHON__ERRORS_HPP_
#define PULSAR_GUARD_PYTHON__ERRORS_HPP_

#include <string>

namespace pulsar{
namespace python {
namespace detail {


/*! \brief Get the current python exception and convert to a GeneralException
 *
 * Can handle some pulsar-derived exception classes
 */
std::string get_py_exception(void);



} // close namespace detail
} // close namespace python
} // close namespace pulsar

#endif
