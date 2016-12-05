/*! \file
 *
 * \brief Python exports for exceptions
 */

#include "pulsar/exception/PulsarException.hpp"
#include "pulsar/util/Pybind11.hpp"

namespace py = pybind11;

namespace pulsar {

void export_exception(py::module & m)
{
    // Disambiguates which AppendInfo we export from PulsarException
    using AppInf =
        void (PulsarException::*)(const std::string &, const std::string &);

    py::class_<PulsarException>(m, "PulsarException_")
        .def(py::init<const std::string &>())
        .def("append_info", static_cast<AppInf>(&PulsarException::append_info))
        .def("what", &PulsarException::what);
}

} // close namespace pulsar
