#ifndef _GUARD_TENSOR_HPP_
#define _GUARD_TENSOR_HPP_

#include <ambit/tensor.h>

#include "bpmodule/python_helper/BoostPython_fwd.hpp"


namespace bpmodule {
namespace tensor {

using namespace ambit;

void InitTensor(const boost::python::list & argv);
void FinalizeTensor(void);


} // close namespace tensor
} // close namespace bpmodule


#endif
