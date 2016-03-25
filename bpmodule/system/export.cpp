/*! \file
 *
 * \brief Python exports for the system/system library
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/python/Pybind11_stl.hpp"
#include "bpmodule/python/Pybind11_functional.hpp"
#include "bpmodule/python/Pybind11_operators.hpp"
#include "bpmodule/python/Pybind11_iterators.hpp"
#include "bpmodule/python/Convert.hpp"
#include "bpmodule/system/AMConvert.hpp"
#include "bpmodule/system/AtomicInfo.hpp"
#include "bpmodule/system/System.hpp"
#include "bpmodule/system/BasisSet.hpp"
#include "bpmodule/datastore/RegisterUIDPointer.hpp"
#include "bpmodule/python/Convert.hpp"
#include "bpmodule/math/RegisterMathSet.hpp"


namespace bpmodule {
namespace system {
namespace export_python {



PYBIND11_PLUGIN(system)
{
    pybind11::module m("system", "Molecular system specification");

    ///////////////
    // Angular momentum
    ///////////////
    m.def("StringToAM", StringToAM);
    m.def("AMToString", AMToString);

    ///////////////
    // Basis set
    ///////////////

    datastore::RegisterUIDPointer<BasisSet>(m, "BasisSet");

    // Enumeration for basis set shell types
    pybind11::enum_<ShellType>(m, "ShellType")
    .value("Gaussian", ShellType::Gaussian)
    .value("Slater", ShellType::Slater)
    ;


    // BasisShellInfo class
    pybind11::class_<BasisShellBase> bshellbase(m, "BasisShellBase");
    bshellbase.def("GetType", &BasisShellBase::GetType)
              .def("AM", &BasisShellBase::AM)
              .def("NPrim", &BasisShellBase::NPrim)
              .def("NCoef", &BasisShellBase::NCoef)
              .def("NGeneral", &BasisShellBase::NGeneral)
              .def("NCartesian", &BasisShellBase::NCartesian)
              .def("NSpherical", &BasisShellBase::NSpherical)
              .def("NFunctions", &BasisShellBase::NFunctions)
              .def("IsCombiendAM", &BasisShellBase::IsCombinedAM)
              .def("IsCartesian", &BasisShellBase::IsCartesian)
              .def("IsSpherical", &BasisShellBase::IsSpherical)

              .def("Alpha", static_cast<double &(BasisShellBase::*)(int)>(&BasisShellBase::Alpha))
              .def("Coef", static_cast<double &(BasisShellBase::*)(int)>(&BasisShellBase::Alpha))

              .def("GetAlpha", &BasisShellBase::GetAlpha)
              .def("SetAlpha", &BasisShellBase::SetAlpha)
              .def("GetCoef", &BasisShellBase::GetCoef)
              .def("SetCoef", &BasisShellBase::SetCoef)

              .def("GetAlphas", &BasisShellBase::GetAlphas)
              .def("SetAlphas", &BasisShellBase::SetAlphas)
              .def("GetCoefs", &BasisShellBase::GetCoefs)
              .def("SetCoefs", &BasisShellBase::SetCoefs)
              .def("GetAllCoefs", &BasisShellBase::GetAllCoefs)
              .def("SetAllCoefs", &BasisShellBase::SetAllCoefs)

              .def("SetPrimitive", static_cast<void (BasisShellBase::*)(int, double, double)>(&BasisShellBase::SetPrimitive))
              .def("SetPrimitive", static_cast<void (BasisShellBase::*)(int, double, const std::vector<double> &)>(&BasisShellBase::SetPrimitive))
    ;


    // BasisShellInfo class
    pybind11::class_<BasisShellInfo> bshell(m, "BasisShellInfo", bshellbase);
    bshell.def(pybind11::init<ShellType, int, bool, int, int>())
    .def(pybind11::self == pybind11::self)
    ;
    

    // BasisSetShell class
    pybind11::class_<BasisSetShell>(m, "BasisSetShell", bshell)
    .def("GetID", &BasisSetShell::GetID)
    .def("GetCenter", &BasisSetShell::GetCenter)
    .def("GetCoords", &BasisSetShell::GetCoords)
    .def(pybind11::self == pybind11::self)
    ;


    // Main BasisSet class
    pybind11::class_<BasisSet>(m, "BasisSet")
    .def(pybind11::init<size_t, size_t, size_t>())
    .def("Print", &BasisSet::Print)
    .def("AddShell", &BasisSet::AddShell)
    .def("NShell", &BasisSet::NShell)
    .def("Shell", &BasisSet::Shell, pybind11::return_value_policy::reference_internal)
    .def("NPrim", &BasisSet::NPrim)
    .def("NCoef", &BasisSet::NCoef)
    .def("NCoef", &BasisSet::NCoef)
    .def("NCartesian", &BasisSet::NCartesian)
    .def("NFunctions", &BasisSet::NFunctions)
    .def("MaxNPrim", &BasisSet::MaxNPrim)
    .def("MaxAM", &BasisSet::MaxAM)
    .def("MaxNCartesian", &BasisSet::MaxNCartesian)
    .def("MaxNFunctions", &BasisSet::MaxNFunctions)
    .def("Transform", &BasisSet::Transform)
    .def("ShrinkFit", &BasisSet::ShrinkFit)
    ;


    ////////////////////
    // System, etc
    ////////////////////
    datastore::RegisterUIDPointer<System>(m, "System");


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
    .def(pybind11::init<const Atom &>())
    .def("GetIdx", &Atom::GetIdx)
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
    .def("GetName", &Atom::GetName)
    .def("GetSymbol", &Atom::GetSymbol)
    .def("HasShells", &Atom::HasShells)
    .def("NShell", &Atom::NShell)
    .def("GetAllBasisLabels", &Atom::GetAllBasisLabels)
    .def("GetBasisDescription", &Atom::GetBasisDescription)
    .def("GetShells", &Atom::GetShells)
    .def("SetShells", &Atom::SetShells)
    .def("AddShell", &Atom::AddShell)
    .def(pybind11::self == pybind11::self)
    ;
   

    // Atom creators
    m.def("CreateAtom", static_cast<Atom (*)(size_t, CoordType, int)>(CreateAtom));
    m.def("CreateAtom", static_cast<Atom (*)(size_t, CoordType, int, int)>(CreateAtom));
    m.def("CreateAtom", static_cast<Atom (*)(size_t, double, double, double, int)>(CreateAtom));
    m.def("CreateAtom", static_cast<Atom (*)(size_t, double, double, double, int, int)>(CreateAtom));

    // Export AtomSetUniverse
    // No need to export AtomSet (at the moment)
    math::RegisterUniverse<AtomSetUniverse>(m, "AtomSetUniverse");

    
    // Main system class 
    python::RegisterPyCopyIterator<System>(m, "System");

    pybind11::class_<System>(m,"System")
    .def(pybind11::init<const std::shared_ptr<AtomSetUniverse>, bool>())
    .def(pybind11::init<const System &>())
    .def("Size",&System::Size)
    .def("Contains", &System::Contains)
    .def("Insert", static_cast<System &(System::*)(const Atom &)>(&System::Insert),
                   pybind11::return_value_policy::reference)
    .def("GetCharge",&System::GetCharge)
    .def("GetNElectrons",&System::GetNElectrons)
    .def("GetBasisSet", &System::GetBasisSet)
    .def("Translate", &System::Translate<std::array<double, 3>>)
    .def("Rotate", &System::Rotate<std::array<double, 9>>)
    .def("CenterOfMass", &System::CenterOfMass)
    .def("CenterOfNuclearCharge", &System::CenterOfNuclearCharge)
    .def("Print", &System::Print)
    .def("ToString", &System::ToString)
    .def("UnionAssign", &System::UnionAssign, pybind11::return_value_policy::reference)
    .def("Union", &System::Union)
    .def("IntersectionAssign", &System::IntersectionAssign, pybind11::return_value_policy::reference)
    .def("Intersection", &System::Intersection)
    .def("DifferenceAssign", &System::DifferenceAssign, pybind11::return_value_policy::reference)
    .def("Difference", &System::Difference)
    .def("Complement", &System::Complement)
    .def("IsProperSubsetOf", &System::IsProperSubsetOf)
    .def("IsSubsetOf", &System::IsSubsetOf)
    .def("IsProperSupersetOf", &System::IsProperSupersetOf)
    .def("IsSupersetOf", &System::IsSupersetOf)
    .def("Transform", &System::Transform)
    .def("Partition", &System::Partition)
    .def(pybind11::self += pybind11::self, pybind11::return_value_policy::reference)
    .def(pybind11::self + pybind11::self)
    .def(pybind11::self -= pybind11::self, pybind11::return_value_policy::reference)
    .def(pybind11::self - pybind11::self)
    .def(pybind11::self /= pybind11::self, pybind11::return_value_policy::reference)
    .def(pybind11::self / pybind11::self)
    .def(pybind11::self >= pybind11::self)
    .def(pybind11::self > pybind11::self)
    .def(pybind11::self <= pybind11::self)
    .def(pybind11::self < pybind11::self)
    .def(pybind11::self == pybind11::self)
    .def("__len__",         &System::Size)
    .def("__contains__",    &System::Contains)
    .def("__str__",&System::ToString)
    .def("__iter__", [](pybind11::object obj)
            {
                const System & cont = obj.cast<const System &>();
                return python::PyCopyIterator<System>(cont, cont.begin(), obj);
            })
    ;

    return m.ptr();
}


} // close namespace export_python
} // close namespace system 
} // close namespace bpmodule

