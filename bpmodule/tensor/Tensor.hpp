#ifndef _GUARD_TENSOR_HPP_
#define _GUARD_TENSOR_HPP_

#include "bpmodule/python_helper/BoostPython_fwd.hpp"


namespace bpmodule {
namespace tensor {

void InitTensor(const boost::python::list & argv);
void FinalizeTensor(void);


} // close namespace tensor
} // close namespace bpmodule


#endif
