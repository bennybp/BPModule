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



// forward declarations
namespace bpmodule {
namespace system {
class System;
}
}

namespace bpmodule {
namespace datastore {

class Wavefunction
{
public:
    Wavefunction(void)=default;
    Wavefunction(const Wavefunction &) = default;
    Wavefunction & operator=(const Wavefunction &) = default;
    Wavefunction(Wavefunction &&) = default;
    Wavefunction & operator=(Wavefunction &&) = default;

    UIDPointer<system::System> system;
    UIDPointer<std::vector<double>> cmat; //! \todo REPLACE ME
    UIDPointer<std::vector<double>> epsilon; //! \todo REPLACE ME

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
