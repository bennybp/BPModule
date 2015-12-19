/*! \file
 *
 * \brief The base, general exception for BPModule (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include <sstream>

#include "bpmodule/exception/Exceptions.hpp"


namespace bpmodule {
namespace exception {


void GeneralException::AppendInfo(void)
{
}


void GeneralException::AppendInfo(const std::string & key, const std::string & value)
{
    // done manully rather than through util::FormatString
    // so that there is no dependence on that module
    std::stringstream sstr(value);
    std::string str;
    std::getline(sstr, str);

    std::stringstream ss;
    auto oldw = ss.width(24);
    ss << key;
    ss.width(oldw);
    ss << " : " << str;

    while(std::getline(sstr, str).good())
    {
        ss << "\n";
        oldw = ss.width(24);
        ss << " ";
        ss.width(oldw);
        ss << "     " << str << "\n";
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

    whatstr_.append("\n");
    whatstr_.append(escaped);
}


const char * GeneralException::what(void) const noexcept
{
    return whatstr_.c_str();
}




} // close namespace exception
} // close namespace bpmodule
