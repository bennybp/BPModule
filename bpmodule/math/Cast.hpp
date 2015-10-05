#include <boost/numeric/conversion/cast.hpp>

#include "bpmodule/exception/MathException.hpp"

//! \todo maybe do manually to not draw in boost?

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

    try {
        // if both are integral types, ok
        if(std::is_integral<Source>::value && std::is_integral<Target>::value)
           return boost::numeric_cast<Target>(s);

        // if both are floating point, ok
        if(std::is_floating_point<Source>::value && std::is_floating_point<Target>::value)
           return boost::numeric_cast<Target>(s);
    }
    catch(std::exception & ex)
    {
        throw exception::MathException("Error in numeric_cast", "info", ex.what());
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


