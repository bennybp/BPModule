#pragma once

#include "pulsar/math/EigenHash.hpp"
#include "pulsar/math/BlockByIrrepSpin.hpp"
#include "pulsar/math/TensorImpl.hpp"

#include <Eigen/Dense>

namespace pulsar {

/** \brief Specialization of TensorImpl to Eigen's Matrix
 * 
 *  \todo Should save/load use the fact that you can grab the double* from an
 *        Eigen matrix?
 *  \todo A lot of duplicate code b/w this and EigenVectorImpl combine?
 */
class EigenMatrixImpl : public MatrixDImpl
{
    public:
        using matrix_type=Eigen::MatrixXd;
        using shared_matrix=std::shared_ptr<matrix_type>;
        /*! \brief For serialization only
         * 
         * \warning NOT FOR USE OUTSIDE OF SERIALIZATION
         * \todo Replace if cereal fixes this
         */
        EigenMatrixImpl() = default;

        ///Aliases this Eigen matrix to \p mat
        EigenMatrixImpl(const shared_matrix& mat)
            : mat_(mat) { }

        ///Copies Eigen matrix from \p mat
        EigenMatrixImpl(const matrix_type & mat)
            : mat_(std::make_shared<matrix_type>(mat)) { }

        ///Moves Eigen matrix from \p mat 
        EigenMatrixImpl(matrix_type && mat)
            : mat_(std::make_shared<matrix_type>(std::move(mat))) { }

        ///True if the underlying Eigen matrices are the same
        bool operator==(const EigenMatrixImpl& rhs)const
        {
            return *mat_==*rhs.mat_;
        }
        
        ///True if the underlying Eigen matrices are not the same
        bool operator!=(const EigenMatrixImpl& rhs)const
        {
            return !((*this)==rhs);
        }
        
        /*! \brief Obtain a hash of the data
         *
         * Details depend on what kind of data is stored.
         * See the hashing functions of the stored type
         * for details.
         */
        bphash::HashValue my_hash(void) const
        {
            return bphash::make_hash(bphash::HashType::Hash128, *this);
        }
        
        ///\copydoc TensorImpl::sizes
        virtual std::array<size_t, 2> sizes(void) const
        {
            return {static_cast<size_t>((*mat_).rows()),
                    static_cast<size_t>((*mat_).cols())};
        }

        ///\copydoc TensorImpl::get_value
        virtual double get_value(std::array<size_t, 2> idx) const
        {
            return (*mat_)(idx[0], idx[1]);
        }

        ///\copydoc TensorImpl::set_Value
        virtual void set_value(std::array<size_t, 2> idx, double val)
        {
            (*mat_)(idx[0], idx[1]) = val;
        }

        ///Allows you to get the actual matrix (in constant form)
        std::shared_ptr<const matrix_type> get_matrix(void) const
        {
            return mat_;
        }

    private:
        shared_matrix mat_;///<The actual matrix

        DECLARE_SERIALIZATION_FRIENDS
        BPHASH_DECLARE_HASHING_FRIENDS
        template<class Archive>
        void save(Archive & archive) const
        {
            archive(cereal::base_class<MatrixDImpl>(this));
            unsigned int nrow = (unsigned int) mat_->rows();
            unsigned int ncol = (unsigned int) mat_->cols();

            archive(nrow, ncol);

            //! \todo slow
            for(unsigned int i = 0; i < nrow; i++)
            for(unsigned int j = 0; j < ncol; j++)
                archive((*mat_)(i,j));
        }

        template<class Archive>
        void load(Archive & archive)
        {
            archive(cereal::base_class<MatrixDImpl>(this));

            unsigned int nrow, ncol;
            archive(nrow, ncol);

            mat_ = std::make_shared<Eigen::MatrixXd>(nrow, ncol);

            //! \todo slow
            for(unsigned int i = 0; i < nrow; i++)
            for(unsigned int j = 0; j < ncol; j++)
                archive((*mat_)(i,j));
        }
        
