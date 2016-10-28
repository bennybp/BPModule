/*! \file
 *
 * \brief Various string helper functions (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <algorithm>
#include "pulsar/util/StringUtil.hpp"


namespace pulsar{

void to_lower(std::string & str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}


std::string to_lower_copy(std::string str)
{
    to_lower(str);
    return str;
}

void left_trim(std::string & s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
}

void right_trim(std::string & s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

void trim(std::string & s)
{
    right_trim(s);
    left_trim(s);
}

std::string left_trim_copy(std::string s)
{
    left_trim(s);
    return s;
}

std::string right_trim_copy(std::string s)
{
    right_trim(s);
    return s;
}

std::string trim_copy(std::string s)
{
    trim(s);
    return s;
}


bool CaseInsensitiveLess::operator()(std::string lhs, std::string rhs) const
{
    to_lower(lhs);
    to_lower(rhs);
    std::less<std::string> comp;
    return comp(lhs, rhs);
}


bool CaseInsensitivetrimLess::operator()(std::string lhs, std::string rhs) const
{
    trim(lhs);    trim(rhs);
    to_lower(lhs); to_lower(rhs);
    std::less<std::string> comp;
    return comp(lhs, rhs);
}


bool CaseInsensitiveEquality::operator()(std::string lhs, std::string rhs) const
{
    to_lower(lhs);
    to_lower(rhs);
    std::equal_to<std::string> comp;
    return comp(lhs, rhs);
}


bool CaseInsensitivetrimEquality::operator()(std::string lhs, std::string rhs) const
{
    trim(lhs);    trim(rhs);
    to_lower(lhs); to_lower(rhs);
    std::equal_to<std::string> comp;
    return comp(lhs, rhs);
}



std::string line(char c, int n)
{
    return std::string(n, c) + "\n";
}

} // close namespace pulsar

