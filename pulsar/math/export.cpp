/*! \file
 *
 * \brief Python exports for math
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "pulsar/util/Pybind11.hpp"
#include "pulsar/math/Factorial.hpp"
#include "pulsar/math/Universe.hpp"
#include "pulsar/math/Point.hpp"
#include "pulsar/math/Grid.hpp"
#include "pulsar/math/Irrep.hpp"
#include "pulsar/math/RegisterMathSet.hpp"
#include "pulsar/math/IrrepSpinMatrix.hpp"
#include "pulsar/math/EigenImpl.hpp"
#include "pulsar/output/GlobalOutput.hpp"
#include "pulsar/math/RegisterTensor.hpp"


//! \todo Export exact casts? Or have the equivalent with python?

namespace pulsar{


void export_math(pybind11::module & m)
{
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
    .def("get_coord", &Point::get_coord)
    .def("set_coords", static_cast<void (Point::*)(double, double, double)>(&Point::set_coords))
    .def("set_coords", static_cast<void (Point::*)(const Point::CoordType &)>(&Point::set_coords))
    .def("set_coord", static_cast<void (Point::*)(int,double)>(&Point::set_coord))
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

    register_Universe<Universe<double>>(m, "DoubleUniverse"); 
    register_MathSet<MathSet<double>>(m, "DoubleSet"); 
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
   
    using SharedMatrix=std::shared_ptr<MatrixDImpl>;
    using SharedVector=std::shared_ptr<VectorDImpl>;

    export_tensor_impl<Rank3DImpl,PyRank3DImpl>(m,"Rank3DImpl");
    export_tensor_impl<MatrixDImpl,PyMatrixDImpl>(m,"MatrixImplD");
    export_tensor_impl<VectorDImpl,PyVectorDImpl>(m,"VectorImplD");  
    export_eigen_x_impl<EigenMatrixImpl,Eigen::MatrixXd,MatrixDImpl>(m,"EigenMatrixImpl");
    export_eigen_x_impl<EigenVectorImpl,Eigen::VectorXd,VectorDImpl>(m,"EigenVectorImpl");
    export_eigen_x_impl<EigenTensorImpl<3>,Eigen::Tensor<double,3>,Rank3DImpl>(m,"EigenTensorImpl");
    export_irrep_spin_X<IrrepSpinMatrixD,SharedMatrix>(m,"BlockedEigenMatrix");
    export_irrep_spin_X<IrrepSpinVectorD,SharedVector>(m,"BlockedEigenVector");
     
}

} // close namespace pulsar

