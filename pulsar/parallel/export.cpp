/*! \file
 *
 * \brief Python exports for parallelization
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "pulsar/parallel/InitFinalize.hpp"
#include <pybind11/pybind11.h>

namespace pulsar{
namespace parallel {
namespace export_python {

void export_pybind11(pybind11::module & mtop)
{
    pybind11::module m = mtop.def_submodule("parallel", "Parallelization stuff");

    // Parallelization
    m.def("initialize", initialize);
    m.def("finalize", finalize);
}


} // close namespace export_python
} // close namespace parallel 
} // close namespace pulsar

