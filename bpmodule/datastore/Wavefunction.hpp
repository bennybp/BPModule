/*! \file
 *
 * \brief General wavefunction class
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#ifndef BPMODULE_GUARD_DATASTORE__WAVEFUNCTION_HPP_
#define BPMODULE_GUARD_DATASTORE__WAVEFUNCTION_HPP_

#include <memory>

#include "bpmodule/tensor/Matrix.hpp"
#include "bpmodule/datastore/UIDPointer.hpp"

//Needed for parallel hack, needs tweaked after agreement on how stuff works
//and wrapping of LibTaskForce in python
#include "bpmodule/parallel/InitFinalize.hpp"
#include "LibParallel.hpp"


// forward declarations
namespace bpmodule {
namespace system {
class Molecule;
}
}

namespace bpmodule {
namespace datastore {

class Wavefunction
{
public:
    Wavefunction(void):comm(parallel::Env_->Comm()){}
    Wavefunction(const Wavefunction &) = default;
    Wavefunction & operator=(const Wavefunction &) = default;
    Wavefunction(Wavefunction &&) = default;
    Wavefunction & operator=(Wavefunction &&) = default;

    UIDPointer<system::Molecule> system;
    UIDPointer<LibTaskForce::Communicator> comm;
    UIDPointer<tensor::DistMatrixD> cmat; //! \todo REPLACE ME WITH TENSOR<2> or <3>
    UIDPointer<tensor::DistMatrixD> epsilon; //! \todo REPLACE ME WITH TENSOR<1> or <2>

    std::string UniqueString(void) const
    {
        std::string s = "WFN_";
        s += "M:" + std::to_string(system.Valid()) + "_" + std::to_string(system.UID()) + "_";
        s += "C:" + std::to_string(cmat.Valid()) + "_" + std::to_string(cmat.UID()) + "_";
        s += "E:" + std::to_string(epsilon.Valid()) + "_" + std::to_string(epsilon.UID()) + ";";
        return s;
    }
};


} // close namespace datastore
} // close namespace system

#endif