        void hash(bphash::Hasher & h) const
        {
            h(*mat_);
        }
};


///Same as EigenMatrix except for vectors
class EigenVectorImpl : public VectorDImpl
{
    public:
        using vector_type=Eigen::VectorXd;///<Type of Eigen's vector class
        using shared_vector=std::shared_ptr<vector_type>;///<Type of ptr to vec
        
        /*! \brief For serialization only
         * 
         * \warning NOT FOR USE OUTSIDE OF SERIALIZATION
         * \todo Replace if cereal fixes this
         */
        EigenVectorImpl() = default;

        ///Aliases this vector to the one in \p mat
        EigenVectorImpl(const shared_vector & mat)
            : mat_(mat) { }

        ///Deep copies the vector \p mat
        EigenVectorImpl(const vector_type & mat)
            : mat_(std::make_shared<vector_type>(mat)) { }

        ///Moves \p mat into this vector
        EigenVectorImpl(vector_type && mat)
            : mat_(std::make_shared<vector_type>(std::move(mat))) { }
        
                ///True if the underlying Eigen vectors are the same
        bool operator==(const EigenVectorImpl& rhs)const
        {
            return *mat_==*rhs.mat_;
        }
        
        ///True if the underlying Eigen vectors are not the same
        bool operator!=(const EigenVectorImpl& rhs)const
        {
            return !((*this)==rhs);
        }

        ///\copydoc TensorImpl::sizes()
        virtual std::array<size_t, 1> sizes(void) const
        {
            return {static_cast<size_t>((*mat_).size())};
        }

        ///\copydoc TensorImpl::get_value
        virtual double get_value(std::array<size_t, 1> idx) const
        {
            return (*mat_)(idx[0]);
        }

        ///\copydoc TensorImpl::set_value
        virtual void set_value(std::array<size_t, 1> idx, double val)
        {
            (*mat_)(idx[0]) = val;
        }

        ///Returns the vector wrapped in this class
        std::shared_ptr<const vector_type> get_matrix(void) const
        {
            return mat_;
        }
        
        ///Returns the hash of this object
        bphash::HashValue my_hash()const
        {
            return bphash::make_hash(bphash::HashType::Hash128, *this);
        }

    private:
        shared_vector mat_;///<The actual vector


        DECLARE_SERIALIZATION_FRIENDS
        BPHASH_DECLARE_HASHING_FRIENDS
        
        template<class Archive>
        void save(Archive & archive) const
        {
            archive(cereal::base_class<VectorDImpl>(this));
            unsigned int nrow = (unsigned int) mat_->rows();
            unsigned int ncol = (unsigned int) mat_->cols();

            archive(nrow, ncol);

            //! \todo slow
            for(unsigned int i = 0; i < nrow; i++)
            for(unsigned int j = 0; j < ncol; j++)
                archive((*mat_)(i,j));
        }

        template<class Archive>
        void load(Archive & archive)
        {
            archive(cereal::base_class<VectorDImpl>(this));

            unsigned int nrow, ncol;
            archive(nrow, ncol);

            mat_ = std::make_shared<Eigen::VectorXd>(nrow, ncol);

            //! \todo slow
            for(unsigned int i = 0; i < nrow; i++)
            for(unsigned int j = 0; j < ncol; j++)
                archive((*mat_)(i,j));
        }
        
        void hash(bphash::Hasher & h) const
        {
            h(*mat_);
        }
};

///Converts a pulsar matrix to an Eigen matrix
std::shared_ptr<const Eigen::MatrixXd>
convert_to_eigen(const MatrixDImpl & ten);

///Converts a pulsar vector to an Eigen vector
std::shared_ptr<const Eigen::VectorXd>
convert_to_eigen(const VectorDImpl & ten);

///Eigen Matrix suitable for use with symmetry and spin
typedef BlockByIrrepSpin<std::shared_ptr<EigenMatrixImpl>> BlockedEigenMatrix;

///Eigen vector suitable for use with symmetry and spin
typedef BlockByIrrepSpin<std::shared_ptr<EigenVectorImpl>> BlockedEigenVector;


} // close namespace pulsar

