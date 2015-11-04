/*! \file
 *
 * \brief Various string formatting functions (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_FORMATSTR_HPP_
#define _GUARD_FORMATSTR_HPP_

#include "bpmodule/output/Output.hpp"


namespace bpmodule {
namespace util {


/*! \brief Create a formatted string
 *
 * \see \ref developer_util_page
 *
 * \throwno Throws boost exceptions for malformed inputs, etc
 *
 * \tparam Targs The types of the arguments to print
 *
 * \param [in] fmt The format string to use
 * \param [in] Fargs The arguments to the format string
 */
template<typename... Targs>
std::string FormatStr(const std::string & fmt, Targs... Fargs)
{
    // re-use the functionality from the output namespace
    std::stringstream ss;
    bpmodule::output::Output(ss, fmt, Fargs...);
    return ss.str();
}


/*! \brief Use FormatStr from python
 *
 * \param [in] fmt Format string to use
 * \param [in] args Arguments to the format string
 */
std::string FormatStrPy_(const std::string & fmt, const boost::python::list & args);


} // close namespace util
} // close namespace bpmodule


#endif
