/*! \file
 *
 * \brief Python exports for parallelization
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/parallel/InitFinalize.hpp"
#include "bpmodule/python/Pybind11.hpp"

namespace bpmodule {
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
} // close namespace bpmodule

