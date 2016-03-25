/*! \file
 *
 * \brief Registration helper for exporting MathSet and Universe
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#ifndef BPMODULE_GUARD_MATH__REGISTERMATHSET_HPP_
#define BPMODULE_GUARD_MATH__REGISTERMATHSET_HPP_

#include "bpmodule/python/Pybind11.hpp"
#include "bpmodule/python/Pybind11_stl.hpp"
#include "bpmodule/python/Pybind11_functional.hpp"
#include "bpmodule/python/Pybind11_operators.hpp"
#include "bpmodule/python/Pybind11_iterators.hpp"
#include "bpmodule/math/Universe.hpp"
#include "bpmodule/math/MathSet.hpp"

PYBIND11_DECLARE_HOLDER_TYPE(T,std::shared_ptr<T>);

//! \todo Export exact casts? Or have the equivalent with python?

namespace bpmodule {
namespace math {


/*! \brief Registers a MathSet
 *
 * To be called from within a python export block
 *
 * \warning Don't forget to register the universe also!
 *
 * \tparam T The type being held within the math set
 */
template<typename T>
void RegisterMathSet(pybind11::module & m,
                     const char * mathsetname)
{
    typedef typename T::value_type value_type;

    // Register the iterator
    python::RegisterPyCopyIterator<T>(m, mathsetname);

    // Register the MathSet
    pybind11::class_<T>(m, mathsetname)
    .def(pybind11::init<std::shared_ptr<typename T::Universe_t>, bool>())
    .def(pybind11::init<const T &>())
    .def("Size", &T::Size)
    .def("Idx",  &T::Idx)
    .def("Contains", &T::Contains)
    .def("ContainsIdx", &T::ContainsIdx)
    .def("InsertIdx", &T::InsertIdx)
    .def("Insert", static_cast<T &(T::*)(const value_type &)>(&T::Insert))
    .def("UnionAssign", static_cast<T &(T::*)(const T &)>(&T::UnionAssign),
                        pybind11::return_value_policy::reference)
    .def("Union", static_cast<T (T::*)(const T &) const>(&T::Union))
    .def("IntersectionAssign", &T::IntersectionAssign,
                               pybind11::return_value_policy::reference)
    .def("Intersection", &T::Intersection)
    .def("DifferenceAssign", &T::DifferenceAssign,
                             pybind11::return_value_policy::reference)
    .def("Difference", &T::Difference)
    .def("Complement",&T::Complement,"Returns the complement of this")
    .def("IsProperSubsetOf", &T::IsProperSubsetOf)
    .def("IsSubsetOf", &T::IsSubsetOf)
    .def("IsProperSupersetOf", &T::IsProperSupersetOf)
    .def("IsSupersetOf", &T::IsSupersetOf)
    .def("Transform", &T::Transform)
    .def("Partition", &T::Partition)
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
    .def("__len__",         &T::Size)
    .def("__contains__",    &T::Contains)
    .def("__str__",&T::ToString)
    .def("__iter__", [](pybind11::object obj)
            {
                const T & cont = obj.cast<const T &>();
                return python::PyCopyIterator<T>(cont, cont.begin(), obj);
            })
    ;
}


template<typename T>
void RegisterUniverse(pybind11::module & m,
                      const char * universename)
{
    typedef typename T::value_type value_type;


    // Register the iterator
    python::RegisterPyCopyIterator<T>(m, universename);

    // Register the universe
    pybind11::class_<T,std::shared_ptr<T>>(m, universename)
    .def(pybind11::init<>())
    .def(pybind11::init<const T &>())
    .def("Size", &T::Size)
    .def("Idx",  &T::Idx)
    .def("At",   &T::At, pybind11::return_value_policy::copy)
    .def("Contains", &T::Contains)
    .def("ContainsIdx", &T::ContainsIdx)
    .def("Insert", static_cast<T &(T::*)(const value_type &)>(&T::Insert))
    .def("UnionAssign", static_cast<T &(T::*)(const T &)>(&T::UnionAssign),
                        pybind11::return_value_policy::reference)
    .def("Union", static_cast<T (T::*)(const T &) const>(&T::Union))
    .def("IntersectionAssign", &T::IntersectionAssign,
                               pybind11::return_value_policy::reference)
    .def("Intersection", &T::Intersection)
    .def("DifferenceAssign", &T::DifferenceAssign,
                             pybind11::return_value_policy::reference)
    .def("Difference", &T::Difference)
    .def("IsProperSubsetOf", &T::IsProperSubsetOf)
    .def("IsSubsetOf", &T::IsSubsetOf)
    .def("IsProperSupersetOf", &T::IsProperSupersetOf)
    .def("IsSupersetOf", &T::IsSupersetOf)
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
    .def("__len__",         &T::Size)
    .def("__contains__",    &T::Contains)
    .def("__getitem__",     &T::At)
    .def("__str__",&T::ToString)
    .def("__iter__", [](pybind11::object obj)
            {
                const T & cont = obj.cast<const T &>();
                return python::PyCopyIterator<T>(cont, cont.begin(), obj);
            })
    ;

}


} // close namespace math 
} // close namespace bpmodule

#endif

