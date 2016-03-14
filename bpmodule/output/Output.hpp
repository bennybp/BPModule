/*! \file
 *
 * \brief Output and printing functionality (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_OUTPUT__OUTPUT_HPP_
#define BPMODULE_GUARD_OUTPUT__OUTPUT_HPP_

#include <iostream>

#include "bpmodule/util/Format.hpp"



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

        void SetString(std::string * str) noexcept
        {
            str_ = str;
        }

    protected:
        virtual std::streamsize xsputn(const char * s, std::streamsize n)
        {
            std::streamsize n1 = sb_->sputn(s, n);

            if(str_ != nullptr)
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
                if(str_ != nullptr)
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



namespace detail {



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
    detail::Output_(out, OutputType::Output, util::FormatString(fmt, Fargs...));
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
void GlobalOutput(const std::string & fmt, const Targs&... Fargs)
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
    detail::Output_(out, OutputType::Changed, util::FormatString(fmt, Fargs...));
}


/*! \brief Print changed information to screen or file
 * \copydetails Output(const std::string &, Targs...)
 */
template<typename... Targs>
void GlobalChanged(const std::string & fmt, const Targs&... Fargs)
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
    detail::Output_(out, OutputType::Error, util::FormatString(fmt, Fargs...));
}


/*! \brief Print error information to screen or file
 * \copydetails Output(const std::string &, Targs...)
 */
template<typename... Targs>
void GlobalError(const std::string & fmt, const Targs&... Fargs)
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
    detail::Output_(out, OutputType::Warning, util::FormatString(fmt, Fargs...));
}


/*! \brief Print warning information to screen or file
 * \copydetails Output(const std::string &, Targs...)
 */
template<typename... Targs>
void GlobalWarning(const std::string & fmt, const Targs&... Fargs)
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
    detail::Output_(out, OutputType::Success, util::FormatString(fmt, Fargs...));
}


/*! \brief Print success information to screen or file
 * \copydetails Output(const std::string &, Targs...)
 */
template<typename... Targs>
void GlobalSuccess(const std::string & fmt, const Targs&... Fargs)
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
    detail::Output_(out, OutputType::Debug, util::FormatString(fmt, Fargs...));
}


/*! \brief Print debug information to screen or file
 * \copydetails Output(const std::string &, Targs...)
 */
template<typename... Targs>
void GlobalDebug(const std::string & fmt, const Targs&... Fargs)
{
    Debug(std::cout, fmt, Fargs...);
}




////////////////////////////
// Classes, etc, for output
////////////////////////////


/*! \brief A stream object with some output helpers
 */
class OutputStream : public std::ostream
{
    public:
        OutputStream(std::streambuf * sb)
            : std::ostream(sb), curtype_(OutputType::Output)
        { }

        template<typename... Targs>
        void Output(const std::string & fmt, const Targs&... Fargs)
        {
            output::Output(*this, fmt, Fargs...);
        }

        template<typename... Targs>
        void Changed(const std::string & fmt, const Targs&... Fargs)
        {
            output::Changed(*this, fmt, Fargs...);
        }

        template<typename... Targs>
        void Error(const std::string & fmt, const Targs&... Fargs)
        {
            output::Error(*this, fmt, Fargs...);
        }

        template<typename... Targs>
        void Warning(const std::string & fmt, const Targs&... Fargs)
        {
            output::Warning(*this, fmt, Fargs...);
        }

        template<typename... Targs>
        void Success(const std::string & fmt, const Targs&... Fargs)
        {
            output::Success(*this, fmt, Fargs...);
        }

        template<typename... Targs>
        void Debug(const std::string & fmt, const Targs&... Fargs)
        {
            output::Debug(*this, fmt, Fargs...);
        }

        void ResetType(void) noexcept
        {
            curtype_ = OutputType::Output;
        }

        OutputType SetType(OutputType type) noexcept
        {
            OutputType old = curtype_;
            curtype_ = type;
            return old;
        }

    private:
        OutputType curtype_;

};


inline OutputStream & operator<<(OutputStream & os, OutputType type)
{
    os.SetType(type);
    return os;
}




} // close namespace output
} // close namespace bpmodule


#endif
