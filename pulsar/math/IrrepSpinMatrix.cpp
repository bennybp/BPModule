/*! \file
 *
 * \brief Matrices and vectors blocked by irrep and spin
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bpmodule/math/IrrepSpinMatrix.hpp"


namespace bpmodule{
namespace math{


// Explicit instantiations
template class BlockByIrrepSpin<SimpleMatrixF>;
template class BlockByIrrepSpin<SimpleMatrixD>;
template class BlockByIrrepSpin<SimpleMatrixCF>;
template class BlockByIrrepSpin<SimpleMatrixCD>;
template class BlockByIrrepSpin<SimpleVectorF>;
template class BlockByIrrepSpin<SimpleVectorD>;
template class BlockByIrrepSpin<SimpleVectorCF>;
template class BlockByIrrepSpin<SimpleVectorCD>;

} // close namespace math
} // close namespace bpmodule

