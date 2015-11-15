#ifndef _GUARD_AMCONVERT_HPP_
#define _GUARD_AMCONVERT_HPP_

#include <string>

namespace bpmodule {
namespace basisset {

int StringToAM(const std::string & s);

std::string AMToString(int am);



} // close namespace basisset
} // close namespace bpmodule

#endif
