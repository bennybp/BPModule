/*! \file
 *
 * \brief General wavefunction class (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <memory>

#include "pulsar/datastore/Wavefunction.hpp"

namespace pulsar{
namespace datastore {

bool Wavefunction::operator==(const Wavefunction & rhs) const
{
    // check for pointer equivalence first
    bool samesystem = (system == rhs.system);
    bool samecmat = (cmat == rhs.cmat);
    bool sameepsilon = (epsilon == rhs.epsilon);
    bool sameoccupations = (occupations == rhs.occupations);
    bool sameopdm = (opdm == rhs.opdm);

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

    if(!sameopdm && bool(opdm) && bool(rhs.opdm))
        sameopdm = ( (*opdm) == (*rhs.opdm) );

    return (samesystem && samecmat && sameepsilon && sameoccupations && sameopdm);
}


bool Wavefunction::operator!=(const Wavefunction & rhs) const
{
    return !((*this) == rhs);
}

void Wavefunction::hash(bphash::Hasher & h) const
{
    h(system, cmat, epsilon, occupations);
}


bphash::HashValue Wavefunction::MyHash(void) const
{
    return bphash::MakeHash(bphash::HashType::Hash128, *this);
}



} // close namespace datastore
} // close namespace system

