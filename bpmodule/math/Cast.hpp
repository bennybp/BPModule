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
 * Checks for overflows and underflows, as well as loss of precision with floating
 * point conversions.
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

    // no casting
    if(std::is_same<Target, Source>::value)
        return static_cast<Target>(s);  // cast to make compiler warnings go away

    ////////////////////////////////////////////////////////////////////////////////
    // this is written with branching so that the limits checking isn't always done
    // (ie, going from short->int should be pretty much optimized out)
    //
    // Actually, most branching shown here should be optimized out at compile
    ////////////////////////////////////////////////////////////////////////////////

    ///////////////////
    // for integers
    ///////////////////
    if(std::is_integral<Source>::value && std::is_integral<Target>::value)
    {
        // both are signed or unsigned
        if(std::is_signed<Source>::value == std::is_signed<Target>::value)
        {
            // going from smaller to larger type - ok
            if(std::numeric_limits<Target>::digits >= std::numeric_limits<Source>::digits)
                return static_cast<Target>(s);
            else
            {
                // source is larger type than target. check limits
                Source tmax = static_cast<Source>(std::numeric_limits<Target>::max());
                Source tmin = static_cast<Source>(std::numeric_limits<Target>::lowest());
                if(s > tmax)
                    throw exception::MathException("Error in numeric_cast",
                                                   "desc", "source value overflows target type",
                                                   "ifrom", mangle::DemangleCppType<Source>(),
                                                   "ito", mangle::DemangleCppType<Target>());
                else if(s < tmin)
                    throw exception::MathException("Error in numeric_cast",
                                                   "desc", "source value underflows target type",
                                                   "ifrom", mangle::DemangleCppType<Source>(),
                                                   "ito", mangle::DemangleCppType<Target>());

                else // safe!
                    return static_cast<Target>(s);

            }
        }
        else if(std::is_signed<Source>::value) // Source is signed, Target is unsigned
        {
            if(s < 0)
                throw exception::MathException("Error in numeric_cast",
                                               "desc", "source value underflows target type",
                                               "ifrom", mangle::DemangleCppType<Source>(),
                                               "ito", mangle::DemangleCppType<Target>());

            
            // going from smaller to larger type - ok (since s >= 0)
            if(std::numeric_limits<Target>::digits >= std::numeric_limits<Source>::digits)
                return static_cast<Target>(s);
            else
            {
                // source is larger type than target. check other limit
                // casting to Source type should be ok since sizeof(Source) > sizeof(Target)
                Source tmax = static_cast<Source>(std::numeric_limits<Target>::max());
                if(s > tmax)
                    throw exception::MathException("Error in numeric_cast",
                                                   "desc", "source value overflows target type",
                                                   "ifrom", mangle::DemangleCppType<Source>(),
                                                   "ito", mangle::DemangleCppType<Target>());

                else // safe!
                    return static_cast<Target>(s);

            }
        }
        else  // Source is unsigned, target is signed
        {
            // going from smaller to larger type - ok
            // (since numeric_limits::digits reports bits without sign bit)
            if(std::numeric_limits<Target>::digits >= std::numeric_limits<Source>::digits)
                return static_cast<Target>(s);
            else
            {
                // source is larger type than target. check max limit
                // casting to Source type should be ok since sizeof(Source) > sizeof(Target)
                Source tmax = static_cast<Source>(std::numeric_limits<Target>::max());
                if(s > tmax)
                    throw exception::MathException("Error in numeric_cast",
                                                   "desc", "source value overflows target type",
                                                   "ifrom", mangle::DemangleCppType<Source>(),
                                                   "ito", mangle::DemangleCppType<Target>());

                else // safe!
                    return static_cast<Target>(s);

            }
        }
    }



    // if both are floating point
    // we have to check that the conversion is valid
    else if(std::is_floating_point<Source>::value && std::is_floating_point<Target>::value)
    {
        // Hackish way, but convert to target and back, then compare
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

    else
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
