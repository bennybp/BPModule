/*! \file
 *
 * \brief Python exports for miscellaneous utilities
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include "bpmodule/util/FormatString.hpp"
#include "bpmodule/util/Cmdline.hpp"


//! \todo export the rest?

namespace bpmodule {
namespace util {
namespace export_python {



////////////////////////////
// Main boost python part
////////////////////////////

PYBIND11_PLUGIN(util)
{
    pybind11::module m("util", "Miscellaneous utilities");

    // format string
    m.def("FormatString_", FormatStringPy);

    // command line
    m.def("SetCmdline", SetCmdline);
    m.def("ClearCmdline", ClearCmdline);

    return m.ptr();
}


} // close namespace export_python
} // close namespace util
} // close namespace bpmodule

