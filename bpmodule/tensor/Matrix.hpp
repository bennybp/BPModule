#ifndef _GUARD_MATRIX_HPP_
#define _GUARD_MATRIX_HPP_


// Main include for libelemental
#include <El.h>

namespace tensor {

template<typename T>
using Matrix = El::Matrix<T>;

typedef Matrix<float> MatrixF;
typedef Matrix<double> MatrixD;
typedef Matrix<std::complex<float>> MatrixCF;
typedef Matrix<std::complex<double>> MatrixCD;


};


#endif
