#pragma once

#include <Eigen/Dense>
#include <bphash/Hasher.hpp>

namespace bphash {

template<typename S,  // Scalar
         int R,       // RowsAtCompileTime
         int C,       // ColsAtCompileTime
         int O,       // Options
         int MR,      // MaxRows
         int MC>      // MaxCols
void hash_object(const Eigen::Matrix<S, R, C, O, MR, MC> & mat, Hasher & h)
{
    h(mat.rows(), mat.cols());
    h(hash_pointer(mat.data(), mat.rows() * mat.cols()));
}

} // close namespace bphash
