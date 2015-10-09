/*! \file
 *
 * \brief Safe conversions of some options
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_OPTIONTYPES_HPP_
#define _GUARD_OPTIONTYPES_HPP_


#include <stdexcept>
#include <limits>
#include <vector>
#include <typeinfo>
#include <cstdint>

#include "bpmodule/math/Cast.hpp"

namespace bpmodule {
namespace options {
namespace detail {




////////////////////////////////////////////////
// Storage types for integral and floating point
////////////////////////////////////////////////
//! \todo Arbitrary precision math lib?
typedef intmax_t OptionInt;
typedef long double OptionFloat;






///////////////////////////////////////////
// Valid types for getting/setting options
///////////////////////////////////////////

/*! \brief Determines if a type is valid for an option
 *
 * The value static member will be true if type T is a valid type
 * for an option
 */
template<typename T>
struct IsValidType
{
    // is_arithmetic includes bool
    static constexpr bool value = (std::is_arithmetic<T>::value || std::is_same<T, std::string>::value);
};


/*! \brief Determines if a vector type is valid for an option
 *
 * The value static member will be true if type T is a valid
 * vector type for an option
 */
template<typename T>
struct IsValidType<std::vector<T>>
{
    static constexpr bool value = IsValidType<T>::value;
};





/////////////////////////////////
// Mapping of Valid types to their
// corresponding stored types
////////////////////////////////

/*! \brief Determines the stored type for an option
 *
 * The stored_type typedef will represent the type actually
 * stored for a option given as type T
 *
 * The default is that the stored_type is the same as the given type
 */
template<typename T, bool = std::is_integral<T>::value, bool = std::is_floating_point<T>::value>
struct OptionStoreType
{
    typedef T stored_type;
};


/*! \brief Determines the stored type for an integral option
 *
 * Integral type specialization. The store_type
 * typedef represents the type stored for all integral
 * options
 */
template<typename T>
struct OptionStoreType<T, true, false>
{
    typedef OptionInt stored_type;
};


/*! \brief Determines the stored type for a floating point option
 *
 * Floating point type specialization. The store_type
 * typedef represents the type stored for all floating point
 * options
 */
template<typename T>
struct OptionStoreType<T, false, true>
{
    typedef OptionFloat stored_type;
};


/*! \brief Determines the stored type for a boolean option
 *
 * Bool type specialization. Bool is stored as bool. This
 * is required so that it isn't stored as an integral type
 * (but std::is_integral<bool>::value is true).
 */
template<>
struct OptionStoreType<bool>
{
    typedef bool stored_type;
};




/////////////////////////////
// Structure for conversion
/////////////////////////////

/*! \brief Converts an option value from its stored type
 *
 * Default struct is empty, therefore invalid for use
 */
template<typename T, bool = std::is_arithmetic<T>::value>
struct OptionConvert
{
};




////////////////////
// Arithmetic types
////////////////////
/*! \brief Structure used for converting to/from stored option types
 *
 * For arithmetic types
 */
template<typename T>
struct OptionConvert<T, true>
{
    typedef T ret_type;
    typedef typename OptionStoreType<T>::stored_type stored_type;

    /*! \brief Convert from a stored type to a given type
     *
     * \throw bpmodule::exception::MathException if there
     *        is a problem with the conversion/casting
     *        (including overflows and underflows)
     */
    static ret_type ConvertFromStored(stored_type val)
    {
        return math::numeric_cast<ret_type>(val);
    }


    /*! \brief Convert from the given type to its stored type
     *
     * \throw bpmodule::exception::MathException if there
     *        is a problem with the conversion/casting
     *        (including overflows and underflows)
     */
    static stored_type ConvertToStored(ret_type val)
    {
        return math::numeric_cast<stored_type>(val);
    }
};


///////////////////////////
// Non-arithmetic types
// (no conversion)
///////////////////////////
/*! \brief Structure used for converting to/from stored option types
 *
 * For non-arithmetic types (no conversion is performed)
 */
template<typename T>
struct OptionConvert<T, false>
{
    typedef T ret_type;
    typedef T stored_type;

    /*! \brief Convert from a stored type to a given type
     */
    static ret_type ConvertFromStored(stored_type val)
    {
        return val;
    }

    /*! \brief Convert from the given type to its stored type
     */
    static stored_type ConvertToStored(ret_type val)
    {
        return val;
    }
};




/////////////////////////////
// Vector types
/////////////////////////////
/*! \brief Structure used for converting to/from stored option types
 *
 * For vector types
 */
template<typename T>
struct OptionConvert<std::vector<T>, false>
{
    typedef typename OptionConvert<T>::stored_type  inner_stored_type;
    typedef          T                              inner_ret_type;

    typedef typename std::vector<inner_stored_type> stored_type;
    typedef std::vector<T>                          ret_type;


    /*! \brief Convert from a stored type to a given type
     *
     * \throw bpmodule::exception::MathException if there
     *        is a problem with the conversion/casting
     *        (including overflows and underflows)
     */
    static ret_type ConvertFromStored(stored_type val)
    {
        ret_type ret;
        ret.reserve(val.size());

        for(auto it = val.begin(); it != val.end(); ++it)
        {
            try {
                ret.push_back(OptionConvert<inner_ret_type>::ConvertFromStored(*it));
            }
            catch(exception::MathException & ex)  // catch overflow, etc
            {
                ex.AppendInfo("vectorelement", std::to_string(std::distance(val.begin(), it)));
                throw;
            }
        }

        return ret;
    }


    /*! \brief Convert from the given type to its stored type
     *
     * \throw bpmodule::exception::MathException if there
     *        is a problem with the conversion/casting
     *        (including overflows and underflows)
     */
    static stored_type ConvertToStored(ret_type val)
    {
        stored_type st;
        st.reserve(val.size());

        for(auto it = val.begin(); it != val.end(); ++it)
        {
            try {
                st.push_back(OptionConvert<inner_stored_type>::ConvertToStored(*it));
            }
            catch(exception::MathException & ex)  // catch overflow, etc
            {
                ex.AppendInfo("vectorelement", std::to_string(std::distance(val.begin(), it)));
                throw;
            }
        }

        return st;
    }
};




} // close namespace detail
} // close namespace options
} // close namespace bpmodule

#endif
