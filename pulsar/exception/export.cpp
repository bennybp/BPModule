/*! \file
 *
 * \brief Python exports for exceptions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include <pybind11/pybind11.h>
#include "pulsar/exception/Exceptions.hpp"

using namespace pulsar::exception;

namespace pulsar{
namespace exception {
namespace export_python {


void export_pybind11(pybind11::module & mtop)
{
    pybind11::module m = mtop.def_submodule("exception", "Pulsar exceptions");

    pybind11::class_<GeneralException>(m, "GeneralException")
    .def(pybind11::init<const std::string &>())
    .def("AppendInfo", static_cast<void(GeneralException::*)(const std::string &, const std::string &)>(&GeneralException::AppendInfo))
    .def("what", &GeneralException::what)
    ;
}


} // close namespace export_python
} // close namespace exception
} // close namespace pulsar

