/*! \file
 *
 * \brief Python exports for the tensor library
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/tensor/Tensor.hpp"

#include <boost/python.hpp>
using namespace boost::python;


namespace bpmodule {
namespace tensor {
namespace export_python {

BOOST_PYTHON_MODULE(tensor)
{
    def("InitTensor", InitTensor);
    def("FinalizeTensor", FinalizeTensor);
}


} // close namespace export_python
} // close namespace tensor
} // close namespace bpmodule

