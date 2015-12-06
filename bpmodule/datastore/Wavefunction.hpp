/*! \file
 *
 * \brief Storage of generic calculation data (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <memory>

#include "bpmodule/molecule/Molecule.hpp"
#include "bpmodule/basisset/BasisSet.hpp"
#include "bpmodule/tensor/Matrix.hpp"

#ifndef _GUARD_WAVEFUNCTION_HPP_
#define _GUARD_WAVEFUNCTION_HPP_

//! \todo forward declare molecule, basis set, matrix?

namespace bpmodule {
namespace datastore {

struct Wavefunction
{
    std::shared_ptr<const basisset::BasisSet> basis; 
    std::shared_ptr<const molecule::Molecule> molecule;
    std::shared_ptr<const tensor::DistMatrixD> cmat; //! \todo REPLACE ME WITH TENSOR
};


} // close namespace datastore
} // close namespace molecule

#endif
