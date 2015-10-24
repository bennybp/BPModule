/*! \file
 *
 * \brief Various string formatting functions (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_FORMATSTR_HPP_
#define _GUARD_FORMATSTR_HPP_

#include <vector>

#include "bpmodule/output/Output.hpp"

/*! \brief Create a formatted string
 *
 * \see \ref developer_output_page
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
    bpmodule::output::Output(ss, fmt, Fargs...);
    return ss.str();
}



/*! \brief Joins each element of a vector of strings
 *
 * \param [in] vec A vector of strings to join
 * \param [in] j   What to put in between each string
 * \return A string with each element of \p vec joined with \p j in between
 */
std::string Join(const std::vector<std::string> & vec, const std::string & j);


#endif
