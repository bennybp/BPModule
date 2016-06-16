/*! \file
 *
 * \brief Matrices and vectors blocked by irrep and spin
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_MATH__IRREPSPINMATRIX_HPP_
#define PULSAR_GUARD_MATH__IRREPSPINMATRIX_HPP_

#include "pulsar/tensor/TensorImpl.hpp"
#include "pulsar/math/BlockByIrrepSpin.hpp"


namespace pulsar{
namespace math{

template<typename T>
using IrrepSpinMatrix = BlockByIrrepSpin<std::shared_ptr<tensor::TensorImpl<2, T>>>;

template<typename T>
using IrrepSpinVector = BlockByIrrepSpin<std::shared_ptr<tensor::TensorImpl<1, T>>>;

template<typename T>
using IrrepSpinScalar = BlockByIrrepSpin<T>;

typedef IrrepSpinMatrix<double>  IrrepSpinMatrixD;
typedef IrrepSpinVector<double>  IrrepSpinVectorD;


} // close namespace math
} // close namespace pulsar

#endif
