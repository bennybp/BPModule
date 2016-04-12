/*! \file
 *
 * \brief A map of spin and spatial symmetry to matrices
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef BPMODULE_GUARD_MATH__BLOCKMATRIX_HPP_
#define BPMODULE_GUARD_MATH__BLOCKMATRIX_HPP_

#include <map>
#include "bpmodule/math/SimpleMatrix.hpp"
#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/system/Irrep.hpp"

namespace bpmodule{
namespace math{


template<typename T>
class BlockedMatrix
{
    public:
        typedef std::pair<system::Irrep, int> IrrepSpin;
        typedef typename SimpleMatrix<T> MatrixType;
        typedef std::reference_wrapper<MatrixType> MatrixRefType;

        typedef std::map<system::Irrep, MatrixRefType> IrrepMap;
        typedef std::map<int, MatrixRefType> SpinMap;
        typedef std::map<IrrepSpin, SimpleMatrix<T>> IrrepSpinMap;


        bool HasMatrix(Irrep irrep, int spin) const
        {
            return matrices_.count({irrep, spin});
        }

        MatrixType & GetMatrix(Irrep irrep, int spin)
        {
            //! \todo need irrep to string
            //if(!HasMatrix(irrep, spin))
            //    throw exception::MathException("No matrix with this irrep/spin",
            //                                   "irrep", irrep, "spin", spin);    

            return matrices_.at({irrep, spin});
        }

        const MatrixType & GetMatrix(Irrep irrep, int spin) const
        {
            //! \todo need irrep to string
            //if(!HasMatrix(irrep, spin))
            //    throw exception::MathException("No matrix with this irrep/spin",
            //                                   "irrep", irrep, "spin", spin);    

            return matrices_.at({irrep, spin});
        }


    private:
        std::map<IrrepSpin, SimpleMatrix<T>> matrices_;
};



} // close namespace math
} // close namespace bpmodule

#endif
