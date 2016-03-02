// Getting/checking information about a number

#ifndef BPMODULE_GUARD_MATH__CHECKING_HPP_
#define BPMODULE_GUARD_MATH__CHECKING_HPP_



#include <cmath>


namespace bpmodule {
namespace math {

namespace detail {


/*! \brief Checks to see if a number is an integer
 *
 * Always true if \p T is an integer type. Otherwise,
 * checks the value to see if it is exactly an integer
 */
template<typename T, bool = std::is_integral<T>::value>
struct IntegerCheck
{
};


template<typename T>
struct IntegerCheck<T, true>
{
    static bool IsInteger(T val)
    {
        return true; 
    }
};



template<typename T>
struct IntegerCheck<T, false>
{
    static bool IsInteger(T val)
    {
        PRAGMA_WARNING_PUSH
        PRAGMA_WARNING_IGNORE_FP_EQUALITY
        return (std::fmod(val, static_cast<T>(1.0)) == static_cast<T>(0));
        PRAGMA_WARNING_POP
    }
};


} // close namespace detail


template<typename T>
bool IsInteger(T val)
{
    return detail::IntegerCheck<T>::IsInteger(val);
}

} // close namespace math
} // close namespace bpmodule

#endif
