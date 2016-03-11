/*! \file
 *
 * \brief Output and printing functionality (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_OUTPUT__OUTPUT_HPP_
#define BPMODULE_GUARD_OUTPUT__OUTPUT_HPP_

#include <iostream>

#include "bpmodule/util/FormatString.hpp"



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




/*! \brief Stream buffer that tees output to a string
 *
 * Output will go to the streambuf, as well as the given string
 */ 
class TeeBufToString : public std::streambuf
{
    public:
        /*! \brief Constructor
         *
         * \param [in] sb The streambuf to tee from
         * \param [in] str The string to copy output to
         */ 
        TeeBufToString(std::streambuf * sb, std::string * str) noexcept
            : sb_(sb), str_(str)
        { }

        TeeBufToString & operator=(TeeBufToString &&)      = default;
        TeeBufToString(TeeBufToString &&)                  = default;
        TeeBufToString(const TeeBufToString &)             = delete;
        TeeBufToString & operator=(const TeeBufToString &) = delete;

    protected:
        virtual std::streamsize xsputn(const char * s, std::streamsize n)
        {
            std::streamsize n1 = sb_->sputn(s, n);
            str_->append(s, n);
    
            return n1;
        }   
        
        virtual int overflow(int c)
        {
            if (c == EOF)
            {
                return !EOF;
            }   
            else
            {
                str_->append(1, static_cast<char>(c));
                return sb_->sputc(static_cast<char>(c));
            }   
        }   
        
        
        virtual int sync()
        {
            return sb_->pubsync();
        }   
        
    private:
        std::streambuf * sb_;
        std::string * str_;
};      


/*! \brief Guards a change to an ostream buffer via RAII
 *
 * The internal buffer of the given ostream is changed.
 * On destruction, the stream is given its original buffer back.
 */
class OstreamBufGuard
{
    public:
        /*! \brief Constructor
         *
         * \param [in] os The stream to manupulate
         * \param [in] str The buffer to use in \p os
         */ 
        OstreamBufGuard(std::ostream * os, std::streambuf * newbuf) noexcept
            : os_(os), origbuf_(os->rdbuf())
        {
            os_->rdbuf(newbuf);
        }

        /*! \brief Sets the buffer of the ostream to its original buffer
         */
        virtual ~OstreamBufGuard()
        {
            os_->rdbuf(origbuf_);
        }

        OstreamBufGuard & operator=(OstreamBufGuard &&)      = default;
        OstreamBufGuard(OstreamBufGuard &&)                  = default;
        OstreamBufGuard(const OstreamBufGuard &)             = delete;
        OstreamBufGuard & operator=(const OstreamBufGuard &) = delete;

    private:
        std::ostream * os_;
        std::streambuf * origbuf_;
};


} // close namespace detail



/*! \brief Tees output of a stream to a string, and resets the buffer on destruction
 *
 * The buffer of os is changed to one that tees output to the given string. 
 * On destruction, the stream is given its original buffer back.
 */
class StringTeeGuard
{
    public:
        /*! \brief Constructor
         *
         * \param [in] os The stream to tee from
         * \param [in] str The string to copy output to
         */ 
        StringTeeGuard(std::ostream * os, std::string * str) noexcept
            : tee_(os->rdbuf(), str),
              guard_(os, &tee_)
        {
        }


        /*! \brief Sets the buffer of the ostream to its original buffer
         */
        ~StringTeeGuard() = default; // everything handled in guard_ destructor

        StringTeeGuard & operator=(StringTeeGuard &&)      = default;
        StringTeeGuard(StringTeeGuard &&)                  = default;
        StringTeeGuard(const StringTeeGuard &)             = delete;
        StringTeeGuard & operator=(const StringTeeGuard &) = delete;

    private:
        detail::TeeBufToString tee_;
        detail::OstreamBufGuard guard_;
};







/*! \brief Returns true if the output system is valid 
 */
bool Valid(void);


/*! \brief Sets the output to stdout
 *
 * Will reset any string tees
 *
 * \see \ref developer_output_page
 */
void SetOut_Stdout(void);


/*! \brief Sets the output to stderr
 *
 * Will reset any string tees
 *
 * \see \ref developer_output_page
 */
void SetOut_Stderr(void);


/*! \brief Sets the output to a file
 *
 * Will reset any string tees
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


/*! \brief Tees output to the given string
 *
 * A guard object is returned. On destruction, this object
 * sets the output back to its original state.
 *
 * The given string must exist until the returned object is destructed,
 * for obvious reasons.
 */
StringTeeGuard TeeToString(std::string * str);



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
    Output(std::cout, fmt, Fargs...);
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
    Changed(std::cout, fmt, Fargs...);
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
    Error(std::cout, fmt, Fargs...);
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
    Warning(std::cout, fmt, Fargs...);
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
    Success(std::cout, fmt, Fargs...);
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
    Debug(std::cout, fmt, Fargs...);
}






namespace export_python {

/*! \brief Wrap printing functions for use from python
 *
 * This function takes a python list rather than the parameter pack
 *
 * \param [in] os Output stream to send the output to
 * \param [in] type The type of output
 * \param [in] fmt Format string to use
 * \param [in] args Arguments to the format string
 */
void Output(std::ostream & os, detail::OutputType type,
            const std::string & fmt, const std::vector<std::string> & args);

} // close namespace export_python





} // close namespace output
} // close namespace bpmodule


#endif
