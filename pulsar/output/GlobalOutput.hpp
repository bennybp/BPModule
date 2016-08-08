/*! \file
 *
 * \brief Global output printing (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_OUTPUT__GLOBALOUTPUT_HPP_
#define PULSAR_GUARD_OUTPUT__GLOBALOUTPUT_HPP_


#include "pulsar/output/OutputStream.hpp"


namespace pulsar{
namespace output {




/*! \brief Sets the global output to stdout
 */
void create_global_output(const std::string & path,
                          const std::string & base,
                          bool use_stdout);


//bool SetOut_File(const std::string & filepath);



/*! \brief Get the current global output stream
 */
OutputStream & get_global_output(void);



/*! \brief Check to see if output should be colorized
 *
 * \return True if output can be colorized
 */
bool color_enabled(void) noexcept;



/*! \brief Enable colorized output
 *
 * \param [in] enabled True to enable color output, False to disable
 */
void enable_color(bool enabled) noexcept;



/*! \brief Print formatted text to the global output stream
 *
 * \throw pulsar::exception::GeneralException with malformed formats,
 *        improper number of arguments, etc.
 *
 * \tparam Targs The types of the arguments to print
 *
 * \param [in] fmt The format string to use
 * \param [in] type The type of output (debug, warning, etc)
 * \param [in] Fargs The arguments to the format string
 */
template<typename... Targs>
void print_global_general_output(const std::string & fmt, OutputType type, const Targs&... Fargs)
{   
    get_global_output().general_output(fmt, type, Fargs...);
}


/*! \brief Print formatted general output to the global output stream
 * 
 * \throw pulsar::exception::GeneralException with malformed formats,
 *        improper number of arguments, etc.
 *
 * \tparam Targs The types of the arguments to print
 *
 * \param [in] fmt The format string to use
 * \param [in] Fargs The arguments to the format string
 */
template<typename... Targs>
void print_global_output(const std::string & fmt, const Targs&... Fargs)
{   
    get_global_output().output(fmt, Fargs...);
}



/*! \brief Print formatted changed output to the global output stream
 * \copydetails global_output(const std::string & fmt, const Targs&... Fargs)
 */
template<typename... Targs> 
void print_global_changed(const std::string & fmt, const Targs&... Fargs)
{   
    get_global_output().changed(fmt, Fargs...);
}


/*! \brief Print formatted error output to the global output stream
 * \copydetails global_output(const std::string & fmt, const Targs&... Fargs)
 */
template<typename... Targs>
void print_global_error(const std::string & fmt, const Targs&... Fargs)
{   
    get_global_output().error(fmt, Fargs...);
}


/*! \brief Print formatted warning output to the global output stream
 * \copydetails global_output(const std::string & fmt, const Targs&... Fargs)
 */
template<typename... Targs> 
void print_global_warning(const std::string & fmt, const Targs&... Fargs)
{   
    get_global_output().warning(fmt, Fargs...);
}


/*! \brief Print formatted success output to the global output stream
 * \copydetails global_output(const std::string & fmt, const Targs&... Fargs)
 */
template<typename... Targs> 
void print_global_success(const std::string & fmt, const Targs&... Fargs)
{   
    get_global_output().success(fmt, Fargs...);
}


/*! \brief Print formatted debug output to the global output stream
 * \copydetails global_output(const std::string & fmt, const Targs&... Fargs)
 */
template<typename... Targs>
void print_global_debug(const std::string & fmt, const Targs&... Fargs)
{   
    get_global_output().debug(fmt, Fargs...);
}


} // close namespace output
} // close namespace pulsar


#endif
