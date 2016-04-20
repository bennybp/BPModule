/*! \file
 *
 * \brief Exports for testing system
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "pulsar/system/System.hpp"
#include "pulsar/system/BasisSet.hpp"
#include "pulsar/testing/TestSerialization.hpp"


namespace pulsar{
namespace system {
namespace export_python {


// Compare two Systems elementwise
//! \todo Maybe replace if able to get universes from mathset. This is ugly
struct SystemCompare
{
    bool operator()(const System & lhs, const System & rhs) const
    {
        return (lhs.Size() == rhs.Size() &&
                lhs.CompareInfo(rhs) &&
                std::equal(lhs.begin(), lhs.end(), rhs.begin()));
    }
};

void export_testing(pybind11::module & m)
{
    using pulsar::testing::TestSerialization;

    /////////////////////////////////
    // Serialization
    /////////////////////////////////
    m.def("TestSerialization_BasisShellInfo",   &TestSerialization<BasisShellInfo>);
    m.def("TestSerialization_Atom",             &TestSerialization<Atom>);
    m.def("TestSerialization_System",           &TestSerialization<System, SystemCompare>);
    m.def("TestSerialization_BasisSet",         &TestSerialization<BasisSet>);
}


} // close namespace export_python
} // close namespace system 
} // close namespace pulsar
