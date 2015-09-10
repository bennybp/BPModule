/*! \file
 *
 * \brief Output and printing functionality (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_OUTPUT_HPP_
#define _GUARD_OUTPUT_HPP_

#include <iostream>
#include <sstream>

#include <boost/format.hpp>

#include "bpmodule/python_helper/BoostPython_fwd.hpp"


//! \todo Handle exceptions better?

namespace bpmodule {
namespace output {

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



/*! \brief Returns true if the output system is valid 
 */
bool Valid(void);


/*! \brief Sets the output to stdout
 *
 * \see \ref developer_output_sec
 */
void SetOut_Stdout(void);


/*! \brief Sets the output to stderr
 *
 * \see \ref developer_output_sec
 */
void SetOut_Stderr(void);


/*! \brief Sets the output to a file
 *
 * \see \ref developer_output_sec
 *
 * The directory containing the file must exist. An existing
 * file will be overwritten.
 *
 * \param [in] filepath Path to an output file.
 */
bool SetOut_File(const std::string & filepath);


/*! \brief Enables or disables the use of colorized output
 *
 * \see \ref developer_output_sec
 *
 * \exnothrow
 *
 * \param [in] usecolor True to turn on colorized output, false to disable.
 */
void SetColor(bool usecolor) noexcept;


/*! \brief Enables or disables debugging output
 *
 * \see \ref developer_output_sec
 *
 * \exnothrow
 *
 * \param [in] debug True to turn on debugging, false to disable.
 */
void SetDebug(bool debug) noexcept;


/*! \brief Flush the output buffers
 *
 * \see \ref developer_output_sec
 */
void Flush(void);


/*! \brief Get the current output stream
 *
 * \return Reference to the current output stream
 */
std::ostream & GetOut(void);



/*! \brief Create a line of characters
 *
 * Repeats a character a number of times. A newline is included.
 *
 * \param [in] c The character to use
 * \param [in] n The number of times to repeat the character
 */
std::string Line(char c, int n = 80);



/*! \brief Output to a stream
 *
 * \throwno Throws boost exceptions for malformed inputs, etc
 * \warning Internal use only
 *
 * \param [in] out The stream to print to
 * \param [in] type The type of output (output, debug, etc)
 * \param [in] bfmt A boost::format string
 */
void Output_(std::ostream & out, OutputType type, const boost::format & bfmt);





/*! \brief Builds up a boost::format string
 *
 * \throwno Throws boost exceptions for malformed inputs, etc
 * \warning Internal use only
 *
 * \tparam T The type of object to print
 * \tparam Targs The types of the rest of the arguments to print
 *
 * \param [in] out The stream to print to
 * \param [in] type The type of output (output, debug, etc)
 * \param [inout] bfmt The boost::format string built up so far
 * \param [in] targ Something to print
 * \param [in] Fargs The rest of the stuff to print
 */
template<typename T, typename... Targs>
void Output_(std::ostream & out, OutputType type, boost::format & bfmt, T targ, Targs... Fargs)
{
    bfmt % targ;
    Output_(out, type, bfmt, Fargs...);
}


/*! \brief Start building up a boost::format string
 *
 * \throwno Throws boost exceptions for malformed inputs, etc
 * \warning Internal use only
 *
 * \tparam Targs The types of the arguments to print
 *
 * \param [in] out The stream to print to
 * \param [in] type The type of output (output, debug, etc)
 * \param [in] fmt The format string to use
 * \param [in] Fargs The stuff to print
 */
template<typename... Targs>
void Output_(std::ostream & out, OutputType type, const std::string & fmt, Targs... Fargs)
{
    boost::format bfmt(fmt);
    Output_(out, type, bfmt, Fargs...);
}



/////////////////////
// Output
/////////////////////

/*! \brief Print general output to a stream
 * 
 * \see \ref developer_output_sec
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
void Output(std::ostream & out, const std::string & fmt, Targs... Fargs)
{
    Output_(out, OutputType::Output, fmt, Fargs...);
}



/*! \brief Print general output to screen or file
 * 
 * The output can be set with SetOut_Stdout, SetOut_Stderr, or
 * SetOut_File.
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
void Output(const std::string & fmt, Targs... Fargs)
{
    Output(GetOut(), fmt, Fargs...);
}



/*! \brief Print general output to a string
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
std::string OutputStr(const std::string & fmt, Targs... Fargs)
{
    std::stringstream ss;
    Output(ss, fmt, Fargs...);
    return ss.str();
}



/////////////////////
// Changed
/////////////////////

/*! \brief Print changed information to a stream
 * \copydetails Output(std::ostream &, const std::string &, Targs...)
 */
