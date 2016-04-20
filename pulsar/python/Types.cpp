/*! \file
 *
 * \brief Handling and detection of python types (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "pulsar/python/Call.hpp"

using pulsar::exception::Assert;
using pulsar::exception::GeneralException;

namespace pulsar{
namespace python {


std::string GetPyClass(const pybind11::object & obj)
{
    Assert<GeneralException>(obj.ptr() != nullptr, "Python object pointer is null");

    pybind11::object cls = obj.attr("__class__");
    pybind11::object name = cls.attr("__name__");
    return name.cast<std::string>();
}

bool IsNone(const pybind11::object & obj)
{
    return GetPyClass(obj) == "NoneType";
}

bool HasAttr(const pybind11::object & obj, const std::string & attr)
{
    Assert<GeneralException>(obj.ptr() != nullptr, "Python object pointer is null");
    return PyObject_HasAttrString(obj.ptr(), attr.c_str());
}

bool IsCallable(const pybind11::object & obj)
{
    Assert<GeneralException>(obj.ptr() != nullptr, "Python object pointer is null");
    return PyCallable_Check(obj.ptr());
}

bool HasCallableAttr(const pybind11::object & obj, const std::string & attr)
{
    Assert<GeneralException>(obj.ptr() != nullptr, "Python object pointer is null");
    return HasAttr(obj, attr) && IsCallable(obj.attr(attr.c_str()));
}


} // close namespace python
} // close namespace pulsar

