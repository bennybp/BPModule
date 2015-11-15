#include <map>

#include "bpmodule/basisset/AMConvert.hpp"
#include "bpmodule/util/StringUtil.hpp" // for case insensitive compare

using bpmodule::util::CaseInsensitiveCompare;

static const std::map<std::string, int, CaseInsensitiveCompare> toi_{ { "s",  0 },
                                                                      { "p",  1 },
                                                                      { "d",  2 },
                                                                      { "f",  3 },
                                                                      { "g",  4 },
                                                                      { "h",  5 },
                                                                      { "i",  6 },
                                                                      { "j",  7 },
                                                                      { "k",  8 },
                                                                      { "l",  9 },
                                                                      { "m", 10 },
                                                                      { "n", 11 },
                                                                      { "o", 12 } };

static const std::map<int, std::string> tostr_{ {  0, "s"},
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
namespace basisset {

//! \todo exceptions
int StringToAM(const std::string & s)
{
    return toi_.at(s);
}

//! \todo exceptions
std::string AMToString(int am)
{
    return tostr_.at(am);
}



} // close namespace basisset
} // close namespace bpmodule

