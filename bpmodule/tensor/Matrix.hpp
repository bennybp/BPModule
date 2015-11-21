#ifndef _GUARD_MATRIX_HPP_
#define _GUARD_MATRIX_HPP_


// Main include for libelemental
// There are some compiler warnings in it

PRAGMA_WARNING_PUSH
PRAGMA_WARNING_IGNORE_UNUSED_PARAMETERS
PRAGMA_WARNING_IGNORE_UNUSED_VARIABLES
PRAGMA_WARNING_IGNORE_SHADOW
PRAGMA_WARNING_IGNORE_CONVERT
PRAGMA_WARNING_IGNORE_FP_CONVERT
#include <El.hpp>
PRAGMA_WARNING_POP

namespace bpmodule {
namespace tensor {

template<typename T>
using Matrix = El::Matrix<T>;

typedef Matrix<float> MatrixF;
typedef Matrix<double> MatrixD;
typedef Matrix<El::Complex<float>> MatrixCF;
typedef Matrix<El::Complex<double>> MatrixCD;


} // closing namespace tensor
} // closing namespace bpmodule


#endif
