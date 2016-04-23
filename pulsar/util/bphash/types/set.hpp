/*! \file
 *
 * \brief A class that hashes objects
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_TYPES__SET_HPP_
#define PULSAR_GUARD_TYPES__SET_HPP_

#include <set>
#include "pulsar/util/bphash/types/ContainerHelper.hpp"

namespace pulsar{
namespace util {
namespace detail {


///////////////
// set
///////////////
template<typename Key, typename Compare, typename Alloc>
struct ObjectHasher<std::set<Key, Compare, Alloc>>
    : public ContainerHasher<std::set<Key, Compare, Alloc>> { };


} // close namespace detail
} // close namespace util
} // close namespace pulsar

#endif
