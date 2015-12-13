/*! \file
 *
 * \brief Python exports for the tensor library
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/python_helper/Pybind11.hpp"
#include "bpmodule/tensor/InitFinalize.hpp"


namespace bpmodule {
namespace tensor {
namespace export_python {

PYBIND11_PLUGIN(parallel)
{
    pybind11::module m("parallel", "Parallelization stuff");

    m.def("Init", Init);
    m.def("Finalize", Finalize);

    return m.ptr();
}


} // close namespace export_python
} // close namespace tensor
} // close namespace bpmodule

