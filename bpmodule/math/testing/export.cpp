/*! \file
 *
 * \brief Exports for testing math
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bpmodule/testing/TestSerialization.hpp"
#include "bpmodule/math/Point.hpp"
#include "bpmodule/math/RegisterMathSet.hpp"


namespace bpmodule {
namespace math {
namespace export_python {


// Compare two MathSets elementwise
//! \todo Maybe replace if able to get universes from mathset. This is ugly
template<typename T>
struct MathSetCompare
{
    bool operator()(const MathSet<T> & lhs, const MathSet<T> & rhs) const
    {
        if(lhs.Size() != rhs.Size())
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

    // For testing
    RegisterUniverse<Universe<std::string>>(m, "StringSetUniverse"); 
    RegisterMathSet<MathSet<std::string>>(m, "StringSet"); 

    /////////////////////////////////
    // Serialization
    /////////////////////////////////
    m.def("TestSerialization_Point",             &TestSerialization<Point>);
    m.def("TestSerialization_StringSetUniverse", &TestSerialization<Universe<std::string>>);
    m.def("TestSerialization_StringSet",         &TestSerialization<MathSet<std::string>, MathSetCompare<std::string>>);
}


} // close namespace export_python
} // close namespace system 
} // close namespace bpmodule
