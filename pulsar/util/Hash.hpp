/*! \file
 *
 * \brief A class holding a hash of data (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_UTIL__HASH_HPP_
#define PULSAR_GUARD_UTIL__HASH_HPP_

#include <cstdint>
#include <memory>
#include <string>

namespace pulsar{
namespace util {

// forward declare for friend
class Hasher;


/*! \brief A hash of some data
 *
 * The length is left unspecified to the end user, but
 * is currently 128-bit.
 *
 * A hash can only be constructed from within a Hasher object
 */
class Hash
{
    public:
        Hash(const Hash &) = default;
        Hash & operator=(const Hash &) = default;
        Hash(Hash &&) = default;
        Hash & operator=(Hash &&) = default;

        bool operator==(const Hash & rhs) const;

        bool operator!=(const Hash & rhs) const;

        bool operator<(const Hash & rhs) const;

        bool operator>(const Hash & rhs) const;

        bool operator>=(const Hash & rhs) const;

        bool operator<=(const Hash & rhs) const;


        /*! \brief Return a string representation of the hash
         *
         * The string representation is the usual hex representation,
         * with lower case letters. The length of the string
         * should be 2*bits/8 characters.
         */
        std::string String(void) const;


    private:
        friend class Hasher;

        std::array<uint64_t, 2> hash_; //!< The hash as a number


        /// Construct as a hash of zeros
        Hash();

        /*! \brief Construct given the lower and upper 64-bit parts */
        Hash(uint64_t h1, uint64_t h2);
};


} // close namespace util
} // close namespace pulsar


#endif

