/*! \file
 *
 * \brief Various string helper functions (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/output/FormatStr.hpp"


namespace bpmodule {
namespace output {

std::string Join(const std::vector<std::string> & vec, const std::string & j)
{
    if(vec.size() == 0)
        return std::string();

    std::string str = vec[0];

    for(size_t i = 1; i < vec.size(); ++i)
    {
        str.append(j);
        str.append(vec[i]);
    }

    return str;

}


void ToLower(std::string & str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}


std::string ToLowerCopy(std::string str)
{
    ToLower(str);
    return str;
}



} // close namespace output
} // close namespace bpmodule

