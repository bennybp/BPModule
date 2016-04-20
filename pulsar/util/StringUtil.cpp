/*! \file
 *
 * \brief Various string helper functions (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <algorithm>
#include "bpmodule/util/StringUtil.hpp"


namespace bpmodule {
namespace util {


void ToLower(std::string & str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}


std::string ToLower_Copy(std::string str)
{
    ToLower(str);
    return str;
}

void LeftTrim(std::string & s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
}

void RightTrim(std::string & s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

void Trim(std::string & s)
{
    RightTrim(s);
    LeftTrim(s);
}

std::string LeftTrim_Copy(std::string s)
{
    LeftTrim(s);
    return s;
}

std::string RightTrim_Copy(std::string s)
{
    RightTrim(s);
    return s;
}

std::string Trim_Copy(std::string s)
{
    Trim(s);
    return s;
}


bool CaseInsensitiveLess::operator()(std::string lhs, std::string rhs) const
{
    ToLower(lhs);
    ToLower(rhs);
    std::less<std::string> comp;
    return comp(lhs, rhs);
}


bool CaseInsensitiveTrimLess::operator()(std::string lhs, std::string rhs) const
{
    Trim(lhs);    Trim(rhs);
    ToLower(lhs); ToLower(rhs);
    std::less<std::string> comp;
    return comp(lhs, rhs);
}


bool CaseInsensitiveEquality::operator()(std::string lhs, std::string rhs) const
{
    ToLower(lhs);
    ToLower(rhs);
    std::equal_to<std::string> comp;
    return comp(lhs, rhs);
}


bool CaseInsensitiveTrimEquality::operator()(std::string lhs, std::string rhs) const
{
    Trim(lhs);    Trim(rhs);
    ToLower(lhs); ToLower(rhs);
    std::equal_to<std::string> comp;
    return comp(lhs, rhs);
}



std::string Line(char c, int n)
{
    return std::string(n, c) + "\n";
}



} // close namespace util
} // close namespace bpmodule

