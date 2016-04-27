/*! \file
 *
 * \brief Python exports for the system/system library
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "pulsar/python/Pybind11_stl.hpp"
#include "pulsar/python/Pybind11_functional.hpp"
#include "pulsar/python/Pybind11_operators.hpp"
#include "pulsar/python/Pybind11_iterators.hpp"
#include "pulsar/python/Convert.hpp"
#include "pulsar/system/AMConvert.hpp"
#include "pulsar/system/AtomicInfo.hpp"
#include "pulsar/system/System.hpp"
#include "pulsar/system/BasisSet.hpp"
#include "pulsar/system/Symmetrizer.hpp"
#include "pulsar/system/SymmetryElements.hpp"
#include "pulsar/system/AOOrdering.hpp"
#include "pulsar/system/SphericalTransform.hpp"
#include "pulsar/math/RegisterMathSet.hpp"



namespace pulsar{
namespace system {
namespace export_python {

// in testing_export.cpp
void export_testing(pybind11::module & m);



PYBIND11_PLUGIN(system)
{
    pybind11::module m("system", "Molecular system specification");

    ///////////////
    // Angular momentum
    ///////////////
    m.def("StringToAM", StringToAM);
    m.def("AMToString", AMToString);


    ///////////////
    // Ordering
    ///////////////
    pybind11::class_<AOOrderingMaps>(m, "AOOrderingMaps")
    .def(pybind11::init<>())
    .def(pybind11::init<const AOOrderingMaps &>())
    .def_readwrite("spherical_order", &AOOrderingMaps::spherical_order)
    .def_readwrite("cartesian_order", &AOOrderingMaps::cartesian_order)
    ;

    m.def("AllAOOrderings", AllAOOrderings, pybind11::return_value_policy::copy);
    m.def("SphericalOrdering", SphericalOrdering);
    m.def("CartesianOrdering", CartesianOrdering);
    m.def("SphericalIndex", SphericalIndex);
    m.def("CartesianIndex", CartesianIndex);
    m.def("MakeBSReorderMap", MakeBSReorderMap);
    m.def("MakeAOBasisOrdering", MakeAOBasisOrdering);

    ////////////////////////////
    // Spherical Transformation
    ////////////////////////////
    pybind11::class_<SphericalTransformCoefficient>(m, "SphericalTransformCoefficient")
    .def(pybind11::init<>())
    .def(pybind11::init<const SphericalTransformCoefficient &>())
    .def_readwrite("sphidx", &SphericalTransformCoefficient::sphidx)
    .def_readwrite("cartidx", &SphericalTransformCoefficient::cartidx)
    .def_readwrite("coef", &SphericalTransformCoefficient::coef)
    ;

    m.def("AllSphericalTransforms", AllSphericalTransforms, pybind11::return_value_policy::copy);

    ///////////////
    // Basis set
    ///////////////

    // Enumeration for basis set shell types
    pybind11::enum_<ShellType>(m, "ShellType")
    .value("SphericalGaussian", ShellType::SphericalGaussian)
    .value("CartesianGaussian", ShellType::CartesianGaussian)
    .value("Slater", ShellType::Slater)
    ;


    // BasisShellBase class
    pybind11::class_<BasisShellBase> bshellbase(m, "BasisShellBase");
    bshellbase.def("GetType", &BasisShellBase::GetType)
              .def("AM", &BasisShellBase::AM)
              .def("GeneralAM", &BasisShellBase::GeneralAM)
              .def("NPrim", &BasisShellBase::NPrim)
              .def("NCoef", &BasisShellBase::NCoef)
              .def("NGeneral", &BasisShellBase::NGeneral)
              .def("NFunctions", &BasisShellBase::NFunctions)
              .def("GeneralNFunctions", &BasisShellBase::GeneralNFunctions)
              .def("IsCombiendAM", &BasisShellBase::IsCombinedAM)
              .def("MyHash", &BasisShellBase::MyHash)
              .def("Alpha", &BasisShellBase::Alpha)
              .def("Coef", &BasisShellBase::Coef)
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
              .def("SetPrimitive", static_cast<void (BasisShellBase::*)(size_t, double, double)>(&BasisShellBase::SetPrimitive))
              .def("SetPrimitive", static_cast<void (BasisShellBase::*)(size_t, double, const std::vector<double> &)>(&BasisShellBase::SetPrimitive))
    ;


    // BasisShellInfo class
    pybind11::class_<BasisShellInfo> bshell(m, "BasisShellInfo", bshellbase);
    bshell.def(pybind11::init<ShellType, int, int, int>())
          .def(pybind11::init<ShellType, int, int, int, const std::vector<double> &, const std::vector<double> &>())
          .def(pybind11::self == pybind11::self)
          .def(pybind11::self != pybind11::self)
    ;
    

    // BasisSetShell class
    pybind11::class_<BasisSetShell>(m, "BasisSetShell", bshell)
    .def("GetID", &BasisSetShell::GetID)
    .def("GetCenter", &BasisSetShell::GetCenter)
    .def("GetCoords", &BasisSetShell::GetCoords)
    .def("SetCoords", &BasisSetShell::GetCoords)
    .def("GetCoord", &BasisSetShell::GetCoord)
    .def("SetCoord", &BasisSetShell::GetCoord)
    .def("MyHash", &BasisSetShell::MyHash)
    .def(pybind11::self == pybind11::self)
    .def(pybind11::self != pybind11::self)
    ;


    // Main BasisSet class
    python::RegisterPyRefIterator<BasisSet>(m, "BasisSet");

    pybind11::class_<BasisSet>(m, "BasisSet")
    .def(pybind11::init<size_t, size_t, size_t, size_t>())
    .def("Print", &BasisSet::Print)
    .def("AddShell", &BasisSet::AddShell)
    .def("NShell", &BasisSet::NShell)
    .def("Shell", &BasisSet::Shell, pybind11::return_value_policy::reference_internal)
    .def("UniqueShell", &BasisSet::UniqueShell, pybind11::return_value_policy::reference_internal)
    .def("ShellInfo", &BasisSet::ShellInfo)
    .def("NPrim", &BasisSet::NPrim)
    .def("NCoef", &BasisSet::NCoef)
    .def("NCoef", &BasisSet::NCoef)
    .def("NFunctions", &BasisSet::NFunctions)
    .def("MaxNPrim", &BasisSet::MaxNPrim)
    .def("MaxAM", &BasisSet::MaxAM)
    .def("AllAM", &BasisSet::AllAM)
    .def("MaxNFunctions", &BasisSet::MaxNFunctions)
    .def("Transform", &BasisSet::Transform)
    .def("ShrinkFit", &BasisSet::ShrinkFit)
    .def("MyHash", &BasisSet::MyHash)
    .def(pybind11::self == pybind11::self)
    .def(pybind11::self != pybind11::self)
    .def("__iter__", [](pybind11::object obj)
            {
                const BasisSet & cont = obj.cast<const BasisSet &>();
                return python::PyRefIterator<BasisSet>(cont, cont.begin(), obj);
            })
    ;
    ;


    ////////////////////
    // System, etc
    ////////////////////

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
    m.def("InertiaTensor",InertiaTensor);


    // Atom structure
    // Atom class
    pybind11::class_<Atom>(m, "Atom", pybind11::base<math::Point>())
    .def(pybind11::init<const Atom &>())
    .def("MyHash", &Atom::MyHash)
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
    .def(pybind11::self != pybind11::self)
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

    pybind11::class_<System, std::shared_ptr<System>>(m,"System")
    .def(pybind11::init<const std::shared_ptr<AtomSetUniverse>, bool>())
    .def(pybind11::init<const System &>())
    .def("Size",&System::Size)
    .def("Contains", &System::Contains)
    .def("Insert", static_cast<System &(System::*)(const Atom &)>(&System::Insert),
                   pybind11::return_value_policy::reference)
    .def("GetCharge",&System::GetCharge)
    .def("GetNElectrons",&System::GetNElectrons)
    .def("GetBasisSet", &System::GetBasisSet)
    .def("MyHash", &System::MyHash)
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
    .def(pybind11::self != pybind11::self)
    .def("__len__",         &System::Size)
    .def("__contains__",    &System::Contains)
    .def("__str__",&System::ToString)
    .def("__iter__", [](pybind11::object obj)
            {
                const System & cont = obj.cast<const System &>();
                return python::PyCopyIterator<System>(cont, cont.begin(), obj);
            })
    ;

    pybind11::class_<Symmetrizer>(m, "Symmetrizer")
    .def(pybind11::init<>())
    .def("GetSymmetry",&Symmetrizer::GetSymmetry);
    
    pybind11::class_<SymmetryElement>(m,"SymmetryElement")
    .def(pybind11::init<const std::array<double,9>&,
                        const std::string&,const std::string&>())
    .def(pybind11::init<const SymmetryElement&>())
    .def_readonly("Elem",&SymmetryElement::Elem)
    .def_readonly("SSymbol",&SymmetryElement::SSymbol)
    .def_readonly("HMSymbol",&SymmetryElement::HMSymbol)
    ;
    
    pybind11::class_<MirrorPlane>
    (m,"MirrorPlane",pybind11::base<SymmetryElement>())
    .def(pybind11::init<const std::array<double,3>&>())
    ;
    
    pybind11::class_<Rotation>
    (m,"Rotation",pybind11::base<SymmetryElement>())
    .def(pybind11::init<const std::array<double,3>&,size_t,size_t>())
    ;
    
    pybind11::class_<ImproperRotation>
    (m,"ImproperRotation",pybind11::base<SymmetryElement>())
    .def(pybind11::init<const std::array<double,3>&,size_t,size_t>())
    ;
    
    m.attr("Identity")=pybind11::cast(SymmetryElement(Identity));
    m.attr("CoI")=pybind11::cast(SymmetryElement(CoI));
    
    // Export the testing stuff
    export_testing(m);

    return m.ptr();
}


} // close namespace export_python
} // close namespace system 
} // close namespace pulsar

