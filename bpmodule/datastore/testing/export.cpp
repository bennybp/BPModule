/*! \file
 *
 * \brief Exports for testing system
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bpmodule/datastore/OptionMap.hpp"
#include "bpmodule/testing/TestSerialization.hpp"


namespace bpmodule {
namespace datastore {
namespace export_python {


// Compare two OptionMaps
struct OptionMapCompare
{
    bool operator()(const OptionMap & lhs, const OptionMap & rhs) const
    {
        return lhs.Compare(rhs);
    }
};


void export_testing(pybind11::module & m)
{
    using bpmodule::testing::TestSerialization;

    /////////////////////////////////
    // Serialization
    /////////////////////////////////
    m.def("TestSerialization_OptionMap",   &TestSerialization<OptionMap, OptionMapCompare>);
}


} // close namespace export_python
} // close namespace system 
} // close namespace bpmodule
