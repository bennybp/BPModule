/*! \file
 *
 * \brief Python exports for parallelization
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "pulsar/parallel/Parallel.hpp"
#include "pulsar/util/Pybind11.hpp"

namespace pulsar{

void export_parallel(pybind11::module & m)
{
    // Parallelization
    m.def("initialize", initialize);
    m.def("finalize", finalize);
    m.def("get_proc_id", get_proc_id);
    m.def("get_nproc", get_nproc);
}

} // close namespace pulsar

