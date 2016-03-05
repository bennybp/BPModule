/*! \file
 *
 * \brief Base class for a test module (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_MODULEBASE__SYSTEMFRAGMENTER_HPP_
#define BPMODULE_GUARD_MODULEBASE__SYSTEMFRAGMENTER_HPP_

#include "bpmodule/modulebase/ModuleBase.hpp"
#include "bpmodule/system/Molecule.hpp"

namespace bpmodule {
namespace modulebase {

/*! \brief A module that fragments a molecule
 */
class SystemFragmenter : public ModuleBase
{
    public:
        typedef SystemFragmenter BaseType;


        SystemFragmenter(unsigned long id)
            : ModuleBase(id, "SystemFragmenter")
        { }


        /*! \brief Fragment the molecule
         */
        system::MoleculeMap Fragmentize(const system::Molecule & mol)
        {
            return ModuleBase::CallFunction(&SystemFragmenter::Fragmentize_, mol);
        }


        /////////////////////////////////////////
        // To be implemented by derived classes
        /////////////////////////////////////////
        /*! \copydoc Fragmentize
         * 
         * \note To be implemented by derived classes
         */ 
        virtual system::MoleculeMap Fragmentize_(const system::Molecule & mol) = 0;
};




class SystemFragmenter_Py : public SystemFragmenter
{
    public:
        using SystemFragmenter::SystemFragmenter;

        MODULEBASE_FORWARD_PROTECTED_TO_PY

    
        virtual system::MoleculeMap Fragmentize_(const system::Molecule & mol)
        {
            return CallPyOverride<system::MoleculeMap>("Fragmentize_", mol);
        }

};


} // close namespace modulebase
} // close namespace bpmodule

#endif
