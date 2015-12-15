/*! \file
 *
 * \brief Mangling/Demangling helpers (header) 
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef _GUARD_MANGLE_HPP_
#define _GUARD_MANGLE_HPP_

#include <string>
#include <typeinfo>

#include "bpmodule/python_helper/Types.hpp"

namespace bpmodule {
namespace util {


/*! \brief Demangle a type string for a C++ object into something human-readable
 *
 * ie, converts  3barI5emptyLi17EE to bar<empty, 17>
 */
std::string DemangleCpp(const char * typestr);


/*! \brief Demangle the type of an object
 *
 * \param [in] t The object whose type to demangle
 */
template<typename T>
std::string DemangleCppType(const T & t)
{
    return DemangleCpp(typeid(t).name());
}


/*! \brief Demangle the type of an object
 *
 * \tparam The type to demangle
 *
 * For a c++ type, same as DemangleCppType.
 * For a python object, will return the class name
 */
template<typename T>
typename std::enable_if<!std::is_base_of<pybind11::object, T>::value, std::string>::type
DemangleCppOrPyType(const T & t)
{
    return DemangleCpp(typeid(t).name());
}



template<typename T>
typename std::enable_if<std::is_base_of<pybind11::object, T>::value, std::string>::type
DemangleCppOrPyType(const T & t)
{
    return python_helper::GetPyClass(t);
}





/*! \brief Demangle the type of an object
 *
 * \tparam The type to demangle
 */
template<typename T>
std::string DemangleCppType(void)
{
    return DemangleCpp(typeid(T).name());
}






} // close namespace util
} // close namespace bpmodule

#endif
