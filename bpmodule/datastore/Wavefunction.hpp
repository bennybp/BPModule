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

class Wavefunction
{
public:
    Wavefunction(void)=default;
    Wavefunction(const Wavefunction &) = default;
    Wavefunction & operator=(const Wavefunction &) = default;
    Wavefunction(Wavefunction &&) = default;
    Wavefunction & operator=(Wavefunction &&) = default;

    std::shared_ptr<system::System> GetSystem(void) const
    {
        if(!system)
            throw bpmodule::exception::DataStoreException("Attempting to get null pointer for system inside wavefunction");
        return system;
    }
 
    void SetSystem(std::shared_ptr<system::System> & sysptr)
    {
        system = sysptr;
    }

    void CopySystem(const system::System & sys)
    {
        system = std::make_shared<system::System>(sys);
    }

    void TakeSystem(system::System && sys)
    { 
        system = std::make_shared<system::System>(std::move(sys));
    }


    std::string UniqueString(void) const
    {
        std::string s = "WFN_TODO";
        //s += "M:" + std::to_string(system.Valid()) + "_" + std::to_string(system.UID()) + "_";
        //s += "C:" + std::to_string(cmat.Valid()) + "_" + std::to_string(cmat.UID()) + "_";
        //s += "E:" + std::to_string(epsilon.Valid()) + "_" + std::to_string(epsilon.UID()) + ";";
        return s;
    }

    private:
        std::shared_ptr<system::System> system;
        std::shared_ptr<const std::vector<double>> cmat; //! \todo REPLACE ME
        std::shared_ptr<const std::vector<double>> epsilon; //! \todo REPLACE ME

};


} // close namespace datastore
} // close namespace system

#endif
