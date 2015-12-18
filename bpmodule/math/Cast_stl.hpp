#ifndef _GUARD_CAST_STL_HPP_
#define _GUARD_CAST_STL_HPP_

#include <vector>
#include <set>

#include "bpmodule/math/Cast.hpp"

namespace bpmodule {
namespace math {


/*! \brief Numeric cast for vector types
 */
template<typename Target, typename Source>
std::vector<Target> numeric_cast(const std::vector<Source> & s)
{
    std::vector<Target> v;
    v.reserve(s.size());
    for(const auto & it : s)
        v.push_back(numeric_cast<Target>(it));
    return v;
}



/*! \brief Numeric cast for set types
 */
template<typename Target, typename Source>
std::set<Target> numeric_cast(const std::set<Source> & s)
{
    std::set<Target> v;
    for(const auto & it : s)
        v.insert(numeric_cast<Target>(it));
    return v;
}






} // close namespace math
} // close namespace bpmodule



#endif
