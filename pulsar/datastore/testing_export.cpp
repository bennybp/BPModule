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


void export_testing(pybind11::module & m)
{
    using bpmodule::testing::TestSerialization;

    /////////////////////////////////
    // Serialization
    /////////////////////////////////
    m.def("TestSerialization_OptionMap",      &TestSerialization<OptionMap>);
    m.def("TestSerialization_Wavefunction",   &TestSerialization<Wavefunction>);
}


} // close namespace export_python
} // close namespace system 
} // close namespace bpmodule
