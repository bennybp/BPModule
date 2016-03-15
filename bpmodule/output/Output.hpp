/*! \file
 *
 * \brief Basic output and printing functionality (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#ifndef BPMODULE_GUARD_OUTPUT__OUTPUT_HPP_
#define BPMODULE_GUARD_OUTPUT__OUTPUT_HPP_

#include "bpmodule/output/OutputType.hpp"
#include "bpmodule/util/Format.hpp"


namespace bpmodule {
namespace output {


namespace detail {
    /*! \brief Print a string to a stream
     * 
     * Wraps the printing to provide colorized output, etc
     *
     * \param [in] os The stream to print to
     * \param [in] type The type of output (debug, warning, etc)
     * \param [in] str The string to print
     */
    void Output_(std::ostream & os, OutputType type, const std::string & str);
}


/*! \brief Print formatted text to a stream
 *
 * \throw bpmodule::exception::GeneralException with malformed formats,
 *        improper number of arguments, etc.
 *
 * \tparam Targs The types of the arguments to print
 *
 * \param [in] os The stream to print to
 * \param [in] fmt The format string to use
 * \param [in] type The type of output (debug, warning, etc)
 * \param [in] Fargs The arguments to the format string
 */
template<typename ... Targs>
void GeneralOutput(std::ostream & os, OutputType type, const std::string & fmt, const Targs&... Fargs)
{
    detail::Output_(os, type, util::FormatString(fmt, Fargs...));
}


/*! \brief Print formatted general output to a stream
 * 
 * \throw bpmodule::exception::GeneralException with malformed formats,
 *        improper number of arguments, etc.
 *
 * \tparam Targs The types of the arguments to print
 *
 * \param [in] os The stream to print to
 * \param [in] fmt The format string to use
 * \param [in] Fargs The arguments to the format string
 */
template<typename ... Targs>
void Output(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
{
    detail::Output_(os, OutputType::Output, util::FormatString(fmt, Fargs...));
}


/*! \brief Print formatted changed output to a stream
 * \copydetails Output(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
 */
template<typename ... Targs>
void Changed(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
{
    detail::Output_(os, OutputType::Changed, util::FormatString(fmt, Fargs...));
}


/*! \brief Print formatted error output to a stream
 * \copydetails Output(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
 */
template<typename ... Targs>
void Error(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
{
    detail::Output_(os, OutputType::Error, util::FormatString(fmt, Fargs...));
}


/*! \brief Print formatted warning output to a stream
 * \copydetails Output(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
 */
template<typename ... Targs>
void Warning(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
{
    detail::Output_(os, OutputType::Warning, util::FormatString(fmt, Fargs...));
}


/*! \brief Print formatted success output to a stream
 * \copydetails Output(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
 */
template<typename ... Targs>
void Success(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
{
    detail::Output_(os, OutputType::Success, util::FormatString(fmt, Fargs...));
}


/*! \brief Print formatted debug output to a stream
 * \copydetails Output(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
 */
template<typename ... Targs>
void Debug(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
{
    detail::Output_(os, OutputType::Debug, util::FormatString(fmt, Fargs...));
}



} // close namespace output
} // close namespace bpmodule

#endif
