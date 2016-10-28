/*! \file
 *
 * \brief Exports for testing data storage
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "pulsar/datastore/OptionMap.hpp"
#include "pulsar/datastore/Wavefunction.hpp"
#include "pulsar/testing/TestSerialization.hpp"


namespace pulsar {

void export_testing_datastore(pybind11::module & m)
{

    /////////////////////////////////
    // Serialization
    /////////////////////////////////
    m.def("TestSerialization_OptionMap",      &TestSerialization<OptionMap>);
    m.def("TestSerialization_Wavefunction",   &TestSerialization<Wavefunction>);
}

} // close namespace pulsar
