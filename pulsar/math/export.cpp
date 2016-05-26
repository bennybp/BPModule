/*! \file
 *
 * \brief Python exports for math
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include "pulsar/math/Factorial.hpp"
#include "pulsar/math/Universe.hpp"
#include "pulsar/math/Point.hpp"
#include "pulsar/math/Grid.hpp"
#include "pulsar/math/SimpleMatrix.hpp"
#include "pulsar/math/IrrepSpinMatrix.hpp"
#include "pulsar/math/RegisterMathSet.hpp"

//declared in RegisterMathSet
//!\todo - fix multiple definitions of this in pybind11
//PYBIND11_DECLARE_HOLDER_TYPE(T,std::shared_ptr<T>);



//! \todo Export exact casts? Or have the equivalent with python?

namespace pulsar{
namespace math {
namespace export_python {


// in testing_export.cpp
void export_testing(pybind11::module & m);


template<typename T>
static void RegisterMatrices(pybind11::module & m, const char * type)
{
    std::string name("SimpleMatrix");
    std::string vname("SimpleVector");
    std::string isname("IrrepSpinMatrix");
    std::string isvname("IrrepSpinVector");
    std::string issname("IrrepSpinScalar");

    name += type;
    vname += type;
    isname += type;
    isvname += type;
    issname += type;

    pybind11::class_<SimpleMatrix<T>, std::shared_ptr<SimpleMatrix<T>>> mclass(m, name.c_str());
    mclass.def(pybind11::init<>())
          .def(pybind11::init<size_t, size_t>())
          .def(pybind11::init<size_t, size_t, const std::vector<T> &>())
          .def(pybind11::init<const SimpleMatrix<T> &>())
          .def("NRows", &SimpleMatrix<T>::NRows)
          .def("NCols", &SimpleMatrix<T>::NCols)
          .def("Size", &SimpleMatrix<T>::Size)
          .def("Zero", &SimpleMatrix<T>::Zero)
          .def("MyHash", &SimpleMatrix<T>::MyHash)
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
                   m.Data(),                                 /* Pointer to buffer */
                   sizeof(T),                                /* Size of one scalar */
                   pybind11::format_descriptor<T>::value,    /* Python struct-style format descriptor */
                   2,                                        /* Number of dimensions */
                   { m.NRows(), m.NCols() },                 /* Buffer dimensions */
                   { sizeof(T) * m.NRows(),                  /* Strides (in bytes) for each index */
                     sizeof(T) }
               );
           });

    pybind11::class_<SimpleVector<T>, std::shared_ptr<SimpleVector<T>>> vclass(m, vname.c_str(), mclass);
    vclass.def(pybind11::init<>())
          .def(pybind11::init<size_t>())
          .def(pybind11::init<const SimpleVector<T> &>())
          .def(pybind11::init<size_t, const std::vector<T> &>())
          .def("__getitem__", [](const SimpleVector<T> & m, size_t i) {
               if (i >= m.Size())
                   throw pybind11::index_error();
               return m(i);
           })
          .def("__setitem__", [](SimpleVector<T> & m, size_t i, T v) {
               if (i >= m.Size())
                   throw pybind11::index_error();
               m(i) = v;
           })
          .def_buffer([](SimpleVector<T> & m) -> pybind11::buffer_info {
               return pybind11::buffer_info(
                   m.Data(),                                /* Pointer to buffer */
                   sizeof(T),                               /* Size of one scalar */
                   pybind11::format_descriptor<T>::value,   /* Python struct-style format descriptor */
                   1,                                       /* Number of dimensions */
                   { m.Size()  },                           /* Buffer dimensions */
                   { sizeof(T) }                            /* Strides (in bytes) for each index */
               );
           });


    pybind11::class_<IrrepSpinMatrix<T>, std::shared_ptr<IrrepSpinMatrix<T>>> ismclass(m, isname.c_str());
    ismclass.def(pybind11::init<>())
            .def(pybind11::init<const IrrepSpinMatrix<T> &>())
            .def(pybind11::self == pybind11::self)
            .def("Has", &IrrepSpinMatrix<T>::Has)
            .def("GetIrreps", &IrrepSpinMatrix<T>::GetIrreps)
            .def("GetSpins", &IrrepSpinMatrix<T>::GetSpins)
            .def("Get", static_cast<SimpleMatrix<T> &(IrrepSpinMatrix<T>::*)(Irrep, int)>(&IrrepSpinMatrix<T>::Get))
            .def("Set", &IrrepSpinMatrix<T>::Set)
            .def("MyHash", &IrrepSpinMatrix<T>::MyHash)
    ;

    pybind11::class_<IrrepSpinVector<T>, std::shared_ptr<IrrepSpinVector<T>>> isvclass(m, isvname.c_str());
    isvclass.def(pybind11::init<>())
            .def(pybind11::init<const IrrepSpinVector<T> &>())
            .def(pybind11::self == pybind11::self)
            .def("Has", &IrrepSpinVector<T>::Has)
            .def("Get", static_cast<SimpleVector<T> &(IrrepSpinVector<T>::*)(Irrep, int)>(&IrrepSpinVector<T>::Get))
            .def("GetIrreps", &IrrepSpinVector<T>::GetIrreps)
            .def("GetSpins", &IrrepSpinVector<T>::GetSpins)
            .def("Set", &IrrepSpinVector<T>::Set)
            .def("MyHash", &IrrepSpinVector<T>::MyHash)
    ;

    pybind11::class_<IrrepSpinScalar<T>, std::shared_ptr<IrrepSpinScalar<T>>> issclass(m, issname.c_str());
    issclass.def(pybind11::init<>())
            .def(pybind11::init<const IrrepSpinScalar<T> &>())
            .def(pybind11::self == pybind11::self)
            .def("Has", &IrrepSpinScalar<T>::Has)
            .def("GetIrreps", &IrrepSpinScalar<T>::GetIrreps)
            .def("GetSpins", &IrrepSpinScalar<T>::GetSpins)
            .def("Get", static_cast<T &(IrrepSpinScalar<T>::*)(Irrep, int)>(&IrrepSpinScalar<T>::Get))
            .def("Set", &IrrepSpinScalar<T>::Set)
            .def("MyHash", &IrrepSpinScalar<T>::MyHash)
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
    // Point/Grid
    //////////////////////////////////
    //! \todo If we need other types, we need a RegisterPoint/etc function
    //        (probably ok just having it in this file)
    //! \todo pybind11 buffers
    pybind11::class_<Point>(m, "Point")
    .def(pybind11::init<double, double, double>()) 
    .def(pybind11::init<const Point::CoordType &>()) 
    .def("GetCoords", &Point::GetCoords)
    .def("SetCoords", static_cast<void (Point::*)(double, double, double)>(&Point::SetCoords))
    .def("SetCoords", static_cast<void (Point::*)(const Point::CoordType &)>(&Point::SetCoords))
    .def("Distance", &Point::Distance)
    .def("Magnitude", &Point::Magnitude)
    .def("Print", &Point::Print)
    .def("MyHash", &Point::MyHash)
    .def(pybind11::self == pybind11::self)
    .def(pybind11::self != pybind11::self)
    .def(pybind11::self += pybind11::self)
    .def(pybind11::self + pybind11::self)
    .def(pybind11::self -= pybind11::self)
    .def(pybind11::self - pybind11::self)
    .def(pybind11::self *= pybind11::self)
    .def(pybind11::self * pybind11::self)
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


    pybind11::class_<GridPoint>(m, "GridPoint")
    .def_readwrite("coords", &GridPoint::coords)
    .def_readwrite("value", &GridPoint::value)
    .def("MyHash", &GridPoint::MyHash)
    ;


    RegisterUniverse<Universe<GridPoint>>(m, "GridUniverse"); 
    RegisterMathSet<MathSet<GridPoint>>(m, "Grid"); 





   
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
    RegisterMatrices<std::complex<float>>(m, "CF");
    RegisterMatrices<std::complex<double>>(m, "CD");

    // Export the testing stuff
    export_testing(m);

    return m.ptr();
}


} // close namespace export_python
} // close namespace math 
} // close namespace pulsar

