/*! \file
 *
 * \brief Safe conversions of some options
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_OPTIONCONVERT_HPP_
#define _GUARD_OPTIONCONVERT_HPP_


#include <stdexcept>
#include <limits>

namespace bpmodule {
namespace datastore {
namespace detail {

template<typename T>
struct OptionConvert
{
    typedef T stored_type;
    typedef T ret_type;

    static ret_type Convert(stored_type val)
    {
        return val; 
    }
};



template<>
struct OptionConvert<int>
{
    typedef long stored_type;
    typedef int ret_type;

    static ret_type Convert(stored_type val)
    {
        if(val < std::numeric_limits<int>::min() ||
           val > std::numeric_limits<int>::max())
            throw std::runtime_error("Value is out of range for an integer");

        return static_cast<int>(val);
    }
};


template<>
template<typename T>
struct OptionConvert<std::vector<T>>
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
