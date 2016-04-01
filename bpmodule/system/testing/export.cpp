/*! \file
 *
 * \brief Exports for testing system
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bpmodule/system/System.hpp"
#include "bpmodule/testing/TestSerialization.hpp"


namespace bpmodule {
namespace system {
namespace export_python {


// Compare two Systems elementwise
//! \todo Maybe replace if able to get universes from mathset. This is ugly
struct SystemCompare
{
    bool operator()(const System & lhs, const System & rhs) const
    {
        if(lhs.Size() != rhs.Size())
            return false;
        if(lhs.CompareInfo(rhs) == false)
            return false;

        auto it = lhs.begin();
        auto it2 = rhs.begin();
        for(; it != lhs.end(); ++it, ++it2)
        {
            if(*it != *it2)
                return false;
        }

        return true;
    }
};

void export_testing(pybind11::module & m)
{
    using bpmodule::testing::TestSerialization;

    /////////////////////////////////
    // Serialization
    /////////////////////////////////
    m.def("TestSerialization_BasisShellInfo",   &TestSerialization<BasisShellInfo>);
    m.def("TestSerialization_Atom",             &TestSerialization<Atom>);
    m.def("TestSerialization_System",           &TestSerialization<System, SystemCompare>);
}


} // close namespace export_python
} // close namespace system 
} // close namespace bpmodule
