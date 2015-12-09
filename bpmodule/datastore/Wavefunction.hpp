/*! \file
 *
 * \brief General wavefunction class
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#ifndef _GUARD_WAVEFUNCTION_HPP_
#define _GUARD_WAVEFUNCTION_HPP_

#include <memory>

#include "bpmodule/tensor/Matrix.hpp"

// forward declarations
namespace bpmodule {
namespace molecule {
class Molecule;
}

namespace basisset {
class BasisSet;
}
}



namespace bpmodule {
namespace datastore {

class Wavefunction
{
public:
    Wavefunction(const basisset::BasisSet & basis,
                 const molecule::Molecule & molecule,
                 const tensor::DistMatrixD & cmat,
                 const tensor::DistMatrixD & epsilon);

    // these all preserve the id
    Wavefunction(const Wavefunction & rhs) = default;
    Wavefunction & operator=(const Wavefunction & rhs) = default;
    Wavefunction(Wavefunction && rhs) = default;
    Wavefunction & operator=(Wavefunction && rhs) = default;

    // get the data
    const basisset::BasisSet & Basis(void) const { return *basis_; }
    const molecule::Molecule & Molecule(void) const { return *molecule_; }
    const tensor::DistMatrixD & CMat(void) const { return *cmat_; }
    const tensor::DistMatrixD & Epsilon(void) const { return *epsilon_; }

private:
    unsigned long uid_; //unique ID for the wavefunction. Placeholder for hashing

    std::shared_ptr<const basisset::BasisSet> basis_; 
    std::shared_ptr<const molecule::Molecule> molecule_;
    std::shared_ptr<const tensor::DistMatrixD> cmat_; //! \todo REPLACE ME WITH TENSOR
    std::shared_ptr<const tensor::DistMatrixD> epsilon_;
};


} // close namespace datastore
} // close namespace molecule

#endif
