/*! \file
 *
 * \brief OutputStream class (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_OUTPUT__OUTPUTSTREAM_HPP_
#define BPMODULE_GUARD_OUTPUT__OUTPUTSTREAM_HPP_

#include <iostream>
#include <string>

#include "bpmodule/output/Output.hpp"
#include "bpmodule/util/Format.hpp"


namespace bpmodule {
namespace output {


// forward declare
template<typename... Targs> void Output(std::ostream & out, const std::string & fmt, const Targs&... Fargs);
template<typename... Targs> void Changed(std::ostream & out, const std::string & fmt, const Targs&... Fargs);
template<typename... Targs> void Warning(std::ostream & out, const std::string & fmt, const Targs&... Fargs);
template<typename... Targs> void Error(std::ostream & out, const std::string & fmt, const Targs&... Fargs);
template<typename... Targs> void Success(std::ostream & out, const std::string & fmt, const Targs&... Fargs);
template<typename... Targs> void Debug(std::ostream & out, const std::string & fmt, const Targs&... Fargs);



/*! \brief A stream object with some output helpers
 */
class OutputStream : public std::ostream
{
    public:
        OutputStream(std::streambuf * sb)
            : std::ostream(sb), debug_(false)
        { }
        

        template<typename... Targs>
        void GeneralOutput(const std::string & fmt, OutputType type, const Targs&... Fargs)
        {
            if(type == OutputType::Debug && !debug_)
                return;
            output::GeneralOutput(*this, type, util::FormatString(fmt, Fargs...));
        }

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
            if(debug_)
                output::Debug(*this, fmt, Fargs...);
        }

        bool DebugEnabled(void) const noexcept
        {
            return debug_;
        }

        void EnableDebug(bool debug) noexcept
        {
            debug_ = debug;
        }

    private:
        bool debug_;
};


} // close namespace output
} // close namespace bpmodule


#endif
