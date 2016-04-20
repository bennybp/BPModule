/*! \file
 *
 * \brief Matrices and vectors blocked by irrep and spin
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_MATH__IRREPSPINMATRIX_HPP_
#define PULSAR_GUARD_MATH__IRREPSPINMATRIX_HPP_

#include "pulsar/math/SimpleMatrix.hpp"
#include "pulsar/math/BlockByIrrepSpin.hpp"


namespace pulsar{
namespace math{

template<typename T>
using IrrepSpinMatrix = BlockByIrrepSpin<SimpleMatrix<T>>;

template<typename T>
using IrrepSpinVector = BlockByIrrepSpin<SimpleVector<T>>;

// Explicit instantiations
extern template class BlockByIrrepSpin<SimpleMatrixF>;
extern template class BlockByIrrepSpin<SimpleMatrixD>;
extern template class BlockByIrrepSpin<SimpleMatrixCF>;
extern template class BlockByIrrepSpin<SimpleMatrixCD>;
extern template class BlockByIrrepSpin<SimpleVectorF>;
extern template class BlockByIrrepSpin<SimpleVectorD>;
extern template class BlockByIrrepSpin<SimpleVectorCF>;
extern template class BlockByIrrepSpin<SimpleVectorCD>;


typedef BlockByIrrepSpin<SimpleMatrixF>  IrrepSpinMatrixF;
typedef BlockByIrrepSpin<SimpleMatrixD>  IrrepSpinMatrixD;
typedef BlockByIrrepSpin<SimpleMatrixCF> IrrepSpinMatrixCF;
typedef BlockByIrrepSpin<SimpleMatrixCD> IrrepSpinMatrixCD;
typedef BlockByIrrepSpin<SimpleVectorF>  IrrepSpinVectorF;
typedef BlockByIrrepSpin<SimpleVectorD>  IrrepSpinVectorD;
typedef BlockByIrrepSpin<SimpleVectorCF> IrrepSpinVectorCF;
typedef BlockByIrrepSpin<SimpleVectorCD> IrrepSpinVectorCD;


} // close namespace math
} // close namespace pulsar

#endif
