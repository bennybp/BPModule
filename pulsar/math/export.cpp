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
#include "pulsar/math/Irrep.hpp"
#include "pulsar/math/RegisterMathSet.hpp"
#include "pulsar/math/BlockByIrrepSpin.hpp"
#include "pulsar/math/EigenCommon.hpp"


//declared in RegisterMathSet
//!\todo - fix multiple definitions of this in pybind11
//PYBIND11_DECLARE_HOLDER_TYPE(T,std::shared_ptr<T>);



//! \todo Export exact casts? Or have the equivalent with python?

namespace pulsar{
namespace math {
namespace export_python {


// in testing_export.cpp
void export_testing(pybind11::module & m);

///Function that exports the various IrrepSpin tensors
///T is the actual type, T2 is the type it wraps, Name is the python class name
template<typename T,typename T2>
void  export_irrep_spin_X(pybind11::module& m,const char* Name)
{   pybind11::class_<T>(m,Name)
    .def("has",&T::has)
    .def("get_irreps",&T:get_irreps)
    .def("get_spins",&T::get_spins)
    .def("get",static_cast<T2(T::*)(Irrep,int)>(&T::get))
    .def("set",static_cast<void(T::*)(Irrep,int,T2&&)>(&T::set))
    .def("same_structure",&T::same_structure)
    .def("my_hash",&T::my_hash)
    .def("__iter__", [](const T& t){
                        return pybind11::make_iterator(t.begin(),t.end());
                      },pybind11::keep_alive<0, 1>())
    .def(pybind11::self == pybind11::self)
    .def(pybind11::self != pybind11::self)
    ;
}



void export_pybind11(pybind11::module & mtop)
{
    pybind11::module m = mtop.def_submodule("math", "Some common math operations");

    m.def("factorial", factorial); 
    m.def("factorial_f", factorial_f); 
    m.def("factorial_d", factorial_d); 

    m.def("double_factorial", double_factorial); 
    m.def("double_factorial_f", double_factorial_f); 
    m.def("double_factorial_d", double_factorial_d); 

    m.def("double_2nm1_factorial", double_2nm1_factorial); 
    m.def("double_2nm1_factorial_f", double_2nm1_factorial_f); 
    m.def("double_2nm1_factorial_d", double_2nm1_factorial_d);



    //////////////////////////////////
    // Point/Grid
    //////////////////////////////////
    //! \todo If we need other types, we need a RegisterPoint/etc function
    //        (probably ok just having it in this file)
    //! \todo pybind11 buffers
    pybind11::class_<Point>(m, "Point")
    .def(pybind11::init<double, double, double>()) 
    .def(pybind11::init<const Point::CoordType &>()) 
    .def("get_coords", &Point::get_coords)
    .def("set_coords", static_cast<void (Point::*)(double, double, double)>(&Point::set_coords))
    .def("set_coords", static_cast<void (Point::*)(const Point::CoordType &)>(&Point::set_coords))
    .def("distance", &Point::distance)
    .def("magnitude", &Point::magnitude)
    .def("print", &Point::print)
    .def("my_hash", &Point::my_hash)
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
                        p.print(ss);
                        return ss.str();})
    ;


    pybind11::class_<GridPoint>(m, "GridPoint")
    .def_readwrite("coords", &GridPoint::coords)
    .def_readwrite("value", &GridPoint::value)
    .def("my_hash", &GridPoint::my_hash)
    ;


    register_Universe<Universe<GridPoint>>(m, "GridUniverse"); 
    register_MathSet<MathSet<GridPoint>>(m, "Grid"); 

  
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

    using SharedMatrix=std::shared_ptr<Eigen::MatrixXd>;
    using SharedVector=std::shared_ptr<Eigen::VectorXd>;
    
    export_irrep_spin_X<IrrepSpinMatrixD,SharedMatrix>(m,"IrrepSpinMatrixD");
    export_irrep_spin_X<IrrepSpinVectorD,SharedVector>(m,"IrrepSpinVectorD");                      
    
    // Export the testing stuff
    export_testing(m);
}


} // close namespace export_python
} // close namespace math 
} // close namespace pulsar

