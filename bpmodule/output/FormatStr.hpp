/*! \file
 *
 * \brief Formatting a string with boost::format
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_FORMATSTR_HPP_
#define _GUARD_FORMATSTR_HPP_

#include "bpmodule/output/Output.hpp"

/*! \brief Create a formatted string
 *
 * \see \ref developer_output_sec
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
    std::stringstream ss;
    Output(ss, fmt, Fargs...);
    return ss.str();
}

#endif
