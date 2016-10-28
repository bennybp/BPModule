/*! \file
 *
 * \brief Matrices and vectors blocked by irrep and spin
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#pragma once

#include "pulsar/math/TensorImpl.hpp"
#include "pulsar/math/BlockByIrrepSpin.hpp"


namespace pulsar{

template<typename DaType>
using IrrepSpinMatrix = BlockByIrrepSpin<std::shared_ptr<TensorImpl<2, DaType>>>;

template<typename DaType>
using IrrepSpinVector = BlockByIrrepSpin<std::shared_ptr<TensorImpl<1, DaType>>>;

template<typename DaType>
using IrrepSpinScalar = BlockByIrrepSpin<DaType>;

typedef IrrepSpinMatrix<double>  IrrepSpinMatrixD;
typedef IrrepSpinVector<double>  IrrepSpinVectorD;

} // close namespace pulsar
