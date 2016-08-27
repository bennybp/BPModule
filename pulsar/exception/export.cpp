/*! \file
 *
 * \brief Python exports for exceptions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include <pybind11/pybind11.h>
#include "pulsar/exception/Exceptions.hpp"

using namespace pulsar::exception;
namespace py = pybind11;

namespace pulsar {
namespace exception {
namespace export_python {


void export_pybind11(py::module & mtop)
{
    //! \todo Document arguments? They aren't really meant to be used
    //        directly by users from python (should go through the
    //        python class)
    py::module m = mtop.def_submodule("exception",
                                      "Exceptions for the Pulsar framework");

    py::class_<GeneralException>(m, "GeneralException")
    .def(py::init<const std::string &>())
    .def("append_info", static_cast<void(GeneralException::*)(const std::string &, const std::string &)>(&GeneralException::append_info))
    .def("what", &GeneralException::what)
    ;
}


} // close namespace export_python
} // close namespace exception
} // close namespace pulsar

