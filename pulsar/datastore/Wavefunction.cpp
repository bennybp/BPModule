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
    bool samesystem = (system.first == rhs.system.first);
    bool samecmat = (cmat.first == rhs.cmat.first);
    bool sameepsilon = (epsilon.first == rhs.epsilon.first);
    bool sameoccupations = (occupations.first == rhs.occupations.first);

    // if the pointers aren't the same, check the 
    // actual data
    if(!samesystem && bool(system.first) && bool(rhs.system.first))
        samesystem = ( (*system.first) == (*rhs.system.first) );

    if(!samecmat && bool(cmat.first) && bool(rhs.cmat.first))
        samecmat = ( (*cmat.first) == (*rhs.cmat.first) );

    if(!sameepsilon && bool(epsilon.first) && bool(rhs.epsilon.first))
        sameepsilon = ( (*epsilon.first) == (*rhs.epsilon.first) );

    if(!sameoccupations && bool(occupations.first) && bool(rhs.occupations.first))
        sameoccupations = ( (*occupations.first) == (*rhs.occupations.first) );

    return (samesystem && samecmat && sameepsilon && sameoccupations);
}


bool Wavefunction::operator!=(const Wavefunction & rhs) const
{
    return !((*this) == rhs);
}

void Wavefunction::hash(util::Hasher & h) const
{
    h(system.first, cmat.first, epsilon.first, occupations.first);
}


util::Hash Wavefunction::MyHash(void) const
{
    return util::MakeHash(*this);
}



} // close namespace datastore
} // close namespace system

