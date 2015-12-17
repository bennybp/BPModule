/*! \file
 *
 * \brief The base, general exception for BPModule (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include <sstream>

#include "bpmodule/exception/GeneralException.hpp"

namespace bpmodule {
namespace exception {

GeneralException::GeneralException(const std::string & whatstr)
    : whatstr_(whatstr)
{
    std::stringstream ss;
    ss << "\n";
    ss << std::string(80, '*') << "\n";
    ss << "Exception thrown!\n";
    ss << "what() = " << whatstr_ << "\n"; 

    fullstr_ = ss.str();
}

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

// needed for variadic template termination?
void GeneralException::AppendInfo(void)
{
}


void GeneralException::AppendInfo(const std::string & key, const std::string & value)
{
    exinfo_.push_back({key, value});

    std::stringstream ss;

    if(value.size())
    {
        // get value by line so we can indent
        std::stringstream sstr(value);
        std::string str;
        std::getline(sstr, str);

        auto oldw = ss.width(24);
        ss << key;
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

    // escape the boost::format percent signs
    std::string escaped;

    for(const auto & it : ss.str())
    {
        if(it == '%')
            escaped.append("%%");
        else
            escaped.push_back(it);
    }

    fullstr_ += escaped;
    
}


const char * GeneralException::what(void) const noexcept
{
    return fullstr_.c_str();
}



} // close namespace exception
} // close namespace bpmodule
