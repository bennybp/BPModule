/*! \file
 *
 * \brief Python exports for the tensor library
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/list.hpp> // needed since InitTensor takes lists
                                 //! \todo move python parsing stuff to here?

#include "bpmodule/tensor/Tensor.hpp"

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

