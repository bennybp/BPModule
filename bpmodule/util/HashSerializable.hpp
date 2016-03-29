/*! \file
 *
 * \brief Functions for hashing an object that is serializable
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_UTIL__HASHSERIALIZABLE_HPP_
#define BPMODULE_GUARD_UTIL__HASHSERIALIZABLE_HPP_

#include <cereal/cereal.hpp>

#include "bpmodule/util/Hasher.hpp"
#include "bpmodule/util/Hash.hpp"


namespace bpmodule {
namespace util {

namespace detail {

/*! \brief Custom Cereal archive type for hashing
 *
 * This prevents the need for serializing the whole
 * object into memory, and then hashing. Instead,
 * we can do it on the fly.
 */
class HashingArchive : public cereal::OutputArchive<HashingArchive>
{
    public:
        HashingArchive()
            : cereal::OutputArchive<HashingArchive>(this) { }

        HashingArchive(const HashingArchive &) = delete;
        HashingArchive & operator=(const HashingArchive &) = delete;
        HashingArchive(HashingArchive &&) = delete;
        HashingArchive & operator=(HashingArchive &&) = delete;


        /// Add data to the hasher
        void Update(void const * data, size_t size)
        {
            hasher_.Update(data, size);
        }


        /// Do any remaining steps and return the hash
        Hash Finalize(void) { return hasher_.Finalize(); }

    private:
        Hasher hasher_;
};




//////////////////////////////////////////////////////////////
// Below is basically a copy from
// cereral/archives/binary.hpp
//
// Except we don't have a load function, since loading from a
// hash is pretty impossible
//////////////////////////////////////////////////////////////

//! Hashing of POD types
template<class T> inline
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
CEREAL_SAVE_FUNCTION_NAME(HashingArchive & ar, T const & t)
{
  ar.Update(std::addressof(t), sizeof(t));
}

//! Hashing of NVP types
template <class Archive, class T> inline
typename std::enable_if<cereal::traits::is_same_archive<Archive, HashingArchive>::value, void>::type
CEREAL_SERIALIZE_FUNCTION_NAME( Archive & ar, cereal::NameValuePair<T> & t )
{
  ar( t.value );
}

//! Hashing cereal::SizeTags to binary
template <class Archive, class T> inline
typename std::enable_if<cereal::traits::is_same_archive<Archive, HashingArchive>::value, void>::type
CEREAL_SERIALIZE_FUNCTION_NAME( Archive & ar, cereal::SizeTag<T> & t )
{
  ar( t.size );
}

//! Hashing binary data
template <class T> inline
void CEREAL_SAVE_FUNCTION_NAME(HashingArchive & ar, cereal::BinaryData<T> const & bd)
{
  ar.Update( bd.data, static_cast<std::size_t>( bd.size ) );
}


} // close namespace detail




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
