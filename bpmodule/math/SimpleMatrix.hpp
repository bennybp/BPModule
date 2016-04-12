/*! \file
 *
 * \brief A general-purpose dense matrix storage class
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef BPMODULE_GUARD_MATH__SIMPLEMATRIX_HPP_
#define BPMODULE_GUARD_MATH__SIMPLEMATRIX_HPP_

#include <memory>
#include <complex>

#include "bpmodule/exception/Assert.hpp"
#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/util/Serialization.hpp"


namespace bpmodule{
namespace math{

/*! \brief A general-purpose dense matrix storage class
 *
 * This class only really stores matrix data. It does not
 * support matrix operations directly - copy to your
 * favorite matrix/tensor library to do heavy-duty operations.
 *
 * Storage is in row-major order.
 */
template<typename T>
class SimpleMatrix
{
    public:
        /*! \brief Construct a matrix of a given size */
        SimpleMatrix(size_t nrows, size_t ncols)
            : nrows_(nrows), ncols_(ncols), size_(nrows*ncols),
              data_(new T[nrows_*ncols_])
        { }

        /*! \brief Construct a matrix by copying data from a raw pointer */
        SimpleMatrix(size_t nrows, size_t ncols, T * data)
            : SimpleMatrix(nrows, ncols)
        {
            std::copy(data, data + size_, data_.get());
        }

        /*! \brief Construct a matrix by moving a unique_ptr */
        SimpleMatrix(size_t nrows, size_t ncols, std::unique_ptr<T []> && data)
            : nrows_(nrows), ncols_(ncols), size_(nrows*ncols),
              data_(std::move(data))
        { }


        /*! \brief Deep copy constructor */
        SimpleMatrix(const SimpleMatrix & rhs)
            : SimpleMatrix(rhs.nrows_, rhs.ncols_)
        {
            std::copy(rhs.data_.get(), rhs.data_.get() + size_, data_.get());
        }

        SimpleMatrix & operator=(const SimpleMatrix & rhs)
        {
            using std::swap;

            if(this != &rhs)
            {
                SimpleMatrix tmp(rhs);
                swap(*this, tmp);
            }
            return *this;
        }

        // compiler-generated ok
        SimpleMatrix(SimpleMatrix &&) = default;
        SimpleMatrix & operator=(SimpleMatrix &&) = default;

        /// Get the number of rows of this matrix
        size_t NRows(void) const noexcept { return nrows_; }

        /// Get the number of columns of this matrix
        size_t NCols(void) const noexcept { return ncols_; }

        /// Fill this matrix with zeroes
        void Zero(void)
        {
            std::fill(data_.get(), data_.get()+size_, static_cast<T>(0));
        }

        /*! \brief Obtain a reference to an element
         * 
         * \throw bpmodule::exception::MathException if the row
         *        or column is out of range (only if assertions are enabled)
         */
        T & operator()(size_t row, size_t col) ASSERTIONS_ONLY
        {
            #ifndef NDEBUG
            CheckIndices_(row, col);
            #endif
            return data_[row*ncols_+col]; 
        }

        /*! \brief Obtain a const reference to an element
         * 
         * \throw bpmodule::exception::MathException if the row
         *        or column is out of range (only if assertions are enabled)
         */
        const T & operator()(size_t row, size_t col) const ASSERTIONS_ONLY
        {
            #ifndef NDEBUG
            CheckIndices_(row, col);
            #endif
            return data_[row*ncols_+col]; 
        }

        /*! \brief Obtain a reference to an element
         * 
         * \throw bpmodule::exception::MathException if the row
         *        or column is out of range
         */
        T & At(size_t row, size_t col) ASSERTIONS_ONLY
        {
            CheckIndices_(row, col);
            return data_[row*ncols_+col]; 
        }

        /*! \brief Obtain a const reference to an element
         * 
         * \throw bpmodule::exception::MathException if the row
         *        or column is out of range
         */
        const T & At(size_t row, size_t col) const ASSERTIONS_ONLY
        {
            CheckIndices_(row, col);
            return data_[row*ncols_+col]; 
        }

        /// Obtain a pointer to the raw data
        T * Data(void) ASSERTIONS_ONLY
        {
            using namespace bpmodule::exception;
            if(!data_)
                throw MathException("Null pointer in SimpleMatrix");
            return data_.get();
        }

        /// Obtain a pointer to the raw data
        const T * Data(void) const ASSERTIONS_ONLY
        {
            using namespace bpmodule::exception;
            if(!data_)
                throw MathException("Null pointer in SimpleMatrix");
            return data_.get();
        }

        /*! \brief Release the raw data
         *
         * \note After this call, rows, cols, etc are all set
         *       to zero. So make sure you get that info beforehand
         */
        std::unique_ptr<T[]> Release(void)
        {
            nrows_ = ncols_ = size_ = 0;
            // rather than move, this assures data_ == nullptr
            return std::unique_ptr<T[]>(data_.release()); 
        }

        /// Take ownership of raw data
        void Take(size_t nrows, size_t ncols, std::unique_ptr<T[]> && data)
        {
            nrows_ = nrows;
            ncols_ = ncols;
            size_ = nrows*ncols;
            data_ = std::move(data);
        }
 
    
        /*! \brief For serialization only
         *
         * \warning NOT FOR USE OUTSIDE OF SERIALIZATION
         * \todo Replace if cereal fixes this
         */
        SimpleMatrix() = default;


    private:
        size_t nrows_;  //!< Number of rows
        size_t ncols_;  //!< Number of columns
        size_t size_;
        std::unique_ptr<T []> data_;  //!< Actual stored data

        void CheckIndices_(size_t row, size_t col) const
        {
            using namespace bpmodule::exception;
            if(row >= nrows_)
                throw MathException("Row out of range", "row", row, "nrows", nrows_);
            if(col >= ncols_)
                throw MathException("Column out of range", "col", col, "ncols", ncols_);
        }


        //! \name Serialization
        ///@{

        DECLARE_SERIALIZATION_FRIENDS

        template<class Archive>
        void save(Archive & ar) const
        {
            //! \todo might be slow. Do a row at a time or something?
            ar(nrows_, ncols_, size_);
            for(size_t i = 0; i < size_; i++)
                ar(data_[i]);
        }

        template<class Archive>
        void load(Archive & ar)
        {
            //! \todo might be slow. Do a row at a time or something?
            ar(nrows_, ncols_, size_);
            data_ = std::unique_ptr<T[]>(new T[size_]);
            for(size_t i = 0; i < size_; i++)
                ar(data_[i]);
        }

        ///@}
};


// Explicit instantiations
extern template class SimpleMatrix<float>;
extern template class SimpleMatrix<double>;
extern template class SimpleMatrix<std::complex<float>>;
extern template class SimpleMatrix<std::complex<double>>;



} // close namespace math
} // close namespace bpmodule

#endif