#ifndef _GUARD_CAST_HPP_
#define _GUARD_CAST_HPP_

#include "bpmodule/math/ExactCast.hpp"

namespace bpmodule {
namespace math {



/*! \brief Perform a safe cast between integer types or between floating point types
 *
 * Checks for overflows and underflows, as well as loss of precision with floating
 * point conversions.
 *
 * Cannot be used to convert integer <-> floating point. Use round_cast instead.
 *
 * \throw bpmodule::exception::MathException if there is a problem (overflow, underflow, etc)
 */
template<typename Target, typename Source>
Target numeric_cast(const Source & s)
{
    static_assert(  ( std::is_integral<Source>::value && std::is_integral<Target>::value) ||
                    ( std::is_floating_point<Source>::value && std::is_floating_point<Target>::value),
                    "Attempting to perform integer <-> floating point conversion using numeric_cast. Consider round_cast");
    return detail::ExactCast<Target, Source>::Cast(s);
}



/*! \brief Convert between integer and floating point types, with rounding
 *
 * \todo NYI
 */
template<typename Target, typename Source>
Target round_cast(Source s)
{
    static_assert(std::is_same<Source, Target>::value, "TODO: round_cast between different types");
    return s;
}




} // close namespace math
} // close namespace bpmodule



#endif
