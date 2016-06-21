/*! \file
 *
 * \brief Exports for testing math
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "pulsar/testing/TestSerialization.hpp"
#include "pulsar/math/Point.hpp"
#include "pulsar/math/Reorder.hpp"
#include "pulsar/math/RegisterMathSet.hpp"
#include "pulsar/math/IrrepSpinMatrix.hpp"

// Needed for the string mathset test
#include "bphash/types/string.hpp"

namespace pulsar{
namespace math {
namespace export_python {


// Compare two MathSets elementwise
template<typename T>
struct MathSetCompare
{
    bool operator()(const MathSet<T> & lhs, const MathSet<T> & rhs) const
    {
        return (lhs.size() == rhs.size() &&
                std::equal(lhs.begin(), lhs.end(), rhs.begin()));
    }
};


std::vector<double> Test_Reorder(const std::vector<size_t> & neworder,
                                 const std::vector<double> & src,
                                 size_t width, size_t niter)
{
    std::vector<double> ret(src.size());
    reorder_block(neworder, src.data(), ret.data(), width, niter);
    return ret;
}
                              


void export_testing(pybind11::module & m)
{
    using pulsar::testing::TestSerialization;

    /////////////////////////
    // MathSet and Universe
    /////////////////////////
    register_Universe<Universe<std::string>>(m, "StringSetUniverse"); 
    register_MathSet<MathSet<std::string>>(m, "StringSet"); 

    /////////////////////////
    // Reordering
    /////////////////////////
    m.def("Test_Reorder", &Test_Reorder);
    m.def("Test_MakeOrdering", &make_ordering<int>);
    

    /////////////////////////////////
    // Serialization
    /////////////////////////////////
    m.def("TestSerialization_Point",             &TestSerialization<Point>);
    m.def("TestSerialization_StringSetUniverse", &TestSerialization<Universe<std::string>>);
    m.def("TestSerialization_StringSet",         &TestSerialization<MathSet<std::string>, MathSetCompare<std::string>>);
    /*
    m.def("TestSerialization_SimpleMatrixF",     &TestSerialization<SimpleMatrixF>);
    m.def("TestSerialization_SimpleMatrixD",     &TestSerialization<SimpleMatrixD>);
    m.def("TestSerialization_IrrepSpinMatrixF",  &TestSerialization<IrrepSpinMatrixF>);
    m.def("TestSerialization_IrrepSpinMatrixD",  &TestSerialization<IrrepSpinMatrixD>);
    m.def("TestSerialization_SimpleVectorF",     &TestSerialization<SimpleVectorF>);
    m.def("TestSerialization_SimpleVectorD",     &TestSerialization<SimpleVectorD>);
    m.def("TestSerialization_IrrepSpinVectorF",  &TestSerialization<IrrepSpinVectorF>);
    m.def("TestSerialization_IrrepSpinVectorD",  &TestSerialization<IrrepSpinVectorD>);
    */
}


} // close namespace export_python
} // close namespace math 
} // close namespace pulsar
