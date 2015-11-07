/*! \file
 *
 * \brief Various string formatting functions (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_FORMATSTRING_HPP_
#define _GUARD_FORMATSTRING_HPP_

#include <iostream>
#include <sstream>
#include <string>

#include <boost/format.hpp>
#include "bpmodule/python_helper/BoostPython_fwd.hpp"


namespace bpmodule {
namespace util {



namespace detail {


/*! \brief Output a boost::format object to a stream
 *
 * \throwno Throws boost exceptions for malformed inputs, etc
 *
 * \param [in] os The stream to output to
 * \param [in] bfmt The format string to output
 */
inline void FormatStream(std::ostream & os, const boost::format & bfmt)
{
    os << bfmt;
}



/*! \brief Output a boost::format object to a stream
 *
 * \throwno Throws boost exceptions for malformed inputs, etc
 *
 * \tparam Targs The types of the arguments to print
 *
 * \param [in] os The stream to output to
 * \param [in] bfmt The format string to use
 * \param [in] targ A single argument to the format string
 * \param [in] Fargs The arguments to the format string
 */
template<typename T, typename... Targs>
void FormatStream(std::ostream & os, boost::format & bfmt,
                  const T & targ, const Targs&... Fargs)
{
    bfmt % targ;
    FormatStream(os, bfmt, Fargs...);
}


} // close namespace detail






/*! \brief Output a formatted string to a stream
 *
 * \throwno Throws boost exceptions for malformed inputs, etc
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
    boost::format bfmt(fmt);
    detail::FormatStream(os, bfmt, Fargs...);
}



/*! \brief Output a formatted string to a stream
 *
 * Overload for format string with no arguments
 *
 * \throwno Throws boost exceptions for malformed inputs, etc
 *
 * \param [in] os The stream to output to
 * \param [in] fmt The format string to use
 */
inline void FormatStream(std::ostream & os, const std::string & fmt)
{
    os << fmt;
}





/*! \brief Create a formatted string
 *
 * \throwno Throws boost exceptions for malformed inputs, etc
 *
 * \tparam Targs The types of the arguments to print
 *
 * \param [in] fmt The format string to use
 * \param [in] Fargs The arguments to the format string
 */
template<typename... Targs>
std::string FormatString(const std::string & fmt, const Targs&... Fargs)
{
    std::stringstream ss;
    FormatStream(ss, fmt, Fargs...);
    return ss.str();
}





/*! \brief Create a formatted string
 *
 * Overload for format string with no arguments
 *
 * \throwno Throws boost exceptions for malformed inputs, etc
 *
 * \param [in] fmt The format string to use
 */
inline std::string FormatString(const std::string & fmt)
{
    return fmt;
}










namespace export_python {

/*! \brief Create a format string from python
 *
 * \param [in] fmt Format string to use
 * \param [in] args Arguments to the format string
 */
std::string FormatStringPy(const std::string & fmt, const boost::python::list & args);

} // close namespace export_python



} // close namespace util
} // close namespace bpmodule


#endif
