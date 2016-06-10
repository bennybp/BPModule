/*! \file
 *
 * \brief Python exports for python helper functions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include <pybind11/pybind11.h>

namespace pulsar{
namespace python {
namespace export_python {


void export_pybind11(pybind11::module & mtop)
{
    pybind11::module m = mtop.def_submodule("python", "Some helper functions for python stuff");
}


} // close namespace export_python
} // close namespace python
} // close namespace pulsar

