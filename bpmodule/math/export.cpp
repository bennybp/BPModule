/*! \file
 *
 * \brief Python exports for math
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/python/Pybind11.hpp"
#include "bpmodule/python/Pybind11_stl.hpp"
#include "bpmodule/python/Pybind11_functional.hpp"
#include "bpmodule/python/Pybind11_operators.hpp"
#include "bpmodule/math/Factorial.hpp"
#include "bpmodule/math/Universe.hpp"
#include "bpmodule/math/Point.hpp"
#include "bpmodule/math/SimpleMatrix.hpp"
#include "bpmodule/math/IrrepSpinMatrix.hpp"


//! \todo Export exact casts? Or have the equivalent with python?

namespace bpmodule {
namespace math {
namespace export_python {


// in testing/export.cpp
void export_testing(pybind11::module & m);



template<typename T>
static void RegisterSimpleMatrix(pybind11::module & m, const char * type)
{
    std::string name("SimpleMatrix");
    name += type;
    pybind11::class_<SimpleMatrix<T>>(m, name.c_str())
    .def(pybind11::init<>())
    .def(pybind11::init<size_t, size_t>())
    .def(pybind11::init<size_t, size_t, const std::vector<T> &>())
    .def(pybind11::init<const SimpleMatrix<T> &>())
    .def("NRows", &SimpleMatrix<T>::NRows)
    .def("NCols", &SimpleMatrix<T>::NCols)
    .def("Size", &SimpleMatrix<T>::Size)
    .def("Zero", &SimpleMatrix<T>::Zero)
    .def(pybind11::self == pybind11::self)
    .def("__getitem__", [](const SimpleMatrix<T> & m, std::pair<size_t, size_t> i) {
         if (i.first >= m.NRows() || i.second >= m.NCols())
             throw pybind11::index_error();
         return m(i.first, i.second);
     })
    .def("__setitem__", [](SimpleMatrix<T> & m, std::pair<size_t, size_t> i, T v) {
         if (i.first >= m.NRows() || i.second >= m.NCols())
             throw pybind11::index_error();
         m(i.first, i.second) = v;
     })
    .def_buffer([](SimpleMatrix<T> & m) -> pybind11::buffer_info {
         return pybind11::buffer_info(
             m.Data(),                              /* Pointer to buffer */
             sizeof(T),                             /* Size of one scalar */
             pybind11::format_descriptor<T>::value(),     /* Python struct-style format descriptor */
             2,                                     /* Number of dimensions */
             { m.NRows(), m.NCols() },              /* Buffer dimensions */
             { sizeof(T) * m.NRows(),               /* Strides (in bytes) for each index */
               sizeof(T) }
         );
     });
}


template<typename T>
static void RegisterIrrepSpinMatrix(pybind11::module & m, const char * type)
{
    std::string name("IrrepSpinMatrix");
    name += type;
    pybind11::class_<IrrepSpinMatrix<T>>(m, name.c_str())
    .def(pybind11::init<>())
    .def(pybind11::init<const IrrepSpinMatrix<T> &>())
    .def(pybind11::self == pybind11::self)
    .def("Has", &IrrepSpinMatrix<T>::Has)
    .def("Get", static_cast<SimpleMatrix<T> &(IrrepSpinMatrix<T>::*)(Irrep, int)>(&IrrepSpinMatrix<T>::Get))
    .def("Set", &IrrepSpinMatrix<T>::Set)
    .def("Erase", &IrrepSpinMatrix<T>::Erase)
    .def("Clear", &IrrepSpinMatrix<T>::Clear)
    ;
}

    
    
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
    ;
   
    /////////////////////////// 
    // Enumeration for Irreps
    /////////////////////////// 
    pybind11::enum_<Irrep>(m, "Irrep")
    .value("A1", Irrep::A1)
    .value("A2", Irrep::A2)
    .value("B1", Irrep::B1)
    .value("B2", Irrep::B2)
    .value("E1", Irrep::E1)
    .value("E2", Irrep::E2)
    .value("A1g", Irrep::A1g)
    .value("A2g", Irrep::A2g)
    .value("B1g", Irrep::B1g)
    .value("B2g", Irrep::B2g)
    .value("E1g", Irrep::E1g)
    .value("E2g", Irrep::E2g)
    .value("A1u", Irrep::A1u)
    .value("A2u", Irrep::A2u)
    .value("B1u", Irrep::B1u)
    .value("B2u", Irrep::B2u)
    .value("E1u", Irrep::E1u)
    .value("E2u", Irrep::E2u)
    ;


    //////////////////////////////////
    // SimpleMatrix
    //////////////////////////////////
    RegisterSimpleMatrix<float>(m, "F");
    RegisterSimpleMatrix<double>(m, "D");

    //////////////////////////////////
    // IrrepSpinMatrix
    //////////////////////////////////
    RegisterIrrepSpinMatrix<float>(m, "F");
    RegisterIrrepSpinMatrix<double>(m, "D");

    //! \todo complex values in python
    //RegisterSimpleMatrix<std::complex<float>>(m, "CF");
    //RegisterSimpleMatrix<std::complex<double>>(m, "CD");

    // Export the testing stuff
    export_testing(m);

    return m.ptr();
}


} // close namespace export_python
} // close namespace math 
} // close namespace bpmodule

