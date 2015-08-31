#ifndef _GUARD_TENSOR_HPP_
#define _GUARD_TENSOR_HPP_

#include <ctf.hpp>

#include "bpmodule/python_helper/BoostPython_fwd.hpp"


namespace bpmodule {
namespace tensor {

typedef CTF::Scalar<double, false> Scalar;
typedef CTF::Vector<double, false> Vector;
typedef CTF::Matrix<double, false> Matrix;
typedef CTF::Tensor<double, false> Tensor;


void InitTensor(const boost::python::list & argv);
void FinalizeTensor(void);
CTF::World & GetWorld(void);


} // close namespace tensor
} // close namespace bpmodule


#endif
