#ifndef _GUARD_REGISTERUIDPOINTER_HPP_
#define _GUARD_REGISTERUIDPOINTER_HPP_

#include <functional>
#include "bpmodule/python_helper/Convert.hpp"
#include "bpmodule/datastore/UIDPointer.hpp"



namespace bpmodule {
namespace datastore {

namespace detail {




/*! \brief Adds the type to the python registry
 *
 * in the cpp file
 */
void AddCreator_(const char * classname, std::function<pybind11::object(pybind11::object)> func);


 

/*! \brief A helper that creates UIDPointer from the specified type
 */
template<typename T>
pybind11::object CreateUIDPointer_(pybind11::object obj)
{
    T cobj = python_helper::ConvertToCpp2<T>(obj);
    return pybind11::cast(UIDPointer<T>(cobj));
}



} // close namespace detail



/*! \brief creates a python object containing a UID pointer
 *
 * The type held within the object must have been registered first
 */
pybind11::object MakeUIDPointerPy(pybind11::object obj);





/*! \brief Registers a UIDPointer to python
 *
 * To be called from within a python export block
 */
template<typename T>
void RegisterUIDPointer(pybind11::module & m, const char * classname)
{
    std::string pyname = std::string("UIDPointer_") + classname;

    pybind11::class_<UIDPointer<T>>(m, pyname.c_str())
    .def(pybind11::init<>())
    .def(pybind11::init<const T &>())
    .def("Valid", &UIDPointer<T>::Valid)
    .def("UID", &UIDPointer<T>::UID)
    .def("Set", &UIDPointer<T>::Set)
    .def("Get", &UIDPointer<T>::Get, pybind11::return_value_policy::reference_internal)
    ;

    // add it to the factory
    std::function<pybind11::object(pybind11::object)> cfunc = std::bind(detail::CreateUIDPointer_<T>, std::placeholders::_1);
    detail::AddCreator_(classname, cfunc);
}



} // close namespace datastore
} // close namespace bpmodule



#endif

