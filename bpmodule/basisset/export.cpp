/*! \file
 *
 * \brief Python exports for basis set
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/python/Pybind11.hpp"
#include "bpmodule/python/Pybind11_functional.hpp"
#include "bpmodule/basisset/BasisSet.hpp"
#include "bpmodule/basisset/BasisShellInfo.hpp"
#include "bpmodule/basisset/Creators.hpp"
#include "bpmodule/datastore/RegisterUIDPointer.hpp"



namespace bpmodule {
namespace basisset {
namespace export_python {


PYBIND11_PLUGIN(basisset)
{
    pybind11::module m("basisset", "Basis set");

    datastore::RegisterUIDPointer<BasisSet>(m, "BasisSet");

    // Enumeration for basis set shell types
    pybind11::enum_<ShellType>(m, "ShellType")
    .value("Gaussian", ShellType::Gaussian)
    .value("Slater", ShellType::Slater)
    ;


    // BasisShellInfo class
    pybind11::class_<BasisShellInfo> bshell(m, "BasisShellInfo");
    bshell.def(pybind11::init<ShellType, int, bool>())
          .def("GetType", &BasisShellInfo::GetType)
          .def("AM", &BasisShellInfo::AM)
          .def("NPrim", &BasisShellInfo::NPrim)
          .def("NCartesian", &BasisShellInfo::NCartesian)
          .def("NSpherical", &BasisShellInfo::NSpherical)
          .def("NFunctions", &BasisShellInfo::NFunctions)
          .def("IsCartesian", &BasisShellInfo::IsCartesian)
          .def("IsSpherical", &BasisShellInfo::IsSpherical)
          .def("Alphas", &BasisShellInfo::Alphas)
          .def("Coefs", &BasisShellInfo::Coefs)
          .def("Alpha", &BasisShellInfo::Alpha)
          .def("Coef", &BasisShellInfo::Coef)
          .def("AddPrimitive", &BasisShellInfo::AddPrimitive);

    // BasisSetShell class
    pybind11::class_<BasisSetShell>(m, "BasisSetShell", bshell)
    .def(pybind11::init<ShellType, int, bool, unsigned long, unsigned long, double, double, double>())
    .def(pybind11::init<const BasisShellInfo &, unsigned long, unsigned long, double, double, double>())
    .def(pybind11::init<ShellType, int, bool, unsigned long, unsigned long, std::array<double, 3>>())
    .def(pybind11::init<const BasisShellInfo &, unsigned long, unsigned long, std::array<double, 3>>())
    .def("ID", &BasisSetShell::ID)
    .def("Center", &BasisSetShell::Center)
    .def("GetCoords", &BasisSetShell::GetCoords)
    ;

    pybind11::class_<BasisSet>(m, "BasisSet")
    .def(pybind11::init<>())
    .def("Print", &BasisSet::Print)
    .def("AddShell", &BasisSet::AddShell)
    .def("NShell", &BasisSet::NShell)
    .def("GetShell", &BasisSet::GetShell, pybind11::return_value_policy::reference_internal)
    .def("NPrim", &BasisSet::NPrim)
    .def("NCartesian", &BasisSet::NCartesian)
    .def("NFunctions", &BasisSet::NFunctions)
    .def("MaxNPrim", &BasisSet::MaxNPrim)
    .def("MaxAM", &BasisSet::MaxAM)
    .def("MaxNCartesian", &BasisSet::MaxNCartesian)
    .def("MaxNFunctions", &BasisSet::MaxNFunctions)
    .def("Transform", &BasisSet::Transform)
    ;


    // Creators
    m.def("SimpleCreator", bpmodule::basisset::SimpleCreator);

    return m.ptr();
}


} // close namespace export_python
} // close namespace basisset 
} // close namespace bpmodule

