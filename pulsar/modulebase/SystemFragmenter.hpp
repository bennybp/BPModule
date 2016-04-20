/*! \file
 *
 * \brief Base class for a test module (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_MODULEBASE__SYSTEMFRAGMENTER_HPP_
#define PULSAR_GUARD_MODULEBASE__SYSTEMFRAGMENTER_HPP_

#include "pulsar/modulebase/ModuleBase.hpp"
#include "pulsar/system/System.hpp"

namespace pulsar{
namespace modulebase {

/*! \brief A module that fragments a system
 */
class SystemFragmenter : public ModuleBase
{
    private:
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
        system::SystemMap MakeNMers(const system::SystemMap& Frags, 
                                     size_t N,double Dist);
    public:
        typedef SystemFragmenter BaseType;


        SystemFragmenter(ID_t id)
            : ModuleBase(id, "SystemFragmenter")
        { }


        /*! \brief Fragment the system
         * 
         *   This function automatically takes care of creating unions
         *   of fragments, if the user requested them.  This is
         *   handy for automation such as that seen in MBE using
         *   MIM.
         */
        system::SystemMap Fragmentize(const system::System & mol);


        /////////////////////////////////////////
        // To be implemented by derived classes
        /////////////////////////////////////////
        /*! \copydoc Fragmentize
         * 
         *   This function should produce the fundamental fragments.
         *   Creation of the n-mers (unions of fragments taken n at a time)
         *   will be handled automatically by the Fragmentize function
         *    if requested)
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
} // close namespace pulsar

#endif
