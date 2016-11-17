/*! \file
 *
 * \brief Mangling/Demangling helpers (header) 
 */

#pragma once

#include <typeinfo>
#include <string>

namespace pulsar{

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
 */
template<typename T>
std::string demangle_cpp_type(void)
{
    return demangle_cpp(typeid(T).name());
}

} // close namespace pulsar

