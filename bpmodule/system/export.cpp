/*! \file
 *
 * \brief Python exports for the molecule/system library
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/python/Pybind11_stl.hpp"
#include "bpmodule/python/Pybind11_functional.hpp"
#include "bpmodule/python/Convert.hpp"
#include "bpmodule/system/AtomicInfo.hpp"
#include "bpmodule/system/Molecule.hpp"
#include "bpmodule/basisset/BasisSet.hpp"
#include "bpmodule/datastore/RegisterUIDPointer.hpp"
#include "bpmodule/python/Convert.hpp"
#include "bpmodule/math/RegisterMathSet.hpp"


namespace bpmodule {
namespace system {
namespace export_python {



PYBIND11_PLUGIN(system)
{
    pybind11::module m("system", "Molecular system specification");

    datastore::RegisterUIDPointer<Molecule>(m, "Molecule");


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
    m.def("AtomicMultiplicityFromZ", AtomicMultiplicityFromZ);
    m.def("AtomicMultiplicityFromSym", AtomicMultiplicityFromSym);


    // Atom structure
    // Atom class
    pybind11::class_<Atom>(m, "Atom", pybind11::base<math::Point>())
    .def("GetZ", &Atom::GetZ)
    .def("SetZ", &Atom::SetZ)
    .def("GetIsonum", &Atom::GetIsonum)
    .def("SetIsonum", &Atom::SetIsonum)
    .def("GetMass", &Atom::GetMass)
    .def("SetMass", &Atom::SetMass)
    .def("GetIsotopeMass", &Atom::GetIsotopeMass)
    .def("SetIsotopeMass", &Atom::SetIsotopeMass)
    .def("GetCharge", &Atom::GetCharge)
    .def("SetCharge", &Atom::SetCharge)
    .def("GetMultiplicity", &Atom::GetMultiplicity)
    .def("SetMultiplicity", &Atom::SetMultiplicity)
    .def("GetNElectrons", &Atom::GetNElectrons)
    .def("SetNElectrons", &Atom::SetNElectrons)
    .def("GetAllShells", &Atom::GetAllShells)
    .def("GetShells", &Atom::GetShells)
    .def("SetShells", &Atom::SetShells)
    .def("AddShell", &Atom::AddShell)
    .def("GetName", &Atom::GetName)
    .def("GetSymbol", &Atom::GetSymbol)
    ;
   

    // Atom creators
    m.def("CreateAtom", static_cast<Atom (*)(size_t, Atom::CoordType, int)>(CreateAtom));
    m.def("CreateAtom", static_cast<Atom (*)(size_t, Atom::CoordType, int, int)>(CreateAtom));
    m.def("CreateAtom", static_cast<Atom (*)(size_t, double, double, double, int)>(CreateAtom));
    m.def("CreateAtom", static_cast<Atom (*)(size_t, double, double, double, int, int)>(CreateAtom));

    // Export AtomSet, etc
    math::RegisterMathSet<Atom>(m, "AtomSetUniverse", "AtomSet");

    // Main molecule class 
    pybind11::class_<Molecule>(m,"Molecule")
    .def(pybind11::init<const std::shared_ptr<AtomSetUniverse>, bool>())
    .def(pybind11::init<const Molecule &>())
    .def("NAtoms",&Molecule::NAtoms)
    .def("HasAtom", &Molecule::HasAtom)
    .def("GetAtom", &Molecule::GetAtom)
    .def("GetCharge",&Molecule::GetCharge)
    .def("GetNElectrons",&Molecule::GetNElectrons)
    .def("GetBasisSet", &Molecule::GetBasisSet)
    .def("Translate", &Molecule::Translate<std::array<double, 3>>)
    .def("Rotate", &Molecule::Rotate<std::array<double, 9>>)
    .def("CenterOfMass", &Molecule::CenterOfMass)
    .def("CenterOfNuclearCharge", &Molecule::CenterOfNuclearCharge)
    .def("ToString", &Molecule::ToString)
    .def("Transform", &Molecule::Transform)
    .def("Insert", &Molecule::Insert)
    .def("Partition", &Molecule::Partition)
    .def("Complement", &Molecule::Complement)
    .def("Intersection", &Molecule::Intersection)
    .def("Union", &Molecule::Union)
    .def("Difference", &Molecule::Difference)
    .def("__str__", &Molecule::ToString)
    ;

        
    return m.ptr();
}


} // close namespace export_python
} // close namespace system 
} // close namespace bpmodule

