/*! \file
 *
 * \brief Python exports for math
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "pulsar/python/Pybind11.hpp"
#include "pulsar/python/Pybind11_stl.hpp"
#include "pulsar/python/Pybind11_functional.hpp"
#include "pulsar/python/Pybind11_operators.hpp"
#include "pulsar/math/Factorial.hpp"
#include "pulsar/math/Universe.hpp"
#include "pulsar/math/Point.hpp"
#include "pulsar/math/SimpleMatrix.hpp"
#include "pulsar/math/IrrepSpinMatrix.hpp"
#include "pulsar/math/RegisterMatrices.hpp"

//declared in RegisterMatrices
//PYBIND11_DECLARE_HOLDER_TYPE(T,std::shared_ptr<T>);



//! \todo Export exact casts? Or have the equivalent with python?

namespace pulsar{
namespace math {
namespace export_python {


// in testing_export.cpp
void export_testing(pybind11::module & m);



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



    //////////////////////////////////
    // Point
    //////////////////////////////////
    //! \todo If we need other types, we need a RegisterPoint function
    //        (probably ok just having it in this file)
    //! \todo pybind11 buffers
    pybind11::class_<Point>(m, "Point")
    .def(pybind11::init<double, double, double>()) 
    .def(pybind11::init<const Point::CoordType &>()) 
    .def("GetCoords", &Point::GetCoords)
    .def("SetCoords", static_cast<void (Point::*)(double, double, double)>(&Point::SetCoords))
    .def("SetCoords", static_cast<void (Point::*)(const Point::CoordType &)>(&Point::SetCoords))
    .def("__getitem__", [](const Point & p, size_t i) {
                             if (i >= 3)
                                 throw pybind11::index_error();
                             return p[i];
                        })
    .def("__setitem__", [](Point &p, size_t i, double v) {
                             if (i >= 3)
                                 throw pybind11::index_error();
                             p[i] = v;
                        })
    .def("__str__",[](const Point& p){
                        std::stringstream ss;
                        p.Print(ss);
                        return ss.str();})
    ;
   
    /////////////////////////// 
    // Enumeration for Irreps
    /////////////////////////// 
    pybind11::enum_<Irrep>(m, "Irrep")
    .value("A1", Irrep::A1).value("A2", Irrep::A2).value("B1", Irrep::B1)
    .value("B2", Irrep::B2).value("E1", Irrep::E1).value("E2", Irrep::E2)
    .value("A1g", Irrep::A1g).value("A2g", Irrep::A2g).value("B1g", Irrep::B1g)
    .value("B2g", Irrep::B2g).value("E1g", Irrep::E1g).value("E2g", Irrep::E2g)
    .value("A1u", Irrep::A1u).value("A2u", Irrep::A2u).value("B1u", Irrep::B1u)
    .value("B2u", Irrep::B2u).value("E1u", Irrep::E1u).value("E2u", Irrep::E2u)
    ;


    //////////////////////////////////
    // SimpleMatrix/IrrepSpinMatrix
    // (and vectors)
    //////////////////////////////////
    RegisterMatrices<float>(m, "F");
    RegisterMatrices<double>(m, "D");

    //! \todo complex values in python
    //RegisterSimpleMatrix<std::complex<float>>(m, "CF");
    //RegisterSimpleMatrix<std::complex<double>>(m, "CD");

    // Export the testing stuff
    export_testing(m);

    return m.ptr();
}


} // close namespace export_python
} // close namespace math 
} // close namespace pulsar

