/*! \file
 *
 * \brief A general-purpose dense matrix class
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bpmodule/math/SimpleMatrix.hpp"


namespace bpmodule{
namespace math{

template class SimpleMatrix<float>;
template class SimpleMatrix<double>;
template class SimpleMatrix<std::complex<float>>;
template class SimpleMatrix<std::complex<double>>;


} // close namespace math
} // close namespace bpmodule
