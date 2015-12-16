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


//! Types defined in python that are available in BPModule
enum class PythonType
{
    Bool,
    Int,
    Float,
    String,
    ListBool,
    ListInt,
    ListFloat,
    ListString,
    ListEmpty,
    ListHetero,
    ListUnknown,
    Tuple,
    Dict,
    None,
    Unknown
};




/*! \brief Convert a string to a PythonType
 */
PythonType StrToPythonType(const std::string & str);


/*! \brief Convert a PythonType to a string
 */
const char * PythonTypeToStr(PythonType pytype);


/*! \brief Determine the type of a python object
 */
PythonType DeterminePyType(pybind11::object obj);


/*! \brief Get the class of a python object as a string
 */
std::string GetPyClass(pybind11::object obj);


/*! \brief Check if a python object has a given attribute
 */
bool HasAttr(pybind11::object obj, const std::string & attr);


/*! \brief Check if a python object is callable
 */ 
bool IsCallable(pybind11::object obj);


/*! \brief Check if a python object has a given callable attribute
 */
bool HasCallableAttr(pybind11::object obj, const std::string & attr);


/*! \brief Compares two python objects
 *
 * \todo - pybind11 might implement this someday into the object class
 */
bool Equal(pybind11::object lhs, pybind11::object rhs);


/*! \brief Deep copy a python object
 */
pybind11::object DeepCopy(const pybind11::object & rhs);


} // close namespace python
} // close namespace bpmodule

#endif
