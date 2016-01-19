/*! \file
 *
 * \brief Handling and detection of python types (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_PYTHON__TYPES_HPP_
#define BPMODULE_GUARD_PYTHON__TYPES_HPP_

#include <string>

#include "bpmodule/python/Pybind11_fwd.hpp"


namespace bpmodule {
namespace python {


/*! \brief Get the class of a python object as a string
 */
std::string GetPyClass(const pybind11::object & obj);


/*! \brief Determine if an object is "None"
 */
bool IsNone(const pybind11::object & obj);


/*! \brief Check if a python object has a given attribute
 */
bool HasAttr(const pybind11::object & obj, const std::string & attr);


/*! \brief Check if a python object is callable
 */ 
bool IsCallable(const pybind11::object & obj);


/*! \brief Check if a python object has a given callable attribute
 */
bool HasCallableAttr(const pybind11::object & obj, const std::string & attr);


} // close namespace python
} // close namespace bpmodule

#endif
