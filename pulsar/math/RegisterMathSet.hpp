/*! \file
 *
 * \brief Registration helper for exporting MathSet and Universe
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#ifndef PULSAR_GUARD_MATH__REGISTERMATHSET_HPP_
#define PULSAR_GUARD_MATH__REGISTERMATHSET_HPP_

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include "pulsar/math/Universe.hpp"
#include "pulsar/math/MathSet.hpp"

PYBIND11_DECLARE_HOLDER_TYPE(T,std::shared_ptr<T>)

//! \todo Export exact casts? Or have the equivalent with python?

namespace pulsar{


/*! \brief Registers a MathSet
 *
 * To be called from within a python export block
 *
 * \warning Don't forget to register the universe also!
 *
 * \tparam T The type being held within the math set
 */
template<typename T>
void register_MathSet(pybind11::module & m,
                     const char * mathsetname)
{
    typedef typename T::value_type value_type;
    typedef std::shared_ptr<typename T::Universe_t> shared_universe;
    
    // Register the MathSet
    pybind11::class_<T>(m, mathsetname)
    .def(pybind11::init<shared_universe, bool>())
    .def(pybind11::init<shared_universe,std::set<size_t>>())
    .def(pybind11::init<const T &>())
    .def(pybind11::init<const T &,bool>())
    .def("clone",&T::clone)
    .def("my_hash", &T::my_hash)
    .def("clear",&T::clear)
    .def("size", &T::size)
    .def("idx",  &T::idx)
    .def("as_universe", &T::as_universe)
    .def("get_universe", &T::get_universe)
    .def("count", &T::count)
    .def("count_idx", &T::count_idx)
    .def("insert_idx", &T::insert_idx)
    .def("insert", static_cast<T &(T::*)(const value_type &)>(&T::insert))
    .def("union_assign", static_cast<T &(T::*)(const T &)>(&T::union_assign),
                        pybind11::return_value_policy::reference)
    .def("set_union", static_cast<T (T::*)(const T &) const>(&T::set_union))
    .def("intersection_assign", &T::intersection_assign,
                               pybind11::return_value_policy::reference)
    .def("intersection", &T::intersection)
    .def("difference_assign", &T::difference_assign,
                             pybind11::return_value_policy::reference)
    .def("difference", &T::difference)
    .def("complement",&T::complement,"Returns the complement of this")
    .def("is_proper_subset_of", &T::is_proper_subset_of)
    .def("is_subset_of", &T::is_subset_of)
    .def("is_proper_superset_of", &T::is_proper_superset_of)
    .def("is_superset_of", &T::is_superset_of)
    .def("transform", &T::transform)
    .def("partition", &T::partition)
    .def(pybind11::self += pybind11::self, pybind11::return_value_policy::reference)
    .def(pybind11::self + pybind11::self)
    .def(pybind11::self -= pybind11::self, pybind11::return_value_policy::reference)
    .def(pybind11::self - pybind11::self)
    .def(pybind11::self /= pybind11::self, pybind11::return_value_policy::reference)
    .def(pybind11::self / pybind11::self)
    .def(pybind11::self >= pybind11::self)
    .def(pybind11::self > pybind11::self)
    .def(pybind11::self <= pybind11::self)
    .def(pybind11::self < pybind11::self)
    .def(pybind11::self == pybind11::self)
    .def("__len__",         &T::size)
    .def("__contains__",    &T::count)
    .def("__str__",&T::to_string)
    .def("__iter__", [](const T & t) { return pybind11::make_iterator(t.begin(), t.end()); },
                     pybind11::keep_alive<0, 1>() )
    ;
}


template<typename T>
void register_Universe(pybind11::module & m,
                      const char * universename)
{
    typedef typename T::value_type value_type;


    // Register the universe
    pybind11::class_<T,std::shared_ptr<T>>(m, universename)
    .def(pybind11::init<>())
    .def(pybind11::init<const T &>())
    .def("__init__",[](T& unv,std::vector<value_type> args){
        new (&unv) T;
        unv.insert(args.begin(),args.end());
    })
    .def("size", &T::size)
    .def("my_hash", &T::my_hash)
    .def("idx",  &T::idx)
    .def("at",   &T::at, pybind11::return_value_policy::copy)
    .def("count", &T::count)
    .def("insert", static_cast<T &(T::*)(const value_type &)>(&T::insert))
    .def("union_assign", static_cast<T &(T::*)(const T &)>(&T::union_assign),
                        pybind11::return_value_policy::reference)
    .def("set_union", static_cast<T (T::*)(const T &) const>(&T::set_union))
    .def("intersection_assign", &T::intersection_assign,
                               pybind11::return_value_policy::reference)
    .def("intersection", &T::intersection)
    .def("difference_assign", &T::difference_assign,
                             pybind11::return_value_policy::reference)
    .def("difference", &T::difference)
    .def("is_proper_subset_of", &T::is_proper_subset_of)
    .def("is_subset_of", &T::is_subset_of)
    .def("is_proper_superset_of", &T::is_proper_superset_of)
    .def("is_superset_of", &T::is_superset_of)
    .def(pybind11::self += pybind11::self, pybind11::return_value_policy::reference)
    .def(pybind11::self + pybind11::self)
    .def(pybind11::self -= pybind11::self, pybind11::return_value_policy::reference)
    .def(pybind11::self - pybind11::self)
    .def(pybind11::self /= pybind11::self, pybind11::return_value_policy::reference)
    .def(pybind11::self / pybind11::self)
    .def(pybind11::self >= pybind11::self)
    .def(pybind11::self > pybind11::self)
    .def(pybind11::self <= pybind11::self)
    .def(pybind11::self < pybind11::self)
    .def(pybind11::self == pybind11::self)
    .def(pybind11::self != pybind11::self)
    .def("__len__",         &T::size)
    .def("__contains__",    &T::count)
    .def("__getitem__",     &T::at)
    .def("__str__",&T::to_string)
    .def("__iter__", [](const T & t) { return pybind11::make_iterator(t.begin(), t.end()); },
                     pybind11::keep_alive<0, 1>() )
    ;

}


} // close namespace pulsar

#endif

