/*! \file
 *
 * \brief Exports for testing data storage
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "pulsar/datastore/OptionMap.hpp"
#include "pulsar/datastore/Wavefunction.hpp"
#include "pulsar/testing/TestSerialization.hpp"


namespace pulsar {
namespace datastore {
namespace export_python {

void export_testing(pybind11::module & m)
{
    using pulsar::testing::TestSerialization;

    /////////////////////////////////
    // Serialization
    /////////////////////////////////
    m.def("TestSerialization_OptionMap",      &TestSerialization<OptionMap>);
    m.def("TestSerialization_Wavefunction",   &TestSerialization<Wavefunction>);
}


} // close namespace export_python
} // close namespace system 
} // close namespace pulsar
