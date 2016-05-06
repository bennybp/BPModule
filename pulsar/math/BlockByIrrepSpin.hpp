/*! \file
 *
 * \brief A map of spin and spatial symmetry to data types
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_MATH__BLOCKBYIRREPSPIN_HPP_
#define PULSAR_GUARD_MATH__BLOCKBYIRREPSPIN_HPP_

#include <map>
#include "pulsar/util/Serialization.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/math/Irrep.hpp"

#include "pulsar/util/bphash/types/map.hpp"


namespace pulsar{
namespace math{

/* \brief Maps spin and spatial symmetry to data
 *
 * \par Hashing
 *     The hash value is unique with respect to the map between
 *     the irrep/spatial symmetry and the hashing of data type \p T.
 *     See that class for details.
 */
template<typename T>
class BlockByIrrepSpin
{
    public:
        typedef std::pair<Irrep, int> IrrepSpin;
        typedef typename std::map<IrrepSpin, T>::iterator iterator;
        typedef typename std::map<IrrepSpin, T>::const_iterator const_iterator;


        //! \todo cannot be default due to some compiler issues
        BlockByIrrepSpin() {};
        BlockByIrrepSpin(const BlockByIrrepSpin & rhs) : data_(rhs.data_) { }
        BlockByIrrepSpin(BlockByIrrepSpin && rhs) : data_(std::move(rhs.data_)) { }

        BlockByIrrepSpin & operator=(const BlockByIrrepSpin &) = default;
        BlockByIrrepSpin & operator=(BlockByIrrepSpin &&) = default;

        bool operator==(const BlockByIrrepSpin & rhs) const
        {
            return data_ == rhs.data_;
        }

        bool operator!=(const BlockByIrrepSpin & rhs) const
        {
            return !((*this) == rhs);
        }


        bool Has(Irrep irrep, int spin) const
        {
            return data_.count({irrep, spin});
        }


        T & Get(Irrep irrep, int spin)
        {
            //! \todo need irrep to string
            //if(!HasMatrix(irrep, spin))
            //    throw exception::MathException("No matrix with this irrep/spin",
            //                                   "irrep", irrep, "spin", spin);    

            return data_.at({irrep, spin});
        }

        const T & Get(Irrep irrep, int spin) const
        {
            //! \todo need irrep to string
            //if(!HasMatrix(irrep, spin))
            //    throw exception::MathException("No matrix with this irrep/spin",
            //                                   "irrep", irrep, "spin", spin);    

            return data_.at({irrep, spin});
        }

        void Set(Irrep irrep, int spin, const T & val)
        {
            if(!Has(irrep, spin))
                data_.emplace(IrrepSpin{irrep, spin}, val);
            else
                data_[{irrep, spin}] = val;
        }

        void Take(Irrep irrep, int spin, T && val)
        {
            if(!Has(irrep, spin))
                data_.emplace(IrrepSpin{irrep, spin}, std::move(val));
            else
                data_[{irrep, spin}] = std::move(val);
        }

        void Erase(Irrep irrep, int spin)
        {
            data_.erase({irrep, spin});
        }

        void Clear(void)
        {
            data_.clear();
        }


        iterator begin(void) { return data_.begin(); }

        const_iterator begin(void) const { return data_.begin(); }

        iterator end(void) { return data_.end(); }

        const_iterator end(void) const { return data_.end(); }


        /*! \brief Obtain a hash of the data
         *
         * Details depend on what kind of data is stored.
         * See the hashing functions of the stored type
         * for details.
         */
        util::Hash MyHash(void) const
        {
            return util::MakeHash(*this);
        }

    private:
        std::map<IrrepSpin, T> data_;

        //! \name Serialization
        ///@{

        DECLARE_SERIALIZATION_FRIENDS
        DECLARE_HASHING_FRIENDS

        template<class Archive>
        void serialize(Archive & ar)
        {
            ar(data_);
        }

        void hash(util::Hasher & h) const
        {
            h(data_);
        }

        ///@}
};



} // close namespace math
} // close namespace pulsar

#endif
