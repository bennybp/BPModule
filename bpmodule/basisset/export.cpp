/*! \file
 *
 * \brief Python exports for basis set
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <boost/python/module.hpp>
#include <boost/python/class.hpp>

#include "bpmodule/basisset/BasisSet.hpp"


using namespace boost::python;


namespace bpmodule {
namespace basisset {
namespace export_python {

BOOST_PYTHON_MODULE(basisset)
{
    class_<GaussianShell>("GaussianShell", init<int, bool, double, double, double>())
    .def("ID", &GaussianShell::ID)
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
    .def("Coordinates", &GaussianShell::Coordinates)
    .def("AddPrimitive", &GaussianShell::AddPrimitive)
    ;


    class_<BasisSet>("BasisSet", init<>())
    .def("AddShell", &BasisSet::AddShell)
    .def("NShell", &BasisSet::NShell)
    .def("Shell", &BasisSet::Shell)
    ;
}


} // close namespace export_python
} // close namespace basisset 
} // close namespace bpmodule

