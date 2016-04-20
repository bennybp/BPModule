/*! \file
 *
 * \brief Exports for testing data storage
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bpmodule/modulemanager/ModuleInfo.hpp"
#include "bpmodule/testing/TestSerialization.hpp"


namespace bpmodule {
namespace modulemanager {
namespace export_python {

void export_testing(pybind11::module & m)
{
    using bpmodule::testing::TestSerialization;

    /////////////////////////////////
    // Serialization
    /////////////////////////////////
    m.def("TestSerialization_ModuleInfo",     &TestSerialization<ModuleInfo>);
}


} // close namespace export_python
} // close namespace system 
} // close namespace bpmodule
