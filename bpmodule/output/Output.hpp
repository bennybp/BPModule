#ifndef BPMODULE_GUARD_OUTPUT__OUTPUT_HPP_
#define BPMODULE_GUARD_OUTPUT__OUTPUT_HPP_

/*! \file
 *
 * \brief Output and printing functionality (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <iostream>
#include <string>
#include "bpmodule/output/OutputType.hpp"
#include "bpmodule/util/Format.hpp"


namespace bpmodule {
namespace output {


namespace detail {
    void Output_(std::ostream & os, OutputType type, const std::string & str);
}


template<typename ... Targs>
void GeneralOutput(std::ostream & os, OutputType type, const std::string & fmt, const Targs&... Fargs)
{
    detail::Output_(os, type, util::FormatString(fmt, Fargs...));
}


template<typename ... Targs>
void Output(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
{
    detail::Output_(os, OutputType::Output, util::FormatString(fmt, Fargs...));
}


template<typename ... Targs>
void Changed(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
{
    detail::Output_(os, OutputType::Changed, util::FormatString(fmt, Fargs...));
}


template<typename ... Targs>
void Error(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
{
    detail::Output_(os, OutputType::Error, util::FormatString(fmt, Fargs...));
}


template<typename ... Targs>
void Warning(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
{
    detail::Output_(os, OutputType::Warning, util::FormatString(fmt, Fargs...));
}


template<typename ... Targs>
void Success(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
{
    detail::Output_(os, OutputType::Success, util::FormatString(fmt, Fargs...));
}


template<typename ... Targs>
void Debug(std::ostream & os, const std::string & fmt, const Targs&... Fargs)
{
    detail::Output_(os, OutputType::Debug, util::FormatString(fmt, Fargs...));
}



} // close namespace output
} // close namespace bpmodule

#endif
