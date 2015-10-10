#ifndef _GUARD_CAST_HPP_
#define _GUARD_CAST_HPP_

#include <limits>

#include "bpmodule/pragma.h"
#include "bpmodule/exception/MathException.hpp"
#include "bpmodule/mangle/Mangle.hpp"


namespace bpmodule {
namespace math {



/*! Perform a safe cast between integer types or between floating point types
 *
 * Checks for overflows and underflows.
 *
 * Cannot be used to convert integer <-> floating point. Use round_cast instead.
 *
 * \throw bpmodule::exception::MathException if there is a problem (overflow, underflow, etc)
 */
template<typename Target, typename Source>
Target numeric_cast(Source s)
{
    static_assert(  ( std::is_integral<Source>::value && std::is_integral<Target>::value) ||
                    ( std::is_floating_point<Source>::value && std::is_floating_point<Target>::value),
                    "Attempting to perform integer <-> floating point conversion using numeric_cast. Consider round_cast");


    // check for overflow, underflow
    if(s > std::numeric_limits<Source>::max())
        throw exception::MathException("Error in numeric_cast",
                                       "desc", "source value overflows target type",
                                       "fpfrom", mangle::DemangleCppType<Source>(),
                                       "fpto", mangle::DemangleCppType<Target>());

    if(s < std::numeric_limits<Source>::lowest())
        throw exception::MathException("Error in numeric_cast",
                                       "desc", "source value underflows target type",
                                       "fpfrom", mangle::DemangleCppType<Source>(),
                                       "fpto", mangle::DemangleCppType<Target>());


    // if both are integral types, checking for overflow/underflow
    // is all that's needed
    if(std::is_integral<Source>::value && std::is_integral<Target>::value)
        return static_cast<Target>(s);


    // if both are floating point
    // we have to check that the conversion is valid
    // Hackish way, but convert to target and back, then compare
    if(std::is_floating_point<Source>::value && std::is_floating_point<Target>::value)
    {
        // check for overflow and underflow
        Target t = static_cast<Target>(s);
        Source s2 = static_cast<Source>(t);

        // ignore this warning -- we are doing it on purpose
        PRAGMA_WARNING_PUSH
        PRAGMA_WARNING_IGNORE_FP_EQUALITY
        if(s == s2)
            return t;
        else
            throw exception::MathException("Error in numeric_cast",
                                           "desc", "Floating point conversion results in loss of precision",
                                           "fpfrom", mangle::DemangleCppType<Source>(),
                                           "fpto", mangle::DemangleCppType<Target>());
        PRAGMA_WARNING_POP
    }

    throw std::logic_error("numeric_cast unhandled types");
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
