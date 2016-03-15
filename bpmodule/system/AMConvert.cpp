#include <map>

#include "bpmodule/system/AMConvert.hpp"
#include "bpmodule/util/StringUtil.hpp" // for case insensitive compare
#include "bpmodule/exception/Exceptions.hpp"

using bpmodule::util::CaseInsensitiveLess;

static const std::map<std::string, int, CaseInsensitiveLess> toi_{ { "spdf", -3},
                                                                      { "spd",  -2},
                                                                      { "sp",   -1},
                                                                      { "s",     0 },
                                                                      { "p",     1 },
                                                                      { "d",     2 },
                                                                      { "f",     3 },
                                                                      { "g",     4 },
                                                                      { "h",     5 },
                                                                      { "i",     6 },
                                                                      { "j",     7 },
                                                                      { "k",     8 },
                                                                      { "l",     9 },
                                                                      { "m",    10 },
                                                                      { "n",    11 },
                                                                      { "o",    12 } };

static const std::map<int, std::string> tostr_{ { -3, "spdf"},
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
                                                { 12, "o"} };


namespace bpmodule {
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
} // close namespace bpmodule

