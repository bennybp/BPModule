/*! \file
 *
 * \brief Global output printing (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_OUTPUT__GLOBALOUTPUT_HPP_
#define BPMODULE_GUARD_OUTPUT__GLOBALOUTPUT_HPP_


#include "bpmodule/output/OutputStream.hpp"


namespace bpmodule {
namespace output {




/*! \brief Sets the global output to stdout
 */
void SetGlobalOut_Stdout(void);


//bool SetOut_File(const std::string & filepath);



/*! \brief Get the current global output stream
 */
OutputStream & GetGlobalOut(void);



/*! \brief Check to see if output should be colorized
 *
 * \return True if output can be colorized
 */
bool ColorEnabled(void) noexcept;



/*! \brief Enable colorized output
 *
 * \param [in] enabled True to enable color output, False to disable
 */
void EnableColor(bool enabled) noexcept;



/*! \brief Print formatted text to the global output stream
 *
 * \throw bpmodule::exception::GeneralException with malformed formats,
 *        improper number of arguments, etc.
 *
 * \tparam Targs The types of the arguments to print
 *
 * \param [in] fmt The format string to use
 * \param [in] type The type of output (debug, warning, etc)
 * \param [in] Fargs The arguments to the format string
 */
template<typename... Targs>
void GlobalGeneralOutput(const std::string & fmt, OutputType type, const Targs&... Fargs)
{   
    GetGlobalOut().GeneralOutput(fmt, type, Fargs...);
}


/*! \brief Print formatted general output to the global output stream
 * 
 * \throw bpmodule::exception::GeneralException with malformed formats,
 *        improper number of arguments, etc.
 *
 * \tparam Targs The types of the arguments to print
 *
 * \param [in] fmt The format string to use
 * \param [in] Fargs The arguments to the format string
 */
template<typename... Targs>
void GlobalOutput(const std::string & fmt, const Targs&... Fargs)
{   
    GetGlobalOut().Output(fmt, Fargs...);
}



/*! \brief Print formatted changed output to the global output stream
 * \copydetails GlobalOutput(const std::string & fmt, const Targs&... Fargs)
 */
template<typename... Targs> 
void GlobalChanged(const std::string & fmt, const Targs&... Fargs)
{   
    GetGlobalOut().Changed(fmt, Fargs...);
}


/*! \brief Print formatted error output to the global output stream
 * \copydetails GlobalOutput(const std::string & fmt, const Targs&... Fargs)
 */
template<typename... Targs>
void GlobalError(const std::string & fmt, const Targs&... Fargs)
{   
    GetGlobalOut().Error(fmt, Fargs...);
}


/*! \brief Print formatted warning output to the global output stream
 * \copydetails GlobalOutput(const std::string & fmt, const Targs&... Fargs)
 */
template<typename... Targs> 
void GlobalWarning(const std::string & fmt, const Targs&... Fargs)
{   
    GetGlobalOut().Warning(fmt, Fargs...);
}


/*! \brief Print formatted success output to the global output stream
 * \copydetails GlobalOutput(const std::string & fmt, const Targs&... Fargs)
 */
template<typename... Targs> 
void GlobalSuccess(const std::string & fmt, const Targs&... Fargs)
{   
    GetGlobalOut().Success(fmt, Fargs...);
}


/*! \brief Print formatted debug output to the global output stream
 * \copydetails GlobalOutput(const std::string & fmt, const Targs&... Fargs)
 */
template<typename... Targs>
void GlobalDebug(const std::string & fmt, const Targs&... Fargs)
{   
    GetGlobalOut().Debug(fmt, Fargs...);
}


} // close namespace output
} // close namespace bpmodule


#endif
