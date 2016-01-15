/*! \file
 *
 * \brief Python exports for math
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/python/Pybind11.hpp"
#include "bpmodule/math/Factorial.hpp"
#include "bpmodule/math/Locus.hpp"

//! \todo Export exact casts? Or have the equivalent with python?

namespace bpmodule {
namespace math {
namespace export_python {

PYBIND11_PLUGIN(math)
{
    pybind11::module m("math", "Some common math operations");

    m.def("Factorial", Factorial); 
    m.def("FactorialF", FactorialF); 
    m.def("FactorialD", FactorialD); 

    m.def("DoubleFactorial", DoubleFactorial); 
    m.def("DoubleFactorialF", DoubleFactorialF); 
    m.def("DoubleFactorialD", DoubleFactorialD); 

    m.def("Double2nm1Factorial", Double2nm1Factorial); 
    m.def("Double2nm1FactorialF", Double2nm1FactorialF); 
    m.def("Double2nm1FactorialD", Double2nm1FactorialD); 

    pybind11::class_<Point>(m,"Point")
          .def(pybind11::init<double,double,double>())
          .def("AddWeight",&Point::AddWeight)
          .def("NWeights",&Point::NWeights)
          .def("Weight",&Point::Weight)
          .def("__getitem__",&Point::operator[]);
          //.def("__str__",&Point::operator std::string());

    return m.ptr();
}


} // close namespace export_python
} // close namespace math 
} // close namespace bpmodule

