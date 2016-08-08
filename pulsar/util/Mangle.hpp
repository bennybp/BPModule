/*! \file
 *
 * \brief Mangling/Demangling helpers (header) 
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_UTIL__MANGLE_HPP_
#define PULSAR_GUARD_UTIL__MANGLE_HPP_

#include <typeinfo>

#include "pulsar/python/Types.hpp"

namespace pulsar{
namespace util {


/*! \brief Demangle a type string for a C++ object into something human-readable
 *
 * ie, converts  3barI5emptyLi17EE to bar<empty, 17>
 */
std::string demangle_cpp(const char * typestr);


/*! \brief Demangle a type string for a C++ object into something human-readable
 *
 * ie, converts  3barI5emptyLi17EE to bar<empty, 17>
 */
std::string demangle_cpp(const std::string & typestr);


/*! \brief Demangle the type of an object
 *
 * \param [in] t The object whose type to demangle
 */
template<typename T>
std::string demangle_cpp_type(const T & t)
{
    return demangle_cpp(typeid(t).name());
}


/*! \brief Demangle the type of an object
 *
 * \tparam The type to demangle
 *
 * For a c++ type, same as demangle_cpp_type.
 * For a python object, will return the class name
 */
template<typename T>
typename std::enable_if<!std::is_base_of<pybind11::object, T>::value, std::string>::type
demangle_cpp_or_py_type(const T & t)
{
    return demangle_cpp(typeid(t).name());
}



template<typename T>
typename std::enable_if<std::is_base_of<pybind11::object, T>::value, std::string>::type
demangle_cpp_or_py_type(const T & t)
{
    return python::get_py_class(t);
}


/*! \brief Demangle the type of an object
 *
 * \tparam The type to demangle
 */
template<typename T>
std::string demangle_cpp_type(void)
{
    return demangle_cpp(typeid(T).name());
}






} // close namespace util
} // close namespace pulsar

#endif
