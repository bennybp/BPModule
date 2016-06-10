/*! \file
 *
 * \brief Python exports for the tensor library
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <pybind11/pybind11.h>
#include "pulsar/tensor/InitFinalize.hpp"


namespace pulsar{
namespace tensor {
namespace export_python {

void export_pybind11(pybind11::module & mtop)
{
    pybind11::module m = mtop.def_submodule("tensor", "Matrix and Tensor classes");

    m.def("Init", Init);
    m.def("Finalize", Finalize);
}


} // close namespace export_python
} // close namespace tensor
} // close namespace pulsar

