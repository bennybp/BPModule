/*! \file
 *
 * \brief Various string formatting functions (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_UTIL__FORMAT_HPP_
#define BPMODULE_GUARD_UTIL__FORMAT_HPP_

#include "bpmodule/util/vprintfcpp/Format.hpp"
#include "bpmodule/exception/Exceptions.hpp"


namespace bpmodule {
namespace util {



/*! \brief Output a formatted string to a stream
 *
 * \throw bpmodule::exception::GeneralException for malformed inputs, etc
 *
 * \tparam Targs The types of the arguments to print
 *
 * \param [in] os The stream to output to
 * \param [in] fmt The format string to use
 * \param [in] Fargs The arguments to the format string
 */
template<typename... Targs>
void FormatStream(std::ostream & os, const std::string & fmt,
                  const Targs&... Fargs)
{
    try {
      vprintfcpp::FormatStream(os, fmt, Fargs...);
    }
    catch(std::exception & ex)
    {
        throw exception::GeneralException("Error in formatting a string or stream",
                                          "error", ex.what(),
                                          "fmt", fmt, "nargs", sizeof...(Fargs));
    }
}



/*! \brief Create a formatted string
 *
 * \throw bpmodule::exception::GeneralException for malformed inputs, etc
 *
 * \tparam Targs The types of the arguments to print
 *
 * \param [in] fmt The format string to use
 * \param [in] Fargs The arguments to the format string
 * \return a Formatted string
 */
template<typename... Targs>
std::string FormatString(const std::string & fmt, const Targs&... Fargs)
{
    std::stringstream ss;
    FormatStream(ss, fmt, Fargs...);
    return ss.str();
}


} // close namespace util
} // close namespace bpmodule


#endif
