/*! \file
 *
 * \brief Functions for hashing an object that is serializable
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_UTIL__HASHSERIALIZABLE_HPP_
#define BPMODULE_GUARD_UTIL__HASHSERIALIZABLE_HPP_

#include "bpmodule/util/HashingArchive.hpp"


namespace bpmodule {
namespace util {


/*! \brief Hash a serializable object
 *
 * This function is used to obtain the hash for any
 * object that can be serialized
 */ 
template<typename T>
Hash HashSerializable(const T & obj)
{
    detail::HashingArchive harchive;
    harchive(obj);
    return harchive.Finalize();
}



} // close namespace util
} // close namespace bpmodule


//////////////////////////////////////////////
// Register. We only have an output archive
// (must be done outside of namespaces)
//////////////////////////////////////////////
CEREAL_REGISTER_ARCHIVE(bpmodule::util::detail::HashingArchive)


#endif
