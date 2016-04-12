/*! \file
 *
 * \brief General-purpose dense matrix & vector classes
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef BPMODULE_GUARD_MATH__SIMPLEMATRIX_HPP_
#define BPMODULE_GUARD_MATH__SIMPLEMATRIX_HPP_

#include <memory>
#include <complex>
#include <vector>
#include <algorithm>

#include "bpmodule/exception/Assert.hpp"
#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/util/Serialization.hpp"
#include "bpmodule/util/HashSerializable.hpp"


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
        /*! \brief Constructs an empty matrix */
        SimpleMatrix() : SimpleMatrix(0, 0) { }

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

        /*! \brief Construct a matrix by copying data from an std::vector */
        SimpleMatrix(size_t nrows, size_t ncols, const std::vector<T> & v)
            : SimpleMatrix(nrows, ncols)
        {
            if(v.size() != size_)
                throw exception::MathException("Vector has incompatible length", "vecsize", v.size(),
                                               "nrows", nrows, "ncols", ncols);
 
            std::copy(v.begin(), v.end(), data_.get()); 
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



        // compiler-generated not ok due to compiler bugs
        SimpleMatrix(SimpleMatrix && rhs) 
          : nrows_(std::move(rhs.nrows_)),
            ncols_(std::move(rhs.ncols_)),
            size_(std::move(rhs.size_)),
            data_(std::move(rhs.data_))
        { }

        SimpleMatrix & operator=(SimpleMatrix &&) = default;
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


        /*! \brief Comparison
         * 
         * Compares sizes and then elementwise. Values must exactly match
         * (ie, to all bits for floating point)
         */
        bool operator==(const SimpleMatrix & rhs) const
        {
            PRAGMA_WARNING_PUSH
            PRAGMA_WARNING_IGNORE_FP_EQUALITY

            if(!data_ && !rhs.data_)
                return true;
            else if(!data_ || !rhs.data_)
                return false;
            else
                return (nrows_ == rhs.nrows_ &&
                        ncols_ == rhs.ncols_ &&
                        std::equal(data_.get(), data_.get() + size_, rhs.data_.get()));

            PRAGMA_WARNING_POP
        }

        /// Get the number of rows of this matrix
        size_t NRows(void) const noexcept { return nrows_; }

        /// Get the number of columns of this matrix
        size_t NCols(void) const noexcept { return ncols_; }

        /// Get the total size this matrix
        size_t Size(void) const noexcept { return size_; }

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
        T & At(size_t row, size_t col)
        {
            CheckIndices_(row, col);
            return data_[row*ncols_+col]; 
        }

        /*! \brief Obtain a const reference to an element
         * 
         * \throw bpmodule::exception::MathException if the row
         *        or column is out of range
         */
        const T & At(size_t row, size_t col) const
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

        util::Hash MyHash(void) const
        {
            return util::HashSerializable(*this);
        } 
    


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


template<typename T>
class SimpleVector : public SimpleMatrix<T>
{
    public:
        /*! \brief Constructs an empty vector */
        SimpleVector() : SimpleMatrix<T>(0, 0) { }

        /*! \brief Construct a vector of a given size */
        SimpleVector(size_t nelements)
            : SimpleMatrix<T>(1, nelements)
        { }

        /*! \brief Construct a vector by copying data from a raw pointer */
        SimpleVector(size_t nelements, T * data)
            : SimpleMatrix<T>(1, nelements, data)
        { }

        /*! \brief Construct a vector by copying data from an std::vector */
        SimpleVector(size_t nelements, const std::vector<T> & v)
            : SimpleMatrix<T>(1, nelements, v)
        { }

        /*! \brief Construct a vector by moving a unique_ptr */
        SimpleVector(size_t nelements, std::unique_ptr<T []> && data)
            : SimpleMatrix<T>(1, nelements, std::move(data))
        { }

        /*! \brief Deep copy constructor */
        SimpleVector(const SimpleVector & rhs)
            : SimpleMatrix<T>(static_cast<SimpleMatrix<T>>(rhs))
        { }

        SimpleVector(SimpleVector && rhs)
            : SimpleMatrix<T>(static_cast<SimpleMatrix<T> &&>(rhs))
        { }

        SimpleVector & operator=(SimpleVector &&) = default;
        SimpleVector & operator=(const SimpleVector & rhs) = default; // use base class


        /*! \brief Obtain a reference to an element
         * 
         * \throw bpmodule::exception::MathException if the row
         *        or column is out of range (only if assertions are enabled)
         */
        T & operator()(size_t i) ASSERTIONS_ONLY
        {
            return static_cast<SimpleMatrix<T>>(*this)(1, i);
        }

        /*! \brief Obtain a const reference to an element
         * 
         * \throw bpmodule::exception::MathException if the row
         *        or column is out of range (only if assertions are enabled)
         */
        const T & operator()(size_t i) const ASSERTIONS_ONLY
        {
            return static_cast<SimpleMatrix<T>>(*this)(1, i);
        }

        /*! \brief Obtain a reference to an element
         * 
         * \throw bpmodule::exception::MathException if the row
         *        or column is out of range
         */
        T & At(size_t i)
        {
            return SimpleMatrix<T>::At(1, i);
        }

        /*! \brief Obtain a const reference to an element
         * 
         * \throw bpmodule::exception::MathException if the row
         *        or column is out of range
         */
        const T & At(size_t i) const
        {
            return SimpleMatrix<T>::At(1, i);
        }


        /// Take ownership of raw data
        void Take(size_t nelements, std::unique_ptr<T[]> && data)
        {
            SimpleMatrix<T>::Take(1, nelements, std::move(data));
        }
 

        ///////////////////////////////////////////////
        // serialization is handled in the base class
        ///////////////////////////////////////////////
};




// Explicit instantiations
extern template class SimpleMatrix<float>;
extern template class SimpleMatrix<double>;
extern template class SimpleMatrix<std::complex<float>>;
extern template class SimpleMatrix<std::complex<double>>;
extern template class SimpleVector<float>;
extern template class SimpleVector<double>;
extern template class SimpleVector<std::complex<float>>;
extern template class SimpleVector<std::complex<double>>;


typedef SimpleMatrix<float> SimpleMatrixF;
typedef SimpleMatrix<double> SimpleMatrixD;
typedef SimpleMatrix<std::complex<float>> SimpleMatrixCF;
typedef SimpleMatrix<std::complex<double>> SimpleMatrixCD;
typedef SimpleVector<float> SimpleVectorF;
typedef SimpleVector<double> SimpleVectorD;
typedef SimpleVector<std::complex<float>> SimpleVectorCF;
typedef SimpleVector<std::complex<double>> SimpleVectorCD;


} // close namespace math
} // close namespace bpmodule

#endif
