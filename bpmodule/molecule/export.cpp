/*! \file
 *
 * \brief Python exports for the molecule library
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/python_helper/Convert.hpp"
#include "bpmodule/molecule/AtomicInfo.hpp"
#include "bpmodule/molecule/Molecule.hpp"
#include "bpmodule/datastore/RegisterUIDPointer.hpp"
#include "bpmodule/python_helper/Convert.hpp"
#include "bpmodule/output/Output.hpp"

using bpmodule::python_helper::CppToPyExport;
using bpmodule::python_helper::ConvertToCpp2;

namespace bpmodule {
namespace molecule {
namespace export_python {


// Ditto for atom XYZ
std::array<double, 3> AtomXYZ_(const Atom & a) { return a.xyz; }



// Molecule::AddAtom wrapper
void Molecule_AddAtomWrapper(Molecule & mol, int Z, pybind11::tuple xyz)
{
    //! \todo check for length
    //! \todo pybind11 doesn't convert to std::array?
    auto tmp = ConvertToCpp2<std::array<double, 3>>(xyz);
    return mol.AddAtom(Z, {tmp[0], tmp[1], tmp[2]});
}


PYBIND11_PLUGIN(molecule)
{
    pybind11::module m("molecule", "Molecular system specification");

    //datastore::RegisterUIDPointer<Molecule>("Molecule");


    pybind11::class_<IsotopeData>(m, "IsotopeData")
    .def_readonly("isonum", &IsotopeData::isonum)
    .def_readonly("mass", &IsotopeData::mass)
    .def_readonly("mass_low", &IsotopeData::mass_low)
    .def_readonly("mass_high", &IsotopeData::mass_high)
    .def_readonly("abund", &IsotopeData::abund)
    .def_readonly("abund_low", &IsotopeData::abund_low)
    .def_readonly("abund_high", &IsotopeData::abund_high)
    ;


    pybind11::class_<AtomicData>(m, "AtomicData")
    .def_readonly("Z", &AtomicData::Z)
    .def_readonly("sym", &AtomicData::sym)
    .def_readonly("name", &AtomicData::name)
    .def_readonly("mass", &AtomicData::mass)
    .def_readonly("mass_low", &AtomicData::mass_low)
    .def_readonly("mass_high", &AtomicData::mass_high)
    .def_readonly("isotopes", &AtomicData::isotopes)
    ;


    // Free functions
    m.def("AtomicInfoFromZ", AtomicInfoFromZ, pybind11::return_value_policy::copy);
    m.def("AtomicInfoFromSym", AtomicInfoFromSym, pybind11::return_value_policy::copy);
    m.def("IsotopeInfoFromZ", IsotopeInfoFromZ, pybind11::return_value_policy::copy);
    m.def("IsotopeInfoFromSym", IsotopeInfoFromSym, pybind11::return_value_policy::copy);
    m.def("MostCommonIsotopeFromZ", MostCommonIsotopeFromZ);
    m.def("MostCommonIsotopeFromSym", MostCommonIsotopeFromSym);

    m.def("AtomicMassFromZ", AtomicMassFromZ);
    m.def("AtomicMassFromSym", AtomicMassFromSym);
    m.def("IsotopeMassFromZ", IsotopeMassFromZ);
    m.def("IsotopeMassFromSym", IsotopeMassFromSym);
    m.def("AtomicSymFromZ", AtomicSymFromZ);
    m.def("AtomicZNumberFromSym", AtomicZNumberFromSym);
    m.def("AtomicNameFromZ", AtomicNameFromZ);
    m.def("AtomicNameFromSym", AtomicNameFromSym);


    // Atom structure
    pybind11::class_<Atom>(m, "Atom")
    .def_readwrite("id", &Atom::id)
    .def_readwrite("z", &Atom::z)
    .def_readwrite("isonum", &Atom::isonum)
    .def_readwrite("xyz", &Atom::xyz)
    .def("Mass", &Atom::Mass)
    .def("Name", &Atom::Name)
    .def("Symbol", &Atom::Symbol)
    ;

    // Molecule class
    pybind11::class_<Molecule>(m, "Molecule")
    .def(pybind11::init<>())
    .def("AddAtom", Molecule_AddAtomWrapper)
    .def("GetAtom", &Molecule::GetAtom)
    .def("NAtoms", &Molecule::NAtoms)
    ;

    return m.ptr();
}


} // close namespace export_python
} // close namespace molecule 
} // close namespace bpmodule

