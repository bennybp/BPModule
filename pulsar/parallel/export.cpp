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

PYBIND11_PLUGIN(parallel)
{
    pybind11::module m("parallel", "Parallelization stuff");

    // Parallelization
    m.def("Init", Init);
    m.def("Finalize", Finalize);

    return m.ptr();
}


} // close namespace export_python
} // close namespace parallel 
} // close namespace pulsar

