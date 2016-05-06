/*! \file
 *
 * \brief General wavefunction class (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <memory>

#include "pulsar/datastore/Wavefunction.hpp"
#include "pulsar/util/bphash/Hasher.hpp"

namespace pulsar{
namespace datastore {

bool Wavefunction::operator==(const Wavefunction & rhs) const
{
    // check for pointer equivalence first
    bool samesystem = (system == rhs.system);
    bool samecmat = (cmat == rhs.cmat);
    bool sameepsilon = (epsilon == rhs.epsilon);
    bool sameoccupations = (occupations == rhs.occupations);

    // if the pointers aren't the same, check the 
    // actual data
    if(!samesystem && bool(system) && bool(rhs.system))
        samesystem = ( (*system) == (*rhs.system) );

    if(!samecmat && bool(cmat) && bool(rhs.cmat))
        samecmat = ( (*cmat) == (*rhs.cmat) );

    if(!sameepsilon && bool(epsilon) && bool(rhs.epsilon))
        sameepsilon = ( (*epsilon) == (*rhs.epsilon) );

    if(!sameoccupations && bool(occupations) && bool(rhs.occupations))
        sameoccupations = ( (*occupations) == (*rhs.occupations) );

    return (samesystem && samecmat && sameepsilon);
}


void Wavefunction::hash(util::Hasher & h) const
{
    h(system, cmat, epsilon, occupations);
}


util::Hash Wavefunction::MyHash(void) const
{
    return util::MakeHash(*this);
}


} // close namespace datastore
} // close namespace system

