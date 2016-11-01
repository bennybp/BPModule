/*! \file
 *
 * \brief Base class for a test module (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MODULEBASE__SYSTEMFRAGMENTER_HPP_
#define PULSAR_GUARD_MODULEBASE__SYSTEMFRAGMENTER_HPP_

#include "pulsar/modulebase/ModuleBase.hpp"
#include "pulsar/system/System.hpp"

namespace pulsar {

using SNType = std::set<size_t>; //< The type of the serial number

///A struct to hold info associated with an NMer
struct NMerInfo {

    NMerInfo() : sn(), nmer(), weight(1.0)
    {
    }
    ///\todo Do we need the SN in this struct?
    SNType sn; ///< The serial number of the subsystem
    System nmer; ///< The actual subsystem
    double weight; ///< The weight of the nmer
    bool operator==(const NMerInfo& rhs)const{
        return (weight==rhs.weight && sn==rhs.sn && nmer==rhs.nmer);
    }
    bool operator!=(const NMerInfo& rhs)const{
        return !(*this==rhs);
    }
};

using NMerSetType = std::map<SNType, NMerInfo>; //< The type of a set of NMers

///A way to break a system into fragments.  See [here](@ref systemfragger)
class SystemFragmenter : public ModuleBase {
protected:

    /** \brief Function for taking all unions of a set of fragments
     *
     *   For a lot of methods that depend on fragments you need to consider
     *   pairs of fragments, triples of fragments, etc.  This function takes
     *   a SystemMap of fragments and takes all N-way unions of them.
     *
     *   \param[in] Frags The monomers we want the unions of
     *
     *   The truncation order is read from option "TRUNCATION_ORDER" and the
     *   distance threholds are read in from option "DISTANCE_THRESHOLDS"
     *   \param[in] Dist The maximum distance
     */
    NMerSetType make_nmers(const NMerSetType& Frags);

    /////////////////////////////////////////
    // To be implemented by derived classes
    /////////////////////////////////////////
    /*! \copydoc fragmentize
     *
     *   This function should produce the fundamental fragments.
     *   Creation of the n-mers (unions of fragments taken n at a time)
     *   will be handled automatically by the fragmentize function
     *    if requested)
     * \note To be implemented by derived classes
     */
    virtual NMerSetType fragmentize_(const System & mol) = 0;


public:
    typedef SystemFragmenter BaseType;

    SystemFragmenter(ID_t id)
    : ModuleBase(id, "SystemFragmenter")
    {
    }

    /*! \brief Fragment the system
     *
     *   The main call users of this class should call
     */
    NMerSetType fragmentize(const System & mol)
    {
        return ModuleBase::call_function(&SystemFragmenter::fragmentize_, mol);
    }
};

class SystemFragmenter_Py : public SystemFragmenter {
public:
    using SystemFragmenter::SystemFragmenter;

    MODULEBASE_FORWARD_PROTECTED_TO_PY


    virtual NMerSetType fragmentize_(const System & mol)
    {
        return call_py_override<NMerSetType>(this, "fragmentize_", mol);
    }
    
    ///Python won't allow sets to be keys so we stringify it
    std::map<std::string,NMerInfo> fragmentize_py(const System & mol) {
        std::map<std::string,NMerInfo> ret;
        for(auto nmeri: fragmentize(mol)){
            std::stringstream ss;
            for(auto val:nmeri.first)ss<<val<<" ";
            ret.emplace(ss.str(),std::move(nmeri.second));
        }
        return ret;
    }
};

} // close namespace pulsar

#endif
