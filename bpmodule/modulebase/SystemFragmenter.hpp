/*! \file
 *
 * \brief Base class for a test module (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_MODULEBASE__SYSTEMFRAGMENTER_HPP_
#define BPMODULE_GUARD_MODULEBASE__SYSTEMFRAGMENTER_HPP_

#include "bpmodule/modulebase/ModuleBase.hpp"
#include "bpmodule/system/System.hpp"

namespace bpmodule {
namespace modulebase {

/*! \brief A module that fragments a system
 */
class SystemFragmenter : public ModuleBase
{
    public:
        typedef SystemFragmenter BaseType;


        SystemFragmenter(unsigned long id)
            : ModuleBase(id, "SystemFragmenter")
        { }


        /*! \brief Fragment the system
         */
        system::SystemMap Fragmentize(const system::System & mol)
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
        virtual system::SystemMap Fragmentize_(const system::System & mol) = 0;
};




class SystemFragmenter_Py : public SystemFragmenter
{
    public:
        using SystemFragmenter::SystemFragmenter;

        MODULEBASE_FORWARD_PROTECTED_TO_PY

    
        virtual system::SystemMap Fragmentize_(const system::System & mol)
        {
            return CallPyOverride<system::SystemMap>("Fragmentize_", mol);
        }

};


} // close namespace modulebase
} // close namespace bpmodule

#endif
