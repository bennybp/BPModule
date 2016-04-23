/*! \file
 *
 * \brief Python exports for miscellaneous utilities
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include "pulsar/python/Pybind11.hpp"
#include "pulsar/python/Pybind11_stl.hpp"
#include "pulsar/python/Pybind11_operators.hpp"
#include "pulsar/util/Cmdline.hpp"
#include "pulsar/util/Memwatch.hpp"
#include "pulsar/util/bphash/Hash.hpp"


namespace pulsar{
namespace util {
namespace export_python {


PYBIND11_PLUGIN(util)
{
    pybind11::module m("util", "Miscellaneous utilities");

    // command line
    m.def("SetCmdline", SetCmdline);
    m.def("ClearCmdline", ClearCmdline);

    //////////////////////////
    // Hashing
    //////////////////////////
    pybind11::class_<Hash>(m, "Hash")
    .def("String", &Hash::String)
    .def("__str__", &Hash::String)
    .def(pybind11::self == pybind11::self)
    .def(pybind11::self != pybind11::self)
    .def(pybind11::self < pybind11::self)
    .def(pybind11::self <= pybind11::self)
    .def(pybind11::self > pybind11::self)
    .def(pybind11::self >= pybind11::self)
    ;

    //////////////////////////
    // Memwatch
    //////////////////////////
    m.def("Memwatch_running", Memwatch_running);
    m.def("Memwatch_allocated", Memwatch_allocated);
    m.def("Memwatch_setlimit", Memwatch_setlimit);
    m.def("Memwatch_getlimit", Memwatch_getlimit);

    return m.ptr();
}


} // close namespace export_python
} // close namespace util
} // close namespace pulsar

