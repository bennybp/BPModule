/*! \file
 *
 * \brief Python exports for basis set
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/python/Pybind11.hpp"
#include "bpmodule/basisset/BasisSet.hpp"
#include "bpmodule/basisset/Creators.hpp"
#include "bpmodule/datastore/RegisterUIDPointer.hpp"



namespace bpmodule {
namespace basisset {
namespace export_python {


PYBIND11_PLUGIN(basisset)
{
    pybind11::module m("basisset", "Basis set");

    datastore::RegisterUIDPointer<BasisSet>(m, "BasisSet");

    pybind11::class_<GaussianBasisShell> gbs(m, "GaussianBasisShell");
    gbs.def(pybind11::init<int, bool>())
       .def("AM", &GaussianShell::AM)
       .def("NPrim", &GaussianShell::NPrim)
       .def("NCartesian", &GaussianShell::NCartesian)
       .def("NSpherical", &GaussianShell::NSpherical)
       .def("NFunctions", &GaussianShell::NFunctions)
       .def("IsCartesian", &GaussianShell::IsCartesian)
       .def("IsSpherical", &GaussianShell::IsSpherical)
       .def("Alphas", &GaussianShell::Alphas)
       .def("Coefs", &GaussianShell::Coefs)
       .def("Alpha", &GaussianShell::Alpha)
       .def("Coef", &GaussianShell::Coef)
       .def("AddPrimitive", &GaussianShell::AddPrimitive);


    pybind11::class_<GaussianShell>(m, "GaussianShell", gbs)
    .def(pybind11::init<int, bool, unsigned long, unsigned long, double, double, double>())
    .def(pybind11::init<const GaussianBasisShell &, unsigned long, unsigned long, double, double, double>())
    .def("ID", &GaussianShell::ID)
    .def("Center", &GaussianShell::Center)
    .def("Coordinates", &GaussianShell::Coordinates)
    ;


    pybind11::class_<BasisSet>(m, "BasisSet")
    .def(pybind11::init<>())
    .def("Print", &BasisSet::Print)
    .def("AddShell", &BasisSet::AddShell)
    .def("NShell", &BasisSet::NShell)
    .def("Shell", &BasisSet::Shell)
    .def("NPrim", &BasisSet::NPrim)
    .def("NCartesian", &BasisSet::NCartesian)
    .def("NFunctions", &BasisSet::NFunctions)
    .def("MaxNPrim", &BasisSet::MaxNPrim)
    .def("MaxAM", &BasisSet::MaxAM)
    .def("MaxNCartesian", &BasisSet::MaxNCartesian)
    .def("MaxNFunctions", &BasisSet::MaxNFunctions)
    ;


    // Creators
    m.def("SimpleCreator", bpmodule::basisset::SimpleCreator);

    return m.ptr();
}


} // close namespace export_python
} // close namespace basisset 
} // close namespace bpmodule

