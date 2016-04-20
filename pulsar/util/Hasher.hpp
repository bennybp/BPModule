/*! \file
 *
 * \brief A class for hashing data (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_UTIL__HASHER_HPP_
#define PULSAR_GUARD_UTIL__HASHER_HPP_

#include <cstdint>
#include <memory>
#include <istream>

namespace pulsar{
namespace util {


// forward declare
class Hash;


/*! \brief A class that hashes arbitrary data
 *
 * This class can be used to progressively hash
 * arbitrary data (via the Update function). The
 * results are obtained via the Finalize function,
 * which returns a Hash object.
 *
 * Currently, the hash algorithm generates 128-bit
 * hashes.
 */
class Hasher
{
    private:
        static const uint64_t c1 = (0x87c37b91114253d5LLU);
        static const uint64_t c2 = (0x4cf5ad432745937fLLU);

        uint64_t h1; //!< First part of the 128-bit hash
        uint64_t h2; //!< Second part of the 128-bit hash

        std::array<uint8_t, 16> buffer_;   //!< Buffer for updating blocks, and any left over

        unsigned short nbuffer_;           //!< Number of elements in the buffer
        size_t len_;                       //!< Total amount already hashed

        /*! \brief Hash 16 bytes of data in buffer_
         * 
         * This does NOT handle any tail/remainder
         */
        void UpdateBlock_(void);

    public:
        Hasher(void);
        ~Hasher(void) = default;

        Hasher(const Hasher &) = default;
        Hasher & operator=(const Hasher &) = default;
        Hasher(Hasher &&) = default;
        Hasher & operator=(Hasher &&) = default;

        /*! \brief Add some data to the hash
         * 
         * Any remaining from the previous call will be done, and
         * any that doesn't fill out a whole block will be stored
         * until next time
         */
        void Update(void const * buffer, size_t size);

        /*! \brief Add all data from a stream to the hash
         * 
         * Any remaining from the previous call will be done, and
         * any that doesn't fill out a whole block will be stored
         * until next time
         *
         * The internal positions of the stream are returned to where they
         * originally were after the hashing operation.
         */
        void Update(std::istream & is);

        /*! \brief Finish hashing and report the hash
         *
         * Any remaining data (that doesn't fill out a block) will be done.
         * Then, any finalization steps will be done and the hash returned. 
         */
        Hash Finalize(void);

        /*! \brief Zero out the hash
         * 
         * After this, you can start hashing something else again
         */
        void Reset(void);
};


} // close namespace util
} // close namespace pulsar

#endif
