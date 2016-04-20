/*! \file
 *
 * \brief Exports for testing data storage
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "pulsar/datastore/OptionMap.hpp"
#include "pulsar/datastore/Wavefunction.hpp"
#include "pulsar/testing/TestSerialization.hpp"


namespace pulsar{
namespace datastore {
namespace export_python {

// Compare two Wavefunctions
struct WavefunctionCompare
{
    bool operator()(const Wavefunction & lhs, const Wavefunction & rhs) const
    {
        if(lhs == rhs)
            return true;
        else
        {
            // check for pointer equivalence first
            bool samesystem = (lhs.system == rhs.system);
            bool samecmat = (lhs.cmat == rhs.cmat);
            bool sameepsilon = (lhs.epsilon == rhs.epsilon);

            // if the pointers aren't the same, check the 
            // actual data
            if(!samesystem && bool(lhs.system) && bool(rhs.system)
                && lhs.system->Size() == rhs.system->Size())
                samesystem = std::equal(lhs.system->begin(), lhs.system->end(),
                                        rhs.system->begin()); 

            if(!samecmat && bool(lhs.cmat) && bool(rhs.cmat))
                samecmat = ( (*lhs.cmat) == (*rhs.cmat) );

            if(!sameepsilon && bool(lhs.epsilon) && bool(rhs.epsilon))
                sameepsilon = ( (*lhs.epsilon) == (*rhs.epsilon) );
                    
            return (samesystem && samecmat && sameepsilon);
        }
    }
};

void export_testing(pybind11::module & m)
{
    using pulsar::testing::TestSerialization;

    /////////////////////////////////
    // Serialization
    /////////////////////////////////
    m.def("TestSerialization_OptionMap",      &TestSerialization<OptionMap>);
    m.def("TestSerialization_Wavefunction",   &TestSerialization<Wavefunction, WavefunctionCompare>);
}


} // close namespace export_python
} // close namespace system 
} // close namespace pulsar
