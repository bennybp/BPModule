/*! \file
 *
 * \brief Matrices and vectors blocked by irrep and spin
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_MATH__IRREPSPINMATRIX_HPP_
#define PULSAR_GUARD_MATH__IRREPSPINMATRIX_HPP_

#include "pulsar/math/TensorImpl.hpp"
#include "pulsar/math/BlockByIrrepSpin.hpp"


namespace pulsar{
namespace math{

template<typename DaType>
using IrrepSpinMatrix = BlockByIrrepSpin<std::shared_ptr<math::TensorImpl<2, DaType>>>;

template<typename DaType>
using IrrepSpinVector = BlockByIrrepSpin<std::shared_ptr<math::TensorImpl<1, DaType>>>;

template<typename DaType>
using IrrepSpinScalar = BlockByIrrepSpin<DaType>;

typedef IrrepSpinMatrix<double>  IrrepSpinMatrixD;
typedef IrrepSpinVector<double>  IrrepSpinVectorD;


} // close namespace math
} // close namespace pulsar

#endif
