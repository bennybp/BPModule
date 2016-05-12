/*! \file
 *
 * \brief An iteration in an SCF procedure
 */ 


#ifndef PULSAR_GUARD_MODULEBASE__FOCKBUILDER_HPP_
#define PULSAR_GUARD_MODULEBASE__FOCKBUILDER_HPP_

#include "pulsar/modulebase/ModuleBase.hpp"
#include "pulsar/math/IrrepSpinMatrix.hpp"


namespace pulsar{
namespace modulebase {

class FockBuilder : public ModuleBase
{   
    public:
        typedef FockBuilder BaseType;
        
        FockBuilder(ID_t id): ModuleBase(id, "FockBuilder") { }
        
        /*! \brief Form a new wavefunction based on the given wfn and fock matrix
         */ 
        math::IrrepSpinMatrixD Build(const datastore::Wavefunction & wfn)
        {
            return ModuleBase::CallFunction(&FockBuilder::Build_, wfn);
        }


        virtual math::IrrepSpinMatrixD Build_(const datastore::Wavefunction & wfn) = 0;
        
};

class FockBuilder_Py : public FockBuilder{
    public:
        using FockBuilder::FockBuilder;
        MODULEBASE_FORWARD_PROTECTED_TO_PY
                
        virtual math::IrrepSpinMatrixD Build_(const datastore::Wavefunction & wfn)
        {
            return CallPyOverride<math::IrrepSpinMatrixD>("Build_", wfn);
        }    
};

} // close namespace modulebase
} // close namespace pulsar

#endif
