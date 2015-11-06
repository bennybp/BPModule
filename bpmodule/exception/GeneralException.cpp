/*! \file
 *
 * \brief The base, general exception for BPModule (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include <sstream>

#include "bpmodule/exception/GeneralException.hpp"


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
    // This is made to explicitly NOT depend on other core modules
    // so it duplicates some minor stuff (formatting, line, etc)
    ExceptionInfo exinfo = GetInfo();
    std::stringstream ss;
    ss << "\n";
    ss << std::string(80, '*') << "\n";
    ss << "Exception thrown!\n";
    ss << "what() = " << what() << "\n"; 
    for(auto & it : exinfo)
    {
        if(it.second.size())
        {
            std::stringstream sstr(it.second);
            std::string str;
            std::getline(sstr, str);

            auto oldw = ss.width(24);
            ss << it.first;
            ss.width(oldw);
            ss << " : " << str << "\n";

            while(std::getline(sstr, str).good())
            {
                oldw = ss.width(24);
                ss << " ";
                ss.width(oldw);
                ss << "     " << str << "\n";
            }
        }
    }
    return ss.str();
}


} // close namespace exception
} // close namespace bpmodule
