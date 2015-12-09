/*! \file
 *
 * \brief General wavefunction class
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#ifndef _GUARD_WAVEFUNCTION_HPP_
#define _GUARD_WAVEFUNCTION_HPP_

#include <memory>

#include "bpmodule/tensor/Matrix.hpp"
#include "bpmodule/datastore/UIDPointer.hpp"


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
    Wavefunction(void) = default;
    Wavefunction(const Wavefunction & rhs) = default;
    Wavefunction & operator=(const Wavefunction & rhs) = default;
    Wavefunction(Wavefunction && rhs) = default;
    Wavefunction & operator=(Wavefunction && rhs) = default;

    UIDPointer<basisset::BasisSet> basis; 
    UIDPointer<molecule::Molecule> molecule;
    UIDPointer<tensor::DistMatrixD> cmat; //! \todo REPLACE ME WITH TENSOR<2> or <3>
    UIDPointer<tensor::DistMatrixD> epsilon; //! \todo REPLACE ME WITH TENSOR<1> or <2>
};


} // close namespace datastore
} // close namespace molecule

#endif
