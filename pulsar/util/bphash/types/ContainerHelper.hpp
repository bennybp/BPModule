/*! \file
 *
 * \brief Helpers for hashing STL containers
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_TYPES__CONTAINERHELPER_HPP_
#define PULSAR_GUARD_TYPES__CONTAINERHELPER_HPP_

#include "pulsar/util/bphash/Hasher.hpp"

namespace pulsar{
namespace util {
namespace detail {


//////////////////////////////////////////
// Helper for STL containers
//////////////////////////////////////////
template<typename Cont>
struct ContainerHasher : public std::true_type
{
    static void
    Hash(Hasher & hasher, const Cont & cont)
    {
        hasher(static_cast<size_t>(cont.size()));
        for(const auto & it : cont)
            hasher(it);
    }
};


} // close namespace detail
} // close namespace util
} // close namespace pulsar

#endif
