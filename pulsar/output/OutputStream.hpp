/*! \file
 *
 * \brief OutputStream class (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_OUTPUT__OUTPUTSTREAM_HPP_
#define PULSAR_GUARD_OUTPUT__OUTPUTSTREAM_HPP_

#include "pulsar/output/Output.hpp"


namespace pulsar{

/*! \brief A stream object with some output helpers
 *
 * This object derives from std::ostream, so it can be used as such.
 * In addition, it adds some functionality for semantic output
 * (ie, error()).
 *
 * If debug output is disabled for the stream, anything printed
 * via debug() will not be printed.
 */
class OutputStream : public std::ostream
{
    public:
        OutputStream(std::streambuf * sb)
            : std::ostream(sb), debug_(false)
        { }
        

        ////////////////////////////////////////////////////////////////////////////
        // We call print_output_ directly to help reduce variadic template instantiations
        // and, hopefully, compile times
        ////////////////////////////////////////////////////////////////////////////

        /*! \brief Print formatted text this stream
         *
         * \throw pulsar::PulsarException with malformed formats,
         *        improper number of arguments, etc.
         *
         * \tparam Targs The types of the arguments to print
         *
         * \param [in] fmt The format string to use
         * \param [in] type The type of output (debug, warning, etc)
         * \param [in] Fargs The arguments to the format string
         */
        template<typename... Targs>
        void general_output(const std::string & fmt, OutputType type, const Targs&... Fargs)
        {
            if(type == OutputType::Debug && !debug_)
                return;
            detail::print_output_(*this, type, format_string(fmt, Fargs...));
        }


        /*! \brief Print formatted general output to this stream
         * 
         * \throw pulsar::PulsarException with malformed formats,
         *        improper number of arguments, etc.
         *
         * \tparam Targs The types of the arguments to print
         *
         * \param [in] fmt The format string to use
         * \param [in] Fargs The arguments to the format string
         */
        template<typename... Targs>
        void output(const std::string & fmt, const Targs&... Fargs)
        {
            detail::print_output_(*this, OutputType::Output, format_string(fmt, Fargs...));
        }


        /*! \brief Print formatted changed output to a stream
         * \copydetails output(std::ostream &, const std::string &, const Targs...)
         */
        template<typename... Targs>
        void changed(const std::string & fmt, const Targs&... Fargs)
        {
            detail::print_output_(*this, OutputType::Changed, format_string(fmt, Fargs...));
        }


        /*! \brief Print formatted error output to a stream
         * \copydetails output(std::ostream &, const std::string &, const Targs...)
         */
        template<typename... Targs>
        void error(const std::string & fmt, const Targs&... Fargs)
        {
            detail::print_output_(*this, OutputType::Error, format_string(fmt, Fargs...));
        }


        /*! \brief Print formatted warning output to a stream
         * \copydetails output(std::ostream &, const std::string &, const Targs...)
         */
        template<typename... Targs>
        void warning(const std::string & fmt, const Targs&... Fargs)
        {
            detail::print_output_(*this, OutputType::Warning, format_string(fmt, Fargs...));
        }


        /*! \brief Print formatted success output to a stream
         * \copydetails output(std::ostream &, const std::string &, const Targs...)
         */
        template<typename... Targs>
        void success(const std::string & fmt, const Targs&... Fargs)
        {
            detail::print_output_(*this, OutputType::Success, format_string(fmt, Fargs...));
        }


        /*! \brief Print formatted debug output to a stream
         * 
         * Will only be printed if debug_enabled()
         *
         * \copydetails output(std::ostream &, const std::string &, const Targs...)
         */
        template<typename... Targs>
        void debug(const std::string & fmt, const Targs&... Fargs)
        {
            if(debug_)
                detail::print_output_(*this, OutputType::Debug, format_string(fmt, Fargs...));
        }


        /*! \brief See if debug printing is enabled in this stream
         * 
         * \return True if debugging is enabled for this stream, False if it's not
         */ 
        bool debug_enabled(void) const noexcept
        {
            return debug_;
        }

        /*! \brief Enable/disable debug printing in this stream
         *
         * \param [in] debug True if you want debugging enabled for this stream, False to disable it 
         */ 
        void enable_debug(bool do_debug) noexcept
        {
            debug_ = do_debug;
        }

    private:
        bool debug_; //!< Is debugging enabled for this stream
};

} // close namespace pulsar


#endif
