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

#include "bphash/types/map.hpp"


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
        typedef typename std::map<int, T> SpinMap;
        typedef typename std::map<Irrep, SpinMap> IrrepSpinMap;

        typedef typename IrrepSpinMap::iterator iterator;
        typedef typename IrrepSpinMap::const_iterator const_iterator;


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


        bool has(Irrep irrep, int spin) const
        {
            if(data_.count(irrep))
                return data_.at(irrep).count(spin);
            else
                return 0;
        }

        std::set<Irrep> get_irreps(void) const
        {
            std::set<Irrep> ret;
            for(const auto & it : data_)
                ret.insert(it.first);
            return ret;
        }

        std::set<int> get_spins(Irrep irrep) const
        {
            std::set<int> ret;
            if(data_.count(irrep))
            {
                for(const auto & it : data_.at(irrep))
                    ret.insert(it.first);
            }
            return ret;
        }


        T & get(Irrep irrep, int spin)
        {
            //! \todo need irrep to string
            //if(!HasMatrix(irrep, spin))
            //    throw exception::MathException("No matrix with this irrep/spin",
            //                                   "irrep", irrep, "spin", spin);    

            return data_.at(irrep).at(spin);
        }

        const T & get(Irrep irrep, int spin) const
        {
            //! \todo need irrep to string
            //if(!HasMatrix(irrep, spin))
            //    throw exception::MathException("No matrix with this irrep/spin",
            //                                   "irrep", irrep, "spin", spin);    

            return data_.at(irrep).at(spin);
        }

        void set(Irrep irrep, int spin, const T & val)
        {
            if(data_.count(irrep) == 0)
            {
                // will create a new SpinMap
                data_[irrep].emplace(spin, val);
            }
            else
            {
                if(data_.at(irrep).count(spin))
                    data_[irrep].erase(spin);  // avoid operator=

                data_.at(irrep).emplace(spin, val);
            }
        }

        void set(Irrep irrep, int spin, T && val)
        {
            if(data_.count(irrep) == 0)
            {
                // will create a new SpinMap
                data_[irrep].emplace(spin, std::move(val));
            }
            else
            {
                if(data_.at(irrep).count(spin))
                    data_[irrep].erase(spin);  // avoid operator=

                data_.at(irrep).emplace(spin, std::move(val));
            }
        }


        iterator begin(void) { return data_.begin(); }

        const_iterator begin(void) const { return data_.begin(); }

        iterator end(void) { return data_.end(); }

        const_iterator end(void) const { return data_.end(); }


        //! Does this object have same irrep/spin structure as another
        template<typename U>
        bool same_structure(const BlockByIrrepSpin<U> & rhs) const
        {
            for(const auto & irrepit : *this)
            for(const auto & spinit : irrepit.second)
            {
                if(!rhs.has(irrepit.first, spinit.first))
                    return false;
            }

            for(const auto & irrepit : rhs)
            for(const auto & spinit : irrepit.second)
            {
                if(!has(irrepit.first, spinit.first))
                    return false;
            }

            return true;
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

    private:
        IrrepSpinMap data_;

        //! \name Serialization
        ///@{

        DECLARE_SERIALIZATION_FRIENDS
        friend class bphash::Hasher;

        template<class Archive>
        void serialize(Archive & ar)
        {
            ar(data_);
        }

        void hash(bphash::Hasher & h) const
        {
            h(data_);
        }

        ///@}
};



} // close namespace math
} // close namespace pulsar

#endif
