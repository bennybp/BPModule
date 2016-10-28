/*! \file
 *
 * \brief Python exports for exceptions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include <pybind11/pybind11.h>
#include "pulsar/exception/Exceptions.hpp"

namespace py = pybind11;

namespace pulsar {

void export_exception(py::module & m)
{
    using E=void(GeneralException::*)(const std::string &, const std::string &);
    
    py::class_<GeneralException>(m, "GeneralException")
    .def(py::init<const std::string &>())
    .def("append_info", static_cast<E>(&GeneralException::append_info))
    .def("what", &GeneralException::what)
    ;
}

} // close namespace pulsar

