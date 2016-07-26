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
#include <pybind11/eigen.h>
#include "pulsar/math/Factorial.hpp"
#include "pulsar/math/Universe.hpp"
#include "pulsar/math/Point.hpp"
#include "pulsar/math/Grid.hpp"
#include "pulsar/math/Irrep.hpp"
#include "pulsar/math/RegisterMathSet.hpp"
#include "pulsar/math/IrrepSpinMatrix.hpp"
#include "pulsar/math/EigenCommon.hpp"
#include "pulsar/output/GlobalOutput.hpp"
//#include "external/pybind11/pybind11-source/include/pybind11/pytypes.h"


//declared in RegisterMathSet
//!\todo - fix multiple definitions of this in pybind11
//PYBIND11_DECLARE_HOLDER_TYPE(T,std::shared_ptr<T>);



//! \todo Export exact casts? Or have the equivalent with python?

namespace pulsar{
namespace math {
namespace export_python {


// in testing_export.cpp
void export_testing(pybind11::module & m);

//Function for exporting the abstract tensorimpl base class
template<typename ImplT,typename PyImplT>
void export_tensor_impl(pybind11::module& m,const char* Name)
{
    pybind11::class_<ImplT,std::unique_ptr<ImplT>,PyImplT>(m,Name)
    .def("sizes",&ImplT::sizes)
    .def("get_value",&ImplT::get_value)
    .def("set_value",&ImplT::set_value)
    .def("size",&ImplT::size)
    ;
}


//Function for exporting our eigen adapted tensors
template<typename TensorT,typename EigenType>
void export_eigen_x_impl(pybind11::module& m,const char* Name)
{
    using SharedX=std::shared_ptr<EigenType>;
    pybind11::class_<TensorT,std::shared_ptr<TensorT>>(m,Name)
    .def(pybind11::init<const SharedX&>())
    .def(pybind11::init<const EigenType&>())
    //.def(pybind11::init<EigenType&&>())
    .def("sizes",&TensorT::sizes)
    .def("get_value",&TensorT::get_value)
    .def("set_value",&TensorT::set_value)
    .def("get_matrix",&TensorT::get_matrix)
    ;
}


//Function that exports the various IrrepSpin tensors, TensorT is the final type
//TensorI is the type it wraps, and Name is the python class name
template<typename TensorT,typename TensorI>
void  export_irrep_spin_X(pybind11::module& m,const char* Name)
{   pybind11::class_<TensorT,std::shared_ptr<TensorT>>(m,Name)
    .def(pybind11::init<>())
    .def(pybind11::init<const TensorT&>())
    //.def(pybind11::init<TensorT&&>())
    .def("has",&TensorT::has)
    .def("get_irreps",&TensorT::get_irreps)
    .def("get_spins",&TensorT::get_spins)
    .def("get",[](TensorT& t,Irrep ir,int spin){
                  return t.get(ir,spin);          
     })
    .def("set",[](TensorT& t,Irrep ir,int spin,TensorI tensor){        
        t.set(ir,spin,tensor);
    })
    .def("__repr__",[](const TensorT& t){
        pulsar::output::get_global_output()<<t;
        return "";
    })
    //.def("same_structure",&TensorT::same_structure)
    //.def("my_hash",&TensorT::my_hash)
    .def("__iter__", [](const TensorT& t){
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
    .value("A",Irrep::A).value("Ag",Irrep::Ag).value("Au",Irrep::Au)
    .value("A'",Irrep::Ap).value("A''",Irrep::App).value("B",Irrep::B)
    .value("Bg",Irrep::Bg).value("Bu",Irrep::Bu)
    .value("A1", Irrep::A1).value("A2", Irrep::A2).value("B1", Irrep::B1)
    .value("B2", Irrep::B2).value("E1", Irrep::E1).value("E2", Irrep::E2)
    .value("A1g", Irrep::A1g).value("A2g", Irrep::A2g).value("B1g", Irrep::B1g)
    .value("B2g", Irrep::B2g).value("E1g", Irrep::E1g).value("E2g", Irrep::E2g)
    .value("A1u", Irrep::A1u).value("A2u", Irrep::A2u).value("B1u", Irrep::B1u)
    .value("B2u", Irrep::B2u).value("E1u", Irrep::E1u).value("E2u", Irrep::E2u)
    ;
    
    pybind11::enum_<Spin>(m,"Spin")
    .value("alpha",Spin::alpha).value("beta",Spin::beta);
   
    using MatrixImplD=TensorImpl<2,double>;
    using PyMatrixImplD=TensorImpl_Py<2,double>;
    using VectorImplD=TensorImpl<1,double>;
    using PyVectorImplD=TensorImpl_Py<1,double>;
    using SharedMatrix=std::shared_ptr<EigenMatrixImpl>;
    using SharedVector=std::shared_ptr<EigenVectorImpl>;

    export_tensor_impl<MatrixImplD,PyMatrixImplD>(m,"MatrixImplD");
    export_tensor_impl<VectorImplD,PyVectorImplD>(m,"VectorImplD");  
    export_eigen_x_impl<EigenMatrixImpl,Eigen::MatrixXd>(m,"EigenMatrixImpl");
    export_eigen_x_impl<EigenVectorImpl,Eigen::VectorXd>(m,"EigenVectorImpl");
    export_irrep_spin_X<IrrepSpinMatrixD,SharedMatrix>(m,"IrrepSpinMatrixD");
    export_irrep_spin_X<IrrepSpinVectorD,SharedVector>(m,"IrrepSpinVectorD");                      
     
    
    // Export the testing stuff
    export_testing(m);
}


} // close namespace export_python
} // close namespace math 
} // close namespace pulsar

