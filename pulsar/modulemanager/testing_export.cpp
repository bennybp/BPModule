/*! \file
 *
 * \brief Exports for testing data storage
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "pulsar/modulemanager/ModuleInfo.hpp"
#include "pulsar/testing/TestSerialization.hpp"


namespace pulsar{

void export_testing_modulemanager(pybind11::module & m)
{
    using pulsar::TestSerialization;

    /////////////////////////////////
    // Serialization
    /////////////////////////////////
    m.def("TestSerialization_ModuleInfo",     &TestSerialization<ModuleInfo>);
}


} // close namespace pulsar
