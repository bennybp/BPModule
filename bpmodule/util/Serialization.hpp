/*! \file
 *
 * \brief Serialization helpers
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef BPMODULE_GUARD_UTIL__SERIALIZATION_HPP_
#define BPMODULE_GUARD_UTIL__SERIALIZATION_HPP_

// Included via SerializationArchives.hpp
//#include <cereal/cereal.hpp>
//#include <cereal/access.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/tuple.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>

#include "bpmodule/util/SerializationArchives.hpp"

#define DECLARE_SERIALIZATION_FRIENDS \
    friend class cereal::access; \
    template<typename T> friend class bpmodule::util::detail::StdStreamArchive;


typedef std::vector<char> ByteArray;


namespace bpmodule {
namespace util {

/*! \brief Convert a c++ object to a byte array
 *
 * The C++ object must be serializable
 */
template<typename T>
ByteArray ToByteArray(const T & obj)
{
    MemoryArchive mar;
    mar.BeginSerialization();
    mar.Serialize(obj);
    mar.EndSerialization();
    return mar.ToByteArray(); 
}


/*! \brief Create c++ object from a byte array
 *
 * The C++ object must be serializable
 */
template<typename T>
T FromByteArray(const ByteArray & arr)
{
    MemoryArchive mar;
    mar.FromByteArray(arr);
    mar.BeginUnserialization();
    T obj;
    mar.Unserialize(obj);
    mar.EndUnserialization();
    return obj; 
}


/*! \brief Create c++ object from a byte array
 *
 * The C++ object must be serializable
 */
template<typename T>
std::unique_ptr<T> NewFromByteArray(const ByteArray & arr)
{
    MemoryArchive mar;
    mar.FromByteArray(arr);
    mar.BeginUnserialization();
    std::unique_ptr<T> objptr(new T);
    mar.Unserialize(*objptr);
    mar.EndUnserialization();
    return std::move(objptr);
}




} // close namespace util
} // close namespace bpmodule

#endif

