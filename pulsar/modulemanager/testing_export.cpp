/*! \file
 *
 * \brief Exports for testing data storage
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "pulsar/modulemanager/ModuleInfo.hpp"
#include "pulsar/testing/TestSerialization.hpp"


namespace pulsar{
namespace modulemanager {
namespace export_python {

void export_testing(pybind11::module & m)
{
    using pulsar::testing::TestSerialization;

    /////////////////////////////////
    // Serialization
    /////////////////////////////////
    m.def("TestSerialization_ModuleInfo",     &TestSerialization<ModuleInfo>);
}


} // close namespace export_python
} // close namespace system 
} // close namespace pulsar
