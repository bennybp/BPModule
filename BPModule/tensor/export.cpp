#include "BPModule/tensor/Tensor.hpp"

#include <boost/python.hpp>
using namespace boost::python;
namespace bpy = boost::python;



namespace bpmodule {
namespace tensor {
namespace export_python {

BOOST_PYTHON_MODULE(bpmodule_tensor)
{
    def("InitTensor", InitTensor);
    def("FinalizeTensor", FinalizeTensor);
}


} // close namespace export_python
} // close namespace tensor
} // close namespace bpmodule

