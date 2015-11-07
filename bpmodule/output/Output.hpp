/*! \file
 *
 * \brief Output and printing functionality (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_OUTPUT_HPP_
#define _GUARD_OUTPUT_HPP_

#include <iostream>

#include "bpmodule/util/FormatString.hpp"
#include "bpmodule/python_helper/BoostPython_fwd.hpp"


//! \todo Handle exceptions better? Yes, catch Boost exceptions, print error, and then continue? Update doc and don't forget about FormatStr

namespace bpmodule {
namespace output {








namespace detail {

/*! The type of information being output
 *
 * \warning Internal use only
 */
enum class OutputType
{
    Output,  //!< General output
    Success, //!< Something has succeeded
    Changed, //!< Something has been changed (ie from defaults)
    Warning, //!< Something is kinda wrong
    Error,   //!< Something is very wrong
    Debug    //!< For developers
};


/*! \brief Output to a stream
 *
 * \throwno Throws boost exceptions for malformed inputs, etc
 * \warning Internal use only
 *
 * \param [in] out The stream to print to
 * \param [in] type The type of output (output, debug, etc)
 * \param [in] str A string to output
 */
void Output_(std::ostream & out, OutputType type, const std::string & str);

} // close namespace detail









/*! \brief Returns true if the output system is valid 
 */
bool Valid(void);


/*! \brief Sets the output to stdout
 *
 * \see \ref developer_output_page
 */
void SetOut_Stdout(void);


/*! \brief Sets the output to stderr
 *
 * \see \ref developer_output_page
 */
void SetOut_Stderr(void);


/*! \brief Sets the output to a file
 *
 * \see \ref developer_output_page
 *
 * The directory containing the file must exist. An existing
 * file will be overwritten.
 *
 * \param [in] filepath Path to an output file.
 */
bool SetOut_File(const std::string & filepath);


/*! \brief Enables or disables the use of colorized output
 *
 * \see \ref developer_output_page
 *
 * \exnothrow
 *
 * \param [in] usecolor True to turn on colorized output, false to disable.
 */
void SetColor(bool usecolor) noexcept;


/*! \brief Enables or disables debugging output
 *
 * \see \ref developer_output_page
 *
 * \exnothrow
 *
 * \param [in] debug True to turn on debugging, false to disable.
 */
void SetDebug(bool debug) noexcept;


/*! \brief Flush the output buffers
 *
 * \see \ref developer_output_page
 */
void Flush(void);


/*! \brief Get the current output stream
 *
 * \return Reference to the current output stream
 */
std::ostream & GetOut(void);







/////////////////////
// Output
/////////////////////

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
void Output(std::ostream & out, const std::string & fmt, const Targs&... Fargs)
{
    detail::Output_(out, detail::OutputType::Output, util::FormatString(fmt, Fargs...));
}



/*! \brief Print general output to screen or file
 * 
 * The output can be set with SetOut_Stdout, SetOut_Stderr, or
 * SetOut_File.
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
void Output(const std::string & fmt, const Targs&... Fargs)
{
    Output(GetOut(), fmt, Fargs...);
}




/////////////////////
// Changed
/////////////////////

/*! \brief Print changed information to a stream
 * \copydetails Output(std::ostream &, const std::string &, Targs...)
 */
template<typename... Targs>
void Changed(std::ostream & out, const std::string & fmt, const Targs&... Fargs)
{
    detail::Output_(out, detail::OutputType::Changed, util::FormatString(fmt, Fargs...));
}


/*! \brief Print changed information to screen or file
 * \copydetails Output(const std::string &, Targs...)
 */
template<typename... Targs>
void Changed(const std::string & fmt, const Targs&... Fargs)
{
    Changed(GetOut(), fmt, Fargs...);
}



/////////////////////
// Error
/////////////////////

/*! \brief Print error information to a stream
 * \copydetails Output(std::ostream &, const std::string &, Targs...)
 */
template<typename... Targs>
void Error(std::ostream & out, const std::string & fmt, const Targs&... Fargs)
{
    detail::Output_(out, detail::OutputType::Error, util::FormatString(fmt, Fargs...));
}


/*! \brief Print error information to screen or file
 * \copydetails Output(const std::string &, Targs...)
 */
template<typename... Targs>
void Error(const std::string & fmt, const Targs&... Fargs)
{
    Error(GetOut(), fmt, Fargs...);
}


/////////////////////
// Warning
/////////////////////

/*! \brief Print warning information to a stream
 * \copydetails Output(std::ostream &, const std::string &, Targs...)
 */
template<typename... Targs>
void Warning(std::ostream & out, const std::string & fmt, const Targs&... Fargs)
{
    detail::Output_(out, detail::OutputType::Warning, util::FormatString(fmt, Fargs...));
}


/*! \brief Print warning information to screen or file
 * \copydetails Output(const std::string &, Targs...)
 */
template<typename... Targs>
void Warning(const std::string & fmt, const Targs&... Fargs)
{
    Warning(GetOut(), fmt, Fargs...);
}



/////////////////////
// Success
/////////////////////

/*! \brief Print success information to a stream
 * \copydetails Output(std::ostream &, const std::string &, Targs...)
 */
template<typename... Targs>
void Success(std::ostream & out, const std::string & fmt, const Targs&... Fargs)
{
    detail::Output_(out, detail::OutputType::Success, util::FormatString(fmt, Fargs...));
}


/*! \brief Print success information to screen or file
 * \copydetails Output(const std::string &, Targs...)
 */
template<typename... Targs>
void Success(const std::string & fmt, const Targs&... Fargs)
{
    Success(GetOut(), fmt, Fargs...);
}




/////////////////////
// Debug
/////////////////////

/*! \brief Print debug information to a stream
 * \copydetails Output(std::ostream &, const std::string &, Targs...)
 */
template<typename... Targs>
void Debug(std::ostream & out, const std::string & fmt, const Targs&... Fargs)
{
    detail::Output_(out, detail::OutputType::Debug, util::FormatString(fmt, Fargs...));
}


/*! \brief Print debug information to screen or file
 * \copydetails Output(const std::string &, Targs...)
 */
template<typename... Targs>
void Debug(const std::string & fmt, const Targs&... Fargs)
{
    Debug(GetOut(), fmt, Fargs...);
}






namespace export_python {

/*! \brief Wrap printing functions for use from python
 *
 * This function takes a boost::python list rather than the parameter pack
 *
 * \param [in] os Output stream to send the output to
 * \param [in] type The type of output
 * \param [in] fmt Format string to use
 * \param [in] args Arguments to the format string
 */
void OutputPy_(std::ostream & os, detail::OutputType type,
               const std::string & fmt, const boost::python::list & args);

} // close namespace export_python





} // close namespace output
} // close namespace bpmodule


#endif
