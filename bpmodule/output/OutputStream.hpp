/*! \file
 *
 * \brief OutputStream class (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_OUTPUT__OUTPUTSTREAM_HPP_
#define BPMODULE_GUARD_OUTPUT__OUTPUTSTREAM_HPP_

#include "bpmodule/output/Output.hpp"


namespace bpmodule {
namespace output {



/*! \brief A stream object with some output helpers
 *
 * This object derives from std::ostream, so it can be used as such.
 * In addition, it adds some functionality for semantic output
 * (ie, Error()).
 *
 * If debug output is disabled for the stream, anything printed
 * via Debug() will not be printed.
 */
class OutputStream : public std::ostream
{
    public:
        OutputStream(std::streambuf * sb)
            : std::ostream(sb), debug_(false)
        { }
        

        /*! \brief Print formatted text this stream
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
        void GeneralOutput(const std::string & fmt, OutputType type, const Targs&... Fargs)
        {
            if(type == OutputType::Debug && !debug_)
                return;
            output::GeneralOutput(*this, type, util::FormatString(fmt, Fargs...));
        }


        /*! \brief Print formatted general output to this stream
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
        void Output(const std::string & fmt, const Targs&... Fargs)
        {
            output::Output(*this, fmt, Fargs...);
        }


        /*! \brief Print formatted changed output to a stream
         * \copydetails Output(std::ostream &, const std::string &, const Targs...)
         */
        template<typename... Targs>
        void Changed(const std::string & fmt, const Targs&... Fargs)
        {
            output::Changed(*this, fmt, Fargs...);
        }


        /*! \brief Print formatted error output to a stream
         * \copydetails Output(std::ostream &, const std::string &, const Targs...)
         */
        template<typename... Targs>
        void Error(const std::string & fmt, const Targs&... Fargs)
        {
            output::Error(*this, fmt, Fargs...);
        }


        /*! \brief Print formatted warning output to a stream
         * \copydetails Output(std::ostream &, const std::string &, const Targs...)
         */
        template<typename... Targs>
        void Warning(const std::string & fmt, const Targs&... Fargs)
        {
            output::Warning(*this, fmt, Fargs...);
        }


        /*! \brief Print formatted success output to a stream
         * \copydetails Output(std::ostream &, const std::string &, const Targs...)
         */
        template<typename... Targs>
        void Success(const std::string & fmt, const Targs&... Fargs)
        {
            output::Success(*this, fmt, Fargs...);
        }


        /*! \brief Print formatted debug output to a stream
         * 
         * Will only be printed if DebugEnabled()
         *
         * \copydetails Output(std::ostream &, const std::string &, const Targs...)
         */
        template<typename... Targs>
        void Debug(const std::string & fmt, const Targs&... Fargs)
        {
            if(debug_)
                output::Debug(*this, fmt, Fargs...);
        }


        /*! \brief See if debug printing is enabled in this stream
         * 
         * \return True if debugging is enabled for this stream, False if it's not
         */ 
        bool DebugEnabled(void) const noexcept
        {
            return debug_;
        }

        /*! \brief Enable/disable debug printing in this stream
         *
         * \param [in] debug True if you want debugging enabled for this stream, False to disable it 
         */ 
        void EnableDebug(bool debug) noexcept
        {
            debug_ = debug;
        }

    private:
        bool debug_; //!< Is debugging enabled for this stream
};


} // close namespace output
} // close namespace bpmodule


#endif
