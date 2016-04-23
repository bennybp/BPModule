/*! \file
 *
 * \brief A class that hashes objects
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_BPHASH__HASHER_FWD_HPP_
#define PULSAR_GUARD_BPHASH__HASHER_FWD_HPP_


namespace pulsar{
namespace util {

class Hasher;
class Hash;


} // close namespace util
} // close namespace pulsar



#ifndef DECLARE_HASHING_FRIENDS

#define DECLARE_HASHING_FRIENDS \
    friend class pulsar::util::Hasher;

#endif


#endif
