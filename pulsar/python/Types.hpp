/*! \file
 *
 * \brief Handling and detection of python types (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_PYTHON__TYPES_HPP_
#define PULSAR_GUARD_PYTHON__TYPES_HPP_

#include <string>

#include <pybind11/pybind11.h>


namespace pulsar{
namespace python {


/*! \brief Get the class of a python object as a string
 */
std::string get_py_class(const pybind11::object & obj);


/*! \brief Check if a python object has a given attribute
 */
bool has_attr(const pybind11::object & obj, const std::string & attr);


/*! \brief Check if a python object is callable
 */ 
bool is_callable(const pybind11::object & obj);


/*! \brief Check if a python object has a given callable attribute
 */
bool has_callable_attr(const pybind11::object & obj, const std::string & attr);


} // close namespace python
} // close namespace pulsar

#endif
