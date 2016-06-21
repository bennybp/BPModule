/*! \file
 *
 * \brief Handling and detection of python types (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "pulsar/python/Call.hpp"

using pulsar::exception::psr_assert;
using pulsar::exception::GeneralException;

namespace pulsar{
namespace python {


std::string get_py_class(const pybind11::object & obj)
{
    psr_assert<GeneralException>(obj.ptr() != nullptr, "Python object pointer is null");

    pybind11::object cls = obj.attr("__class__");
    pybind11::object name = cls.attr("__name__");
    return name.cast<std::string>();
}

bool is_none(const pybind11::object & obj)
{
    return get_py_class(obj) == "NoneType";
}

bool has_attr(const pybind11::object & obj, const std::string & attr)
{
    psr_assert<GeneralException>(obj.ptr() != nullptr, "Python object pointer is null");
    return PyObject_HasAttrString(obj.ptr(), attr.c_str());
}

bool is_callable(const pybind11::object & obj)
{
    psr_assert<GeneralException>(obj.ptr() != nullptr, "Python object pointer is null");
    return PyCallable_Check(obj.ptr());
}

bool has_callable_attr(const pybind11::object & obj, const std::string & attr)
{
    psr_assert<GeneralException>(obj.ptr() != nullptr, "Python object pointer is null");
    return has_attr(obj, attr) && is_callable(obj.attr(attr.c_str()));
}


} // close namespace python
} // close namespace pulsar

