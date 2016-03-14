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




/*! \brief Sets the output to stdout
 *
 * \see \ref developer_output_page
 */
void SetGlobalOut_Stdout(void);


/*! \brief Sets the output to a file
 *
 * \see \ref developer_output_page
 *
 * The directory containing the file must exist. An existing
 * file will be overwritten.
 *
 * \param [in] filepath Path to an output file.
 */
//bool SetOut_File(const std::string & filepath);


/*! \brief Get the current global output stream
 *
 * \see \ref developer_output_page
 */
OutputStream & GetGlobalOut(void);



/*! \brief Check to see if output should be colorized
 */
bool ColorEnabled(void) noexcept;


/*! \brief Enable colorized output
 */
void EnableColor(bool enabled) noexcept;


template<typename... Targs>
void GlobalGeneralOutput(const std::string & fmt, OutputType type, const Targs&... Fargs)
{   
    GetGlobalOut().GeneralOutput(fmt, type, Fargs...);
}


/*! \brief Print general output to a stream
 * 
 * \see \ref developer_output_page
 *
 * \throwno Throws boost exceptions for malformed inputs, etc
 *
 * \tparam Targs The types of the arguments to print
 *
 * \param [in] out The stream to print to
 * \param [in] fmt The format string to use
 * \param [in] Fargs The arguments to the format string
 */
template<typename... Targs>
void GlobalOutput(const std::string & fmt, const Targs&... Fargs)
{   
    GetGlobalOut().Output(fmt, Fargs...);
}



template<typename... Targs> 
void GlobalChanged(const std::string & fmt, const Targs&... Fargs)
{   
    GetGlobalOut().Changed(fmt, Fargs...);
}


template<typename... Targs>
void GlobalError(const std::string & fmt, const Targs&... Fargs)
{   
    GetGlobalOut().Error(fmt, Fargs...);
}


/*! \brief Print warning information to a stream
 * \copydetails Output(std::ostream &, const std::string &, Targs...)
 */
template<typename... Targs> 
void GlobalWarning(const std::string & fmt, const Targs&... Fargs)
{   
    GetGlobalOut().Warning(fmt, Fargs...);
}


/*! \brief Print success information to a stream
 * \copydetails Output(std::ostream &, const std::string &, Targs...)
 */
template<typename... Targs> 
void GlobalSuccess(const std::string & fmt, const Targs&... Fargs)
{   
    GetGlobalOut().Success(fmt, Fargs...);
}


/*! \brief Print debug information to a stream
 * \copydetails Output(std::ostream &, const std::string &, Targs...)
 */
template<typename... Targs>
void GlobalDebug(const std::string & fmt, const Targs&... Fargs)
{   
    GetGlobalOut().Debug(fmt, Fargs...);
}


} // close namespace output
} // close namespace bpmodule


#endif
