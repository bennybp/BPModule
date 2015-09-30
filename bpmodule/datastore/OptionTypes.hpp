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

namespace bpmodule {
namespace datastore {
namespace detail {


/////////////////////////////////
// Valid types
////////////////////////////////
template<typename T>
struct IsValidType
{
    static constexpr bool value = (std::is_arithmetic<T>::value || std::is_same<T, std::string>::value);
};


// specialization for vector
// (check the inner type)
template<>
template<typename T>
struct IsValidType<std::vector<T>>
{
    static constexpr bool value = IsValidType<T>::value;
};





/////////////////////////////////
// Mapping of Valid types to their
// corresponding stored types
////////////////////////////////

// by default, it's the same
template<typename T, bool = std::is_integral<T>::value, bool = std::is_floating_point<T>::value>
struct OptionStoreType
{
    typedef T stored_type;
};


// for integral types
template<typename T>
struct OptionStoreType<T, true, false>
{
    typedef long stored_type;
};


// for floating point types
template<typename T>
struct OptionStoreType<T, false, true>
{
    typedef double stored_type;
};


// for bool (since bool is in integral type, but we
// don't want implicit conversions)
template<>
struct OptionStoreType<bool>
{
    typedef bool stored_type;
};




/////////////////////////////
// Structure for conversion
/////////////////////////////
template<typename T, bool = std::is_arithmetic<T>::value>
struct OptionConvert
{
};




////////////////////
// Arithmetic types
////////////////////
template<typename T>
struct OptionConvert<T, true>
{
    typedef T ret_type;
    typedef typename OptionStoreType<T>::stored_type stored_type;

    static ret_type Convert(stored_type val)
    {
        if(val < std::numeric_limits<T>::min() ||
           val > std::numeric_limits<T>::max())
            throw std::runtime_error(std::string("Value is out of range for requested type: ") + typeid(T).name());
        else
            return static_cast<ret_type>(val); 
    }
};


///////////////////////////
// Non-arithmetic types
// (no conversion)
///////////////////////////
template<typename T>
struct OptionConvert<T, false>
{
    typedef T ret_type;
    typedef T stored_type;

    static ret_type Convert(stored_type val)
    {
        return val; 
    }
};




/////////////////////////////
// Vector types
/////////////////////////////
template<>
template<typename T>
struct OptionConvert<std::vector<T>, false>
{
    typedef typename OptionConvert<T>::stored_type inner_type;

    typedef typename std::vector<inner_type> stored_type;
    typedef std::vector<T> ret_type;


    static ret_type Convert(stored_type val)
    {
        ret_type ret;
        ret.reserve(val.size());

        for(const auto & it : val)
            ret.push_back(OptionConvert<T>::Convert(it));

        return ret;
    }
};




} // close namespace detail
} // close namespace datastore
} // close namespace bpmodule

#endif
