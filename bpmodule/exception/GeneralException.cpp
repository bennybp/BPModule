/*! \file
 *
 * \brief The base, general exception for BPModule (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include <sstream>

#include "bpmodule/exception/GeneralException.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/util/FormatString.hpp"
#include "bpmodule/util/StringUtil.hpp"


namespace bpmodule {
namespace exception {


const GeneralException::ExceptionInfo & GeneralException::GetInfo(void) const noexcept
{
    return exinfo_;
}

const char * GeneralException::GetField(const std::string & field) const noexcept
{
    for(const auto & it : exinfo_)
    {
        if(it.first == field)
            return it.second.c_str();
    }

    return "(field not found)";
}


void GeneralException::AppendInfo(void)
{
}


void GeneralException::AppendInfo(const std::string & key, const std::string & value)
{
    exinfo_.push_back({key, value});
}


const char * GeneralException::what(void) const noexcept
{
    return whatstr_.c_str();
}



std::string GeneralException::ExceptionString(void) const
{
    ExceptionInfo exinfo = GetInfo();
    std::stringstream ss;
    util::FormatStream(ss, "\n");
    util::FormatStream(ss, util::Line('*'));
    util::FormatStream(ss, "Exception thrown!\n");
    util::FormatStream(ss, "what() = %1%\n", what());
    for(auto & it : exinfo)
    {
        if(it.second.size())
        {
            std::stringstream sstr(it.second);
            std::string str;
            std::getline(sstr, str);

            util::FormatStream(ss, "%|24| : %|-|\n", it.first, str);

            while(std::getline(sstr, str).good())
                util::FormatStream(ss, "%|24|       %|-|\n", "", str);
        }
    }
    return ss.str();
}


} // close namespace exception
} // close namespace bpmodule
