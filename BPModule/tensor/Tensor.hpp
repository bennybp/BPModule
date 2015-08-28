#ifndef BPMODULE_TENSOR_HPP
#define BPMODULE_TENSOR_HPP

#include <ctf.hpp>


namespace bpmodule {
namespace tensor {

typedef CTF::Scalar<double, false> Scalar;
typedef CTF::Vector<double, false> Vector;
typedef CTF::Matrix<double, false> Matrix;
typedef CTF::Tensor<double, false> Tensor;


void InitTensor(int argc, char ** argv);
void FinalizeTensor(void);
CTF::World & GetWorld(void);


} // close namespace tensor
} // close namespace bpmodule


#endif
