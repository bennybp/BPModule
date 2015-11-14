/*! \file
 *
 * \brief Python exports for the molecule library
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include <boost/python/def.hpp>
#include <boost/python/copy_const_reference.hpp>
#include <boost/python/to_python_converter.hpp>

#include "bpmodule/python_helper/Convert.hpp"
#include "bpmodule/molecule/AtomicInfo.hpp"
#include "bpmodule/molecule/Molecule.hpp"

using namespace boost::python;
using bpmodule::python_helper::CppToPyExport;
using bpmodule::python_helper::ConvertToCpp;

namespace bpmodule {
namespace molecule {
namespace export_python {


// A bit of a hack
// Since we want isotopes to be a data member, and not a function,
// Python won't use the to_python_converter. So we use add_property
// instead and create a little wrapper function
std::vector<IsotopeData> AtomicDataIsotopes_(const AtomicData & ad) { return ad.isotopes; }

// Ditto for atom XYZ
std::array<double, 3> AtomXYZ_(const Atom & a) { return a.xyz; }



// Molecule::AddAtom wrapper
void Molecule_AddAtomWrapper(Molecule & mol, int Z, const boost::python::list & xyz)
{
    //! \todo check for list of length 3
    double x = ConvertToCpp<double>(xyz[0]);
    double y = ConvertToCpp<double>(xyz[1]);
    double z = ConvertToCpp<double>(xyz[2]);
    return mol.AddAtom(Z, {x, y, z});
}




BOOST_PYTHON_MODULE(molecule)
{
    class_<IsotopeData>("IsotopeData", no_init)
    .def_readonly("isonum", &IsotopeData::isonum)
    .def_readonly("mass", &IsotopeData::mass)
    .def_readonly("mass_low", &IsotopeData::mass_low)
    .def_readonly("mass_high", &IsotopeData::mass_high)
    .def_readonly("abund", &IsotopeData::abund)
    .def_readonly("abund_low", &IsotopeData::abund_low)
    .def_readonly("abund_high", &IsotopeData::abund_high)
    ;

    // A vector of isotopes
    to_python_converter<std::vector<IsotopeData>, CppToPyExport<std::vector<IsotopeData>>>();


    class_<AtomicData>("AtomicData", no_init)
    .def_readonly("Z", &AtomicData::Z)
    .def_readonly("sym", &AtomicData::sym)
    .def_readonly("name", &AtomicData::name)
    .def_readonly("mass", &AtomicData::mass)
    .def_readonly("mass_low", &AtomicData::mass_low)
    .def_readonly("mass_high", &AtomicData::mass_high)
    .add_property("isotopes", AtomicDataIsotopes_)
    ;


    // Free functions
    def("AtomicInfoFromZ", AtomicInfoFromZ, return_value_policy<copy_const_reference>());
    def("AtomicInfoFromSym", AtomicInfoFromSym, return_value_policy<copy_const_reference>());
    def("IsotopeInfoFromZ", IsotopeInfoFromZ, return_value_policy<copy_const_reference>());
    def("IsotopeInfoFromSym", IsotopeInfoFromSym, return_value_policy<copy_const_reference>());
    def("MostCommonIsotopeFromZ", MostCommonIsotopeFromZ);
    def("MostCommonIsotopeFromSym", MostCommonIsotopeFromSym);

    def("AtomicMassFromZ", AtomicMassFromZ);
    def("AtomicMassFromSym", AtomicMassFromSym);
    def("IsotopeMassFromZ", IsotopeMassFromZ);
    def("IsotopeMassFromSym", IsotopeMassFromSym);
    def("AtomicSymFromZ", AtomicSymFromZ);
    def("AtomicZNumberFromSym", AtomicZNumberFromSym);
    def("AtomicNameFromZ", AtomicNameFromZ);
    def("AtomicNameFromSym", AtomicNameFromSym);


    // Atom structure
    class_<Atom>("Atom", no_init)
    .def_readwrite("id", &Atom::id)
    .def_readwrite("z", &Atom::z)
    .def_readwrite("isonum", &Atom::isonum)
    .add_property("xyz", AtomXYZ_)
    .def("Mass", &Atom::Mass)
    .def("Name", &Atom::Name)
    .def("Symbol", &Atom::Symbol)
    ;

    // Molecule class
    class_<Molecule>("Molecule", init<>())
    .def("AddAtom", Molecule_AddAtomWrapper)
    .def("GetAtom", &Molecule::GetAtom)
    .def("NAtoms", &Molecule::NAtoms)
    ;


}


} // close namespace export_python
} // close namespace molecule 
} // close namespace bpmodule

