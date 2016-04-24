/*! \file
 *
 * \brief A class that hashes objects
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_TYPES__STRING_HPP_
#define PULSAR_GUARD_TYPES__STRING_HPP_

//#include <string> // included via ContainerHelper
#include "pulsar/util/bphash/types/ContainerHelper.hpp"

namespace pulsar{
namespace util {
namespace detail {


///////////////
// string
///////////////
template<typename charT, typename Traits, typename Alloc>
struct ObjectHasher<std::basic_string<charT, Traits, Alloc>>
    : public ContainerHasher<std::basic_string<charT, Traits, Alloc>> { };


} // close namespace detail
} // close namespace util
} // close namespace pulsar

#endif
