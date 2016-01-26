/*! \file
 *
 * \brief Python exports for the system library
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/python/Convert.hpp"
#include "bpmodule/system/AtomicInfo.hpp"
#include "bpmodule/system/Molecule.hpp"
#include "bpmodule/datastore/RegisterUIDPointer.hpp"
#include "bpmodule/python/Convert.hpp"

using bpmodule::python::ConvertToCpp;

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


    /* Atom structure
    pybind11::class_<Atom>(m, "Atom")
    .def_readwrite("id", &Atom::id)
    .def_readwrite("z", &Atom::z)
    .def_readwrite("isonum", &Atom::isonum)
    .def_readwrite("xyz", &Atom::xyz)
    .def("Mass", &Atom::Mass)
    .def("Name", &Atom::Name)
    .def("Symbol", &Atom::Symbol)
    ;
    */
    
    pybind11::class_<Atom>(m,"Atom")
    .def(pybind11::init<int,double,double,double>())
    .def("Z",&Atom::Z,"Returns the atomic number")
    .def("Symbol",&Atom::Symbol,"Returns the atomic symbol")
    .def("Mass",&Atom::Mass,"Returns the mass")
    .def("Charge",&Atom::Charge,"Returns the charge")
    .def("Mult",&Atom::Mult,"Returns the multiplicity")
    .def("NElec",&Atom::NElec,"Returns the number of electrons")
    .def("SetMass",&Atom::SetMass,"Sets the mass of the atom")
    .def("SetChargeAndMult",&Atom::SetChargeAndMult,
            "Sets the charge and multiplicity")
    .def("SetNElec",&Atom::SetNElec,"Sets the number of electrons")
    .def("__str__",&Atom::ToString,"Prints the atom out");

            
    pybind11::class_<Molecule>(m,"Molecule")
    .def(pybind11::init<>())
    .def("AddAtom",&Molecule::AddAtom)
    .def("__str__",&Molecule::ToString);        
            
    /* Molecule class
    pybind11::class_<Molecule>(m, "Molecule")
    .def(pybind11::init<>())
    .def("AddAtom", &Molecule::AddAtom)
    .def("GetAtom", &Molecule::GetAtom)
    .def("NAtoms", &Molecule::NAtoms)
    ;*/

    return m.ptr();
}


} // close namespace export_python
} // close namespace system 
} // close namespace bpmodule

