#ifndef BPMODULE_GUARD_MATH__CAST_HPP_
#define BPMODULE_GUARD_MATH__CAST_HPP_

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
//template<typename Target, typename Source>
//typename std::enable_if<std::is_arithmetic<Source>::value && std::is_arithmetic<Target>::value, Target>::type
//numeric_cast(const Source & s)
template<typename Target, typename Source>
Target numeric_cast(const Source & s)
{
    return detail::ExactCast<Target, Source>::Cast(s);
}



/*! \brief Convert between integer and floating point types, with rounding
 *
 * \todo NYI
 */
template<typename Target, typename Source>
Target round_cast(Source s)
{
    static_assert(  ( std::is_arithmetic<Source>::value && std::is_arithmetic<Target>::value ),
                    "Attempting to perform round cast on non-arithmetic types");

    static_assert(std::is_same<Source, Target>::value, "TODO: round_cast between different types");

    return s;
}




} // close namespace math
} // close namespace bpmodule



#endif
