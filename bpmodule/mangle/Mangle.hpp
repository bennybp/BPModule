/*! \file
 *
 * \brief Mangling/Demangling helpers (header) 
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef _GUARD_MANGLE_HPP_
#define _GUARD_MANGLE_HPP_

#include <string>
#include <typeinfo>


namespace bpmodule {
namespace mangle {


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
 */
template<typename T>
std::string DemangleCppType(void)
{
    return DemangleCpp(typeid(T).name());
}


} // close namespace mangle
} // close namespace bpmodule

#endif
