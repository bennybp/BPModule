#ifndef PULSAR_GUARD_MATH__CAST_STL_HPP_
#define PULSAR_GUARD_MATH__CAST_STL_HPP_

#include <vector>
#include <set>

#include "pulsar/math/ExactCast.hpp"



namespace pulsar{
namespace detail {


template<typename Target, typename Source>
struct ExactCast<std::vector<Target>, std::vector<Source>>
{
    static std::vector<Target> Cast(const std::vector<Source> & s)
    {
        std::vector<Target> r;
        r.reserve(s.size());
        for(const auto & it : s)
            r.push_back(ExactCast<Target, Source>::Cast(it));
        return r;
    }
};


template<typename Source>
struct ExactCast<std::vector<Source>, std::vector<Source>>
{
    static std::vector<Source> Cast(const std::vector<Source> & s)
    {
        return s;
    }
};




template<typename Target, typename Source>
struct ExactCast<std::set<Target>, std::set<Source>>
{
    static std::set<Target> Cast(const std::set<Source> & s)
    {
        std::set<Target> r;
        for(const auto & it : s)
            r.insert(ExactCast<Target, Source>::Cast(it));
        return r;
    }
};


template<typename Source>
struct ExactCast<std::set<Source>, std::set<Source>>
{
    static std::set<Source> Cast(const std::set<Source> & s)
    {
        return s;
    }
};



} // close namespace detail
} // close namespace pulsar



#endif
