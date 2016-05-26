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

PYBIND11_PLUGIN(tensor)
{
    pybind11::module m("tensor", "Matrix and Tensor classes");

    m.def("Init", Init);
    m.def("Finalize", Finalize);

    return m.ptr();
}


} // close namespace export_python
} // close namespace tensor
} // close namespace pulsar

