/*! \file
 *
 * \brief Conversion of angular momentum info to human-readable forms (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include <map>

#include "pulsar/system/AMConvert.hpp"
#include "pulsar/util/StringUtil.hpp" // for case insensitive compare
#include "pulsar/exception/Exceptions.hpp"

using pulsar::util::CaseInsensitiveLess;

static const std::map<std::string, int, CaseInsensitiveLess> toi_{ { "spdfgh", -5 },
                                                                   { "spdfg",  -4 },
                                                                   { "spdf",   -3 },
                                                                   { "spd",    -2 },
                                                                   { "sp",     -1 },
                                                                   { "s",       0 },
                                                                   { "p",       1 },
                                                                   { "d",       2 },
                                                                   { "f",       3 },
                                                                   { "g",       4 },
                                                                   { "h",       5 },
                                                                   { "i",       6 },
                                                                   { "j",       7 },
                                                                   { "k",       8 },
                                                                   { "l",       9 },
                                                                   { "m",      10 },
                                                                   { "n",      11 },
                                                                   { "o",      12 },
                                                                   { "q",      13 },
                                                                   { "r",      14 },
                                                                   { "t",      15 },
                                                                   { "u",      16 },
                                                                   { "v",      17 },
                                                                   { "w",      18 },
                                                                   { "x",      19 },
                                                                   { "y",      20 },
                                                                   { "z",      21 } };

static const std::map<int, std::string> tostr_{ { -5, "spdfgh"},
                                                { -4, "spdfg"},
                                                { -3, "spdf"},
                                                { -2, "spd"},
                                                { -1, "sp"},
                                                {  0, "s"},
                                                {  1, "p"},
                                                {  2, "d"},
                                                {  3, "f"},
                                                {  4, "g"},
                                                {  5, "h"},
                                                {  6, "i"},
                                                {  7, "j"},
                                                {  8, "k"},
                                                {  9, "l"},
                                                { 10, "m"},
                                                { 11, "n"},
                                                { 12, "o"},
                                                { 13, "q"},
                                                { 14, "r"},
                                                { 15, "t"},
                                                { 16, "u"},
                                                { 17, "v"},
                                                { 18, "w"},
                                                { 19, "x"},
                                                { 20, "y"},
                                                { 21, "z"} };


namespace pulsar{
namespace system {


int StringToAM(const std::string & s)
{
    if(toi_.count(s))
        return toi_.at(s);
    else
        throw exception::BasisSetException("Cannot find integer AM for this string", "str", s);
}


std::string AMToString(int am)
{
    if(tostr_.count(am))
        return tostr_.at(am);
    else
        throw exception::BasisSetException("Cannot find string for this integer AM", "int", am);
}



} // close namespace system
} // close namespace pulsar

