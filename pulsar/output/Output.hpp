/*! \file
 *
 * \brief Basic output and printing functionality (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#ifndef PULSAR_GUARD_OUTPUT__OUTPUT_HPP_
#define PULSAR_GUARD_OUTPUT__OUTPUT_HPP_

#include "pulsar/output/OutputType.hpp"
#include "pulsar/util/Format.hpp"


namespace pulsar{
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
    void print_output_(std::ostream & os, OutputType type, const std::string & str);
}


/*! \brief Print formatted text to a stream
 *
 * \throw pulsar::exception::GeneralException with malformed formats,
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
void print_general_output(std::ostream & os, OutputType type, const std::string & fmt, const Targs&... Fargs)
{
    detail::print_output_(os, type, util::format_string(fmt, Fargs...));
}


/*! \brief Print formatted general output to a stream
 * 
 * \throw pulsar::exception::GeneralException with malformed formats,
 *        improper number of arguments, etc.
 *
 * \tparam Targs The types of the arguments to print
 *
 * \param [in] os The stream to print to
 * \param [in] fmt The format string to use
 * \param [in] Fargs The arguments to the format string
 */
template<typename ... Targs>
void print_output(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
{
    detail::print_output_(os, OutputType::Output, util::format_string(fmt, Fargs...));
}


/*! \brief Print formatted changed output to a stream
 * \copydetails print_output(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
 */
template<typename ... Targs>
void print_changed(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
{
    detail::print_output_(os, OutputType::Changed, util::format_string(fmt, Fargs...));
}


/*! \brief Print formatted error output to a stream
 * \copydetails print_output(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
 */
template<typename ... Targs>
void print_error(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
{
    detail::print_output_(os, OutputType::Error, util::format_string(fmt, Fargs...));
}


/*! \brief Print formatted warning output to a stream
 * \copydetails print_output(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
 */
template<typename ... Targs>
void print_warning(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
{
    detail::print_output_(os, OutputType::Warning, util::format_string(fmt, Fargs...));
}


/*! \brief Print formatted success output to a stream
 * \copydetails print_output(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
 */
template<typename ... Targs>
void print_success(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
{
    detail::print_output_(os, OutputType::Success, util::format_string(fmt, Fargs...));
}


/*! \brief Print formatted debug output to a stream
 * \copydetails print_output(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
 */
template<typename ... Targs>
void print_debug(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
{
    detail::print_output_(os, OutputType::Debug, util::format_string(fmt, Fargs...));
}



} // close namespace output
} // close namespace pulsar

#endif
