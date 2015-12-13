/*! \file
 *
 * \brief Python exports for exceptions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bpmodule/python_helper/Pybind11.hpp"
#include "bpmodule/exception/GeneralException.hpp"

using namespace bpmodule::exception;

namespace bpmodule {
namespace exception {
namespace export_python {


PYBIND11_PLUGIN(exception)
{
    pybind11::module m("exception", "BPModule exceptions");

    pybind11::class_<GeneralException>(m, "GeneralException")
    .def(pybind11::init<const std::string &>())
    .def("AppendInfo", static_cast<void(GeneralException::*)(const std::string &, const std::string &)>(&GeneralException::AppendInfo))
    .def("ExceptionString", &GeneralException::ExceptionString)
    ;

    return m.ptr();
}


} // close namespace export_python
} // close namespace exception
} // close namespace bpmodule

