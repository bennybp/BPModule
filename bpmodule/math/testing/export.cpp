/*! \file
 *
 * \brief Exports for testing math
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bpmodule/testing/TestSerialization.hpp"
#include "bpmodule/math/Point.hpp"


namespace bpmodule {
namespace math {
namespace export_python {

void export_testing(pybind11::module & m)
{
    using bpmodule::testing::TestSerialization;

    /////////////////////////////////
    // Serialization
    /////////////////////////////////
    m.def("TestSerialization_Point",     &TestSerialization<Point>);
}


} // close namespace export_python
} // close namespace system 
} // close namespace bpmodule
