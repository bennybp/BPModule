/*! \file
 *
 * \brief Register SimpleMatrix/IrrepSpinMatrix (and vectors) for use from Python
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_MATH__REGISTERMATRICES_HPP_
#define PULSAR_GUARD_MATH__REGISTERMATRICES_HPP_

#include "pulsar/python/Pybind11.hpp"
#include "pulsar/python/Pybind11_stl.hpp"
#include "pulsar/python/Pybind11_functional.hpp"
#include "pulsar/python/Pybind11_operators.hpp"
#include "pulsar/math/SimpleMatrix.hpp"


PYBIND11_DECLARE_HOLDER_TYPE(T,std::shared_ptr<T>);


namespace pulsar{
namespace math {


template<typename T>
static void RegisterMatrices(pybind11::module & m, const char * type)
{
    std::string name("SimpleMatrix");
    std::string vname("SimpleVector");
    std::string isname("IrrepSpinMatrix");
    std::string isvname("IrrepSpinVector");

    name += type;
    vname += type;
    isname += type;
    isvname += type;

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
                   pybind11::format_descriptor<T>::value(),  /* Python struct-style format descriptor */
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
                   pybind11::format_descriptor<T>::value(), /* Python struct-style format descriptor */
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
            .def("Get", static_cast<SimpleMatrix<T> &(IrrepSpinMatrix<T>::*)(Irrep, int)>(&IrrepSpinMatrix<T>::Get))
            .def("Set", &IrrepSpinMatrix<T>::Set)
            .def("Erase", &IrrepSpinMatrix<T>::Erase)
            .def("Clear", &IrrepSpinMatrix<T>::Clear)
            .def("MyHash", &SimpleMatrix<T>::MyHash)
    ;

    pybind11::class_<IrrepSpinVector<T>, std::shared_ptr<IrrepSpinVector<T>>> isvclass(m, isvname.c_str(), mclass);
    isvclass.def(pybind11::init<>())
            .def(pybind11::init<const IrrepSpinVector<T> &>())
            .def(pybind11::self == pybind11::self)
            .def("Has", &IrrepSpinVector<T>::Has)
            .def("Get", static_cast<SimpleVector<T> &(IrrepSpinVector<T>::*)(Irrep, int)>(&IrrepSpinVector<T>::Get))
            .def("Set", &IrrepSpinVector<T>::Set)
            .def("Erase", &IrrepSpinVector<T>::Erase)
            .def("Clear", &IrrepSpinVector<T>::Clear)
            .def("MyHash", &SimpleVector<T>::MyHash)
    ;
}


} // close namespace math 
} // close namespace pulsar

#endif

