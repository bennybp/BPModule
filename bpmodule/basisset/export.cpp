/*! \file
 *
 * \brief Python exports for basis set
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include <boost/python/def.hpp>

#include "bpmodule/basisset/BasisSet.hpp"
#include "bpmodule/basisset/Creators.hpp"
#include "bpmodule/datastore/RegisterUIDPointer.hpp"


using namespace boost::python;


namespace bpmodule {
namespace basisset {
namespace export_python {


BOOST_PYTHON_MODULE(basisset)
{
    datastore::RegisterUIDPointer<BasisSet>("BasisSet");


    class_<GaussianBasisShell>("GaussianBasisShell", init<int, bool>())
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
    .def("AddPrimitive", &GaussianShell::AddPrimitive)
    ;

    class_<GaussianShell, bases<GaussianBasisShell>>("GaussianShell", init<int, bool, unsigned long, unsigned long, double, double, double>())
    .def(init<const GaussianBasisShell &, unsigned long, unsigned long, double, double, double>())
    .def("ID", &GaussianShell::ID)
    .def("Center", &GaussianShell::Center)
    .def("Coordinates", &GaussianShell::Coordinates)
    ;


    class_<BasisSet>("BasisSet", init<>())
    .def("Print", &BasisSet::Print)
    .def("AddShell", &BasisSet::AddShell)
    .def("NShell", &BasisSet::NShell)
    .def("Shell", &BasisSet::Shell)
    ;


    // Creators
    def("SimpleCreator", bpmodule::basisset::SimpleCreator);
}


} // close namespace export_python
} // close namespace basisset 
} // close namespace bpmodule

