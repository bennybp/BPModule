/*! \file
 *
 * \brief Handling and detection of python types (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_TYPES_HPP_
#define _GUARD_TYPES_HPP_

#include <string>

#include "bpmodule/python/Pybind11_fwd.hpp"


namespace bpmodule {
namespace python {


/*! \brief Get the class of a python object as a string
 */
std::string GetPyClass(pybind11::object obj);


/*! \brief Determine if an object is "None"
 */
bool IsNone(pybind11::object obj);


/*! \brief Check if a python object has a given attribute
 */
bool HasAttr(pybind11::object obj, const std::string & attr);


/*! \brief Check if a python object is callable
 */ 
bool IsCallable(pybind11::object obj);


/*! \brief Check if a python object has a given callable attribute
 */
bool HasCallableAttr(pybind11::object obj, const std::string & attr);


} // close namespace python
} // close namespace bpmodule

#endif
