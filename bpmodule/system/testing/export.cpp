#include "bpmodule/python/Pybind11_stl.hpp"
#include "bpmodule/system/BasisShellInfo.hpp"
#include "bpmodule/testing/TestSerialization.hpp"


namespace bpmodule {
namespace system {
namespace export_python {

void export_testing(pybind11::module & m)
{
    using bpmodule::testing::TestSerialization;

    /////////////////////////////////
    // Serialization
    /////////////////////////////////
    m.def("TestSerialization_BasisShellInfo",     &TestSerialization<BasisShellInfo>);
}


} // close namespace export_python
} // close namespace system 
} // close namespace bpmodule
