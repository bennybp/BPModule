/*! \file
 *
 * \brief General wavefunction class
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#ifndef BPMODULE_GUARD_DATASTORE__WAVEFUNCTION_HPP_
#define BPMODULE_GUARD_DATASTORE__WAVEFUNCTION_HPP_

#include <memory>

#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/tensor/Matrix.hpp"
#include "bpmodule/system/System.hpp"


namespace bpmodule {
namespace datastore {

struct Wavefunction
{
    std::shared_ptr<const system::System> system;
    std::shared_ptr<const std::vector<double>> cmat; //! \todo REPLACE ME
    std::shared_ptr<const std::vector<double>> epsilon; //! \todo REPLACE ME

    std::string UniqueString(void) const
    {
        std::string s = "WFN_TODO";
        //s += "M:" + std::to_string(system.Valid()) + "_" + std::to_string(system.UID()) + "_";
        //s += "C:" + std::to_string(cmat.Valid()) + "_" + std::to_string(cmat.UID()) + "_";
        //s += "E:" + std::to_string(epsilon.Valid()) + "_" + std::to_string(epsilon.UID()) + ";";
        return s;
    }
};


} // close namespace datastore
} // close namespace system

#endif