template<typename... Targs>
void Changed(std::ostream & out, const std::string & fmt, Targs... Fargs)
{
    Output_(out, OutputType::Changed, fmt, Fargs...);
}


/*! \brief Print changed information to screen or file
 * \copydetails Output(const std::string &, Targs...)
 */
template<typename... Targs>
void Changed(const std::string & fmt, Targs... Fargs)
{
    Changed(GetOut(), fmt, Fargs...);
}


/*! \brief Print changed information to a string
 * \copydetails OutputStr(const std::string &, Targs...)
 */
template<typename... Targs>
std::string ChangedStr(const std::string & fmt, Targs... Fargs)
{
    std::stringstream ss;
    Changed(ss, fmt, Fargs...);
    return ss.str();
}



/////////////////////
// Error
/////////////////////

/*! \brief Print error information to a stream
 * \copydetails Output(std::ostream &, const std::string &, Targs...)
 */
template<typename... Targs>
void Error(std::ostream & out, const std::string & fmt, Targs... Fargs)
{
    Output_(out, OutputType::Error, fmt, Fargs...);
}


/*! \brief Print error information to screen or file
 * \copydetails Output(const std::string &, Targs...)
 */
template<typename... Targs>
void Error(const std::string & fmt, Targs... Fargs)
{
    Error(GetOut(), fmt, Fargs...);
}


/*! \brief Print error information to a string
 * \copydetails OutputStr(const std::string &, Targs...)
 */
template<typename... Targs>
std::string ErrorStr(const std::string & fmt, Targs... Fargs)
{
    std::stringstream ss;
    Error(ss, fmt, Fargs...);
    return ss.str();
}



/////////////////////
// Warning
/////////////////////

/*! \brief Print warning information to a stream
 * \copydetails Output(std::ostream &, const std::string &, Targs...)
 */
template<typename... Targs>
void Warning(std::ostream & out, const std::string & fmt, Targs... Fargs)
{
    Output_(out, OutputType::Warning, fmt, Fargs...);
}


/*! \brief Print warning information to screen or file
 * \copydetails Output(const std::string &, Targs...)
 */
template<typename... Targs>
void Warning(const std::string & fmt, Targs... Fargs)
{
    Warning(GetOut(), fmt, Fargs...);
}


/*! \brief Print warning information to a string
 * \copydetails OutputStr(const std::string &, Targs...)
 */
template<typename... Targs>
std::string WarningStr(const std::string & fmt, Targs... Fargs)
{
    std::stringstream ss;
    Warning(ss, fmt, Fargs...);
    return ss.str();
}



/////////////////////
// Success
/////////////////////

/*! \brief Print success information to a stream
 * \copydetails Output(std::ostream &, const std::string &, Targs...)
 */
template<typename... Targs>
void Success(std::ostream & out, const std::string & fmt, Targs... Fargs)
{
    Output_(out, OutputType::Success, fmt, Fargs...);
}


/*! \brief Print success information to screen or file
 * \copydetails Output(const std::string &, Targs...)
 */
template<typename... Targs>
void Success(const std::string & fmt, Targs... Fargs)
{
    Success(GetOut(), fmt, Fargs...);
}


/*! \brief Print success information to a string
 * \copydetails OutputStr(const std::string &, Targs...)
 */
template<typename... Targs>
std::string SuccessStr(const std::string & fmt, Targs... Fargs)
{
    std::stringstream ss;
    Success(ss, fmt, Fargs...);
    return ss.str();
}



/////////////////////
// Debug
/////////////////////

/*! \brief Print debug information to a stream
 * \copydetails Output(std::ostream &, const std::string &, Targs...)
 */
template<typename... Targs>
void Debug(std::ostream & out, const std::string & fmt, Targs... Fargs)
{
    Output_(out, OutputType::Debug, fmt, Fargs...);
}


/*! \brief Print debug information to screen or file
 * \copydetails Output(const std::string &, Targs...)
 */
template<typename... Targs>
void Debug(const std::string & fmt, Targs... Fargs)
{
    Debug(GetOut(), fmt, Fargs...);
}


/*! \brief Print debug information to a string
 * \copydetails OutputStr(const std::string &, Targs...)
 */
template<typename... Targs>
std::string DebugStr(const std::string & fmt, Targs... Fargs)
{
    std::stringstream ss;
    Debug(ss, fmt, Fargs...);
    return ss.str();
}


} // close namespace output
} // close namespace bpmodule


#endif
