/*! \file
 *
 * \brief Python exports for parallelization
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/parallel/Parallel.hpp"
#include "bpmodule/python/Pybind11.hpp"

namespace bpmodule {
namespace parallel {
namespace export_python {

PYBIND11_PLUGIN(parallel)
{
    pybind11::module m("parallel", "Parallelization stuff");

    // Parallelization
    m.def("InitParallel", InitParallel);
    m.def("FinalizeParallel", FinalizeParallel);

    return m.ptr();
}


} // close namespace export_python
} // close namespace parallel 
} // close namespace bpmodule

