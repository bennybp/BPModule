#ifndef _GUARD_REGISTERUIDPOINTER_HPP_
#define _GUARD_REGISTERUIDPOINTER_HPP_

#include <functional>
#include <boost/python/object.hpp>
#include <boost/python/class.hpp>
#include <boost/python/return_internal_reference.hpp>

#include "bpmodule/python_helper/Convert.hpp"
#include "bpmodule/datastore/UIDPointer.hpp"


using namespace boost::python;



namespace bpmodule {
namespace datastore {

namespace detail {




/*! \brief Adds the type to the python registry
 *
 * in the cpp file
 */
void AddCreator_(const char * classname, std::function<boost::python::object(const boost::python::object)> func);


 

/*! \brief A helper that creates UIDPointer from the specified type
 */
template<typename T>
boost::python::object CreateUIDPointer_(const boost::python::object & obj)
{
    T cobj = python_helper::ConvertToCpp<T>(obj);
    return boost::python::object(UIDPointer<T>(cobj));
}



} // close namespace detail



/*! \brief creates a python object containing a UID pointer
 *
 * The type held within the object must have been registered first
 */
boost::python::object MakeUIDPointerPy(const boost::python::object & obj);





/*! \brief Registers a UIDPointer to python
 *
 * To be called from within a python export block
 */
template<typename T>
void RegisterUIDPointer(const char * classname)
{
    std::string pyname = std::string("UIDPointer_") + classname;

    class_<UIDPointer<T>>(pyname.c_str(), init<>())
    .def(init<const T &>())
    .def("Valid", &UIDPointer<T>::Valid)
    .def("UID", &UIDPointer<T>::UID)
    .def("Set", &UIDPointer<T>::Set)
    .def("Get", &UIDPointer<T>::Get, return_internal_reference<>())
    ;

    // add it to the factory
    std::function<boost::python::object(const boost::python::object)> cfunc = std::bind(detail::CreateUIDPointer_<T>, std::placeholders::_1);
    detail::AddCreator_(classname, cfunc);
}



} // close namespace datastore
} // close namespace bpmodule



#endif

