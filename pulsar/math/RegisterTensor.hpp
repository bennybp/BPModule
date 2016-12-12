/*! \file
 *
 * \brief  Contains templates for exporting tensors to Python
 */

#pragma once
#include "pulsar/util/Pybind11.hpp"
#include <bphash/types/memory.hpp>
#include "pulsar/math/Irrep.hpp"

namespace pulsar {


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
template<typename TensorT,typename EigenType,typename BaseType>
void export_eigen_x_impl(pybind11::module& m,const char* Name)
{
    pybind11::class_<TensorT,BaseType,std::shared_ptr<TensorT>>(m,Name)
    .def(pybind11::init<const EigenType&>())
    .def(pybind11::init<const TensorT&>())
    .def(pybind11::self == pybind11::self)
    .def(pybind11::self != pybind11::self)
    .def("my_hash",&TensorT::my_hash)
    .def("sizes",&TensorT::sizes)
    .def("get_value",&TensorT::get_value)
    .def("set_value",&TensorT::set_value)
    .def("get_matrix",[](const TensorT& tens){
        return *tens.get_matrix();}
        )
    ;
}


//Function that exports the various IrrepSpin tensors, TensorT is the final type
//TensorI is the type it wraps, and Name is the python class name
template<typename TensorT,typename TensorI>
void  export_irrep_spin_X(pybind11::module& m,const char* Name)
{   
    using get_sig=TensorI&(TensorT::*)(pulsar::Irrep,int);
    using set_sig=void(TensorT::*)(pulsar::Irrep,int,const TensorI&);
    pybind11::class_<TensorT,std::shared_ptr<TensorT>>(m,Name)
    .def(pybind11::init<>())
    .def(pybind11::init<const TensorT&>())
    .def("my_hash",&TensorT::my_hash)
    .def("has",&TensorT::has)
    .def("get_irreps",&TensorT::get_irreps)
    .def("get_spins",&TensorT::get_spins)
    .def("get",static_cast<get_sig>(&TensorT::get))
    .def("set",static_cast<set_sig>(&TensorT::set))
    .def("same_structure",[](const TensorT& t,const TensorT& t2)
    {
     return t.same_structure(t2);
    })
    .def("__repr__",[](const TensorT& t){
        pulsar::get_global_output()<<t;
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

}//End namespace pulsar

