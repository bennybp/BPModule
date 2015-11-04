/*! \file
 *
 * \brief Various string helper functions (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <algorithm>
#include "bpmodule/util/StrUtil.hpp"


namespace bpmodule {
namespace util {

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


bool CaseInsensitiveCompare::operator()(std::string lhs, std::string rhs) const
{
    ToLower(lhs);
    ToLower(rhs);
    std::less<std::string> comp;
    return comp(lhs, rhs);
}


std::string Line(char c, int n)
{
    return std::string(n, c) + "\n";
}



} // close namespace util
} // close namespace bpmodule

