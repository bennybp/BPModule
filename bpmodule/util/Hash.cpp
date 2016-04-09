/*! \file
 *
 * \brief A class holding a hash of data (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bpmodule/util/Hash.hpp"

namespace bpmodule {
namespace util {

Hash::Hash(uint64_t h1, uint64_t h2)
    : hash_({h1, h2})
{ }


Hash::Hash(void)
    : Hash(0, 0)
{ }


bool Hash::operator==(const Hash & rhs) const
{
    return hash_ == rhs.hash_;
}

bool Hash::operator!=(const Hash & rhs) const
{
    return hash_ != rhs.hash_;
}

bool Hash::operator<(const Hash & rhs) const
{
    return hash_ < rhs.hash_;
}

bool Hash::operator>(const Hash & rhs) const
{
    return hash_ > rhs.hash_;
}

bool Hash::operator>=(const Hash & rhs) const
{
    return hash_ >= rhs.hash_;
}

bool Hash::operator<=(const Hash & rhs) const
{
    return hash_ <= rhs.hash_;
}


std::string Hash::String(void) const
{
    char buf[2*(128/8) + 1]; // 2 chars per byte + null

    const uint8_t * p = reinterpret_cast<const uint8_t *>(hash_.data());

    for(size_t i = 0; i < 128/8; i++)
        snprintf(buf + 2*i, 3, "%02x", p[i]); // max size is 2 + null

    return std::string(buf);
}



} // close namespace util
} // close namespace bpmodule


