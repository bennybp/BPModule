/*! \file
 *
 * \brief Functions for hashing an object that is serializable
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_UTIL__HASHSERIALIZABLE_HPP_
#define PULSAR_GUARD_UTIL__HASHSERIALIZABLE_HPP_

#include "pulsar/util/HashingArchive.hpp"


namespace pulsar{
namespace util {


/*! \brief Hash a serializable object
 *
 * This function is used to obtain the hash for any
 * object that can be serialized
 */ 
template<typename... Targs>
Hash HashSerializable(const Targs &... args)
{
    detail::HashingArchive harchive;
    harchive(args...);
    return harchive.Finalize();
}



} // close namespace util
} // close namespace pulsar


//////////////////////////////////////////////
// Register. We only have an output archive
// (must be done outside of namespaces)
//////////////////////////////////////////////
CEREAL_REGISTER_ARCHIVE(pulsar::util::detail::HashingArchive)


#endif
