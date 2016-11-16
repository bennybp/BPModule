/*! \file
 *
 * \brief Exports for testing system
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "pulsar/system/System.hpp"
#include "pulsar/system/BasisSet.hpp"
#include "pulsar/system/SphericalTransform.hpp"
#include "pulsar/testing/TestSerialization.hpp"



namespace pulsar{

// Compare two Systems elementwise
//! \todo Maybe replace if able to get universes from mathset. This is ugly
struct SystemCompare
{
    bool operator()(const System & lhs, const System & rhs) const
    {
        return (lhs.size() == rhs.size() &&
                lhs.compare_info(rhs) &&
                std::equal(lhs.begin(), lhs.end(), rhs.begin()));
    }
};


std::vector<double> Test_spherical_transform_block(const SphericalTransformCoefs & coefs,
                                                 const std::vector<double> & src,
                                                 size_t width, int am, size_t niter)
{
    size_t size = width * niter * n_spherical_gaussian(am);
    std::vector<double> ret(size, 0.0);
    spherical_transform_block(coefs, src.data(), ret.data(), width, am, niter);
    return ret;
}


void export_testing_system(pybind11::module & m)
{
    using pulsar::TestSerialization;

    /////////////////////////////////
    // Serialization
    /////////////////////////////////
    m.def("TestSerialization_BasisShellInfo",   &TestSerialization<BasisShellInfo>);
    m.def("TestSerialization_Atom",             &TestSerialization<Atom>);
    m.def("TestSerialization_System",           &TestSerialization<System, SystemCompare>);
    m.def("TestSerialization_BasisSet",         &TestSerialization<BasisSet>);


    /////////////////////////////////
    // Ordering & Spherical transformation
    /////////////////////////////////
    m.def("Test_spherical_transform_block", &Test_spherical_transform_block);
}

} // close namespace pulsar
