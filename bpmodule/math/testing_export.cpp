/*! \file
 *
 * \brief Exports for testing math
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bpmodule/testing/TestSerialization.hpp"
#include "bpmodule/math/Point.hpp"
#include "bpmodule/math/RegisterMathSet.hpp"
#include "bpmodule/math/IrrepSpinMatrix.hpp"


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
        return (lhs.Size() == rhs.Size() &&
                std::equal(lhs.begin(), lhs.end(), rhs.begin()));
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
    m.def("TestSerialization_SimpleMatrixF",     &TestSerialization<SimpleMatrixF>);
    m.def("TestSerialization_SimpleMatrixD",     &TestSerialization<SimpleMatrixD>);
    m.def("TestSerialization_IrrepSpinMatrixF",  &TestSerialization<IrrepSpinMatrixF>);
    m.def("TestSerialization_IrrepSpinMatrixD",  &TestSerialization<IrrepSpinMatrixD>);
    m.def("TestSerialization_SimpleVectorF",     &TestSerialization<SimpleVectorF>);
    m.def("TestSerialization_SimpleVectorD",     &TestSerialization<SimpleVectorD>);
    m.def("TestSerialization_IrrepSpinVectorF",  &TestSerialization<IrrepSpinVectorF>);
    m.def("TestSerialization_IrrepSpinVectorD",  &TestSerialization<IrrepSpinVectorD>);
}


} // close namespace export_python
} // close namespace math 
} // close namespace bpmodule
