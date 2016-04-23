/*! \file
 *
 * \brief A class that hashes objects
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_TYPES__MAP_HPP_
#define PULSAR_GUARD_TYPES__MAP_HPP_

#include <map>
#include "pulsar/util/bphash/types/ContainerHelper.hpp"
#include "pulsar/util/bphash/types/utility.hpp"

namespace pulsar{
namespace util {
namespace detail {


///////////////
// map
///////////////
template<typename Key, typename T, typename Compare, typename Alloc>
struct ObjectHasher<std::map<Key, T, Compare, Alloc>>
    : public ContainerHasher<std::map<Key, T, Compare, Alloc>> { };


} // close namespace detail
} // close namespace util
} // close namespace pulsar

#endif
