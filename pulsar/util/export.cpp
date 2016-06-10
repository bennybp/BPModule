/*! \file
 *
 * \brief Python exports for miscellaneous utilities
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include "pulsar/util/Cmdline.hpp"
#include "pulsar/util/Memwatch.hpp"


namespace pulsar{
namespace util {
namespace export_python {


void export_pybind11(pybind11::module & mtop)
{
    pybind11::module m = mtop.def_submodule("util", "Miscellaneous utilities");

    // command line
    m.def("SetCmdline", SetCmdline);
    m.def("ClearCmdline", ClearCmdline);

    //////////////////////////
    // Memwatch
    //////////////////////////
    m.def("Memwatch_running", Memwatch_running);
    m.def("Memwatch_allocated", Memwatch_allocated);
    m.def("Memwatch_setlimit", Memwatch_setlimit);
    m.def("Memwatch_getlimit", Memwatch_getlimit);
}


} // close namespace export_python
} // close namespace util
} // close namespace pulsar

