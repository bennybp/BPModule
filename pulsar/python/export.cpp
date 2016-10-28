/*! \file
 *
 * \brief Python exports for python helper functions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include <pybind11/pybind11.h>

namespace pulsar{

void export_python(pybind11::module & mtop)
{
    pybind11::module m = mtop.def_submodule("python", "Some helper functions for python stuff");
}

} // close namespace pulsar

