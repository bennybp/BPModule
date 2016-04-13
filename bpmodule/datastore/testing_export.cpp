/*! \file
 *
 * \brief Exports for testing data storage
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bpmodule/datastore/OptionMap.hpp"
#include "bpmodule/datastore/Wavefunction.hpp"
#include "bpmodule/testing/TestSerialization.hpp"


namespace bpmodule {
namespace datastore {
namespace export_python {


// Compare two OptionMaps
struct OptionMapCompare
{
    bool operator()(const OptionMap & lhs, const OptionMap & rhs) const
    {
        return lhs.Compare(rhs);
    }
};


// Compare two Wavefunctions
// (we do it this way since system has a mathset)
//! \todo replace with System compare function? Same with mathset
struct WavefunctionCompare
{
    bool operator()(const Wavefunction & lhs, const Wavefunction & rhs) const
    {
        // check for pointer equivalence first
        bool samesystem = (lhs.system == rhs.system);
        bool samecmat = (lhs.cmat == rhs.cmat);
        bool sameepsilon = (lhs.epsilon == rhs.epsilon);

        // if the pointers aren't the same, check the 
        // actual data
        if(!samesystem && bool(lhs.system) && bool(rhs.system))
            if (lhs.system->Size() != rhs.system->Size() &&
                !std::equal(lhs.system->begin(), lhs.system->end(), rhs.system->begin()))
                return false;

        if(!samecmat && bool(lhs.cmat) && bool(rhs.cmat))
            if( (*lhs.cmat) != (*rhs.cmat) )
                return false;

        if(!sameepsilon && bool(lhs.epsilon) && bool(rhs.epsilon))
            if( (*lhs.epsilon) != (*rhs.epsilon) )
                return false;
                
        return true;
    }
};


void export_testing(pybind11::module & m)
{
    using bpmodule::testing::TestSerialization;

    /////////////////////////////////
    // Serialization
    /////////////////////////////////
    m.def("TestSerialization_OptionMap",      &TestSerialization<OptionMap, OptionMapCompare>);
    m.def("TestSerialization_Wavefunction",   &TestSerialization<Wavefunction, WavefunctionCompare>);
}


} // close namespace export_python
} // close namespace system 
} // close namespace bpmodule
