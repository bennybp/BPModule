/*! \file
 *
 * \brief Python exports for python helper functions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bpmodule/python/Pybind11.hpp"
#include "bpmodule/python/Types.hpp"

namespace bpmodule {
namespace python {
namespace export_python {


PYBIND11_PLUGIN(python)
{
    pybind11::module m("python", "Some helper functions for python stuff");

    pybind11::enum_<PythonType>(m, "PythonType")
    .value("Bool", PythonType::Bool)
    .value("Int", PythonType::Int)
    .value("Float", PythonType::Float)
    .value("String", PythonType::String)
    .value("ListBool", PythonType::ListBool)
    .value("ListInt", PythonType::ListInt)
    .value("ListFloat", PythonType::ListFloat)
    .value("ListString", PythonType::ListString)
    .value("ListEmpty", PythonType::ListEmpty)
    .value("ListHetero", PythonType::ListHetero)
    .value("ListUnknown", PythonType::ListUnknown)
    .value("Tuple", PythonType::Tuple)
    .value("Dict", PythonType::Dict)
    .value("None", PythonType::None)
    .value("Unknown", PythonType::Unknown)
    ;


    return m.ptr();
}


} // close namespace export_python
} // close namespace python
} // close namespace bpmodule

