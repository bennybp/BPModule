/*! \file
 *
 * \brief Registration helper for exporting MathSet and Universe
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/python/Pybind11.hpp"
#include "bpmodule/python/Pybind11_stl.hpp"
#include "bpmodule/python/Pybind11_functional.hpp"
#include "bpmodule/math/MathSet.hpp"
#include "bpmodule/math/Universe.hpp"

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
template<typename MathSet_t>
void RegisterMathSet(pybind11::module & m,
                     const char * mathsetname)
{
    // register MathSet
    pybind11::class_<MathSet_t>(m, mathsetname)
    .def(pybind11::init<std::shared_ptr<typename MathSet_t::Universe_t>, bool>())
    .def(pybind11::init<const MathSet_t &>())
    .def("append",(MathSet_t& (MathSet_t::*)(const typename MathSet_t::value_type &)) 
                  &MathSet_t::operator<<,"Adds a new element to the set")
    .def("append",(MathSet_t& (MathSet_t::*)(size_t)) 
                  &MathSet_t::operator<<,"Adds a new element to the set")
    .def("Transform", &MathSet_t::Transform,
                     "Perform a transformation on the elements of the set")
    .def("__iadd__",&MathSet_t::operator+=,
         "Makes this the union of this and other")
    .def("__isub__",&MathSet_t::operator-=,
         "Makes this the set difference of this and other")
    .def("__idiv__",&MathSet_t::operator/=,
         "Makes this the intersection of this and other")
    .def("__add__",&MathSet_t::operator+,"Returns union of this and other")
    .def("__sub__",&MathSet_t::operator-,
         "Returns set-difference of this and other")
    .def("__div__",&MathSet_t::operator/,
         "Returns the intersection of this and other")
    .def("Complement",&MathSet_t::Complement,"Returns the complement of this")
    .def("__str__",&MathSet_t::ToString,"Prints out the set");
}


template<typename Universe_t>
void RegisterUniverse(pybind11::module & m,
                      const char * universename)
{
    // Register the universe
    pybind11::class_<Universe_t,std::shared_ptr<Universe_t>>(m, universename)
    .def(pybind11::init<>())
    .def("append",&Universe_t::operator<<,"Adds a new universe to the set")
    .def("__iadd__",&Universe_t::operator+=,"Makes this the union of this and other")
    .def("__isub__",&Universe_t::operator-=,
         "Makes this the set difference of this and other")
    .def("__idiv__",&Universe_t::operator/=,
         "Makes this the intersection of this and other")
    .def("__add__",&Universe_t::operator+,"Returns union of this and other")
    .def("__sub__",&Universe_t::operator-,"Returns set-difference of this and other")
    .def("__div__",&Universe_t::operator/,
         "Returns the intersection of this and other")
    .def("__str__",&Universe_t::ToString,"Prints out the universe");
}


} // close namespace math 
} // close namespace bpmodule

