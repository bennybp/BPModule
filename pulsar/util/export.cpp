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
    m.def("set_cmdline", set_cmdline);
    m.def("clear_cmdline", clear_cmdline);

    //////////////////////////
    // Memwatch
    //////////////////////////
    m.def("memwatch_running", memwatch_running);
    m.def("memwatch_allocated", memwatch_allocated);
    m.def("memwatch_setlimit", memwatch_setlimit);
    m.def("memwatch_getlimit", memwatch_getlimit);
}


} // close namespace export_python
} // close namespace util
} // close namespace pulsar

