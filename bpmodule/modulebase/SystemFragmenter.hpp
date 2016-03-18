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

/** \brief Function for taking all unions of a set of fragments
 * 
 *   For a lot of methods that depend on fragments you need to consider
 *   pairs of fragments, triples of fragments, etc.  This function takes
 *   a SystemMap of fragments and takes all N-way unions of them.  Optionally,
 *   distance cut-offs may be applied to screen fragments.  This is applied as
 *   follows, for each fragment in the N-mer the
 * 
 *   \param[in] Frags The monomers we want the unions of
 *   \param[in] N How many ways the union is
 *   \param[in] Dist The maximum distance 
 */
system::SystemMap MakeNMers(const system::SystemMap& Frags, size_t N,double Dist);

} // close namespace modulebase
} // close namespace bpmodule

#endif
