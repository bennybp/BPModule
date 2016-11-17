/*! \file
 *
 * \brief A map of spin and spatial symmetry to data types
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_MATH__BLOCKBYIRREPSPIN_HPP_
#define PULSAR_GUARD_MATH__BLOCKBYIRREPSPIN_HPP_

#include <map>
#include <iostream>
#include "pulsar/util/Serialization.hpp"
#include "pulsar/exception/PulsarException.hpp"
#include "pulsar/math/Irrep.hpp"

#include "bphash/types/map.hpp"


namespace pulsar{

/* \brief Maps spin and spatial symmetry to data
 *
 * \par Hashing
 *     The hash value is unique with respect to the map between
 *     the irrep/spatial symmetry and the hashing of data type \p T.
 *     See that class for details.
 * 
 * \param[in] T The type of the data
 */
template<typename T>
class BlockByIrrepSpin
{
    public:
        typedef typename std::map<int, T> SpinMap;///<Spin 2 data
        typedef typename std::map<Irrep, SpinMap> IrrepSpinMap;///<Irrep 2 spin

        typedef typename IrrepSpinMap::iterator iterator;///<Over SpinMaps
        ///Iterator over const SpinMaps
        typedef typename IrrepSpinMap::const_iterator const_iterator;


        //! \todo cannot be default due to some compiler issues
        BlockByIrrepSpin() {};
        ///Deep copies all data of \p rhs
        BlockByIrrepSpin(const BlockByIrrepSpin & rhs) : data_(rhs.data_) { }
        ///Moves other tensor
        BlockByIrrepSpin(BlockByIrrepSpin && rhs) : data_(std::move(rhs.data_)) { }
        ///Deep copies
        BlockByIrrepSpin & operator=(const BlockByIrrepSpin &) = default;
        ///Moves other tensor
        BlockByIrrepSpin & operator=(BlockByIrrepSpin &&) = default;
        ///Calls operator== on each data element in this and in \p rhs
        bool operator==(const BlockByIrrepSpin & rhs) const
        {
            return data_ == rhs.data_;
        }
        ///Checks if any element differs between this and \p rhs
        bool operator!=(const BlockByIrrepSpin & rhs) const
        {
            return !((*this) == rhs);
        }


        ///True if there is an object with irrep \p irrep and spin \p spin
        bool has(Irrep irrep, int spin) const
        {
            if(data_.count(irrep))
                return data_.at(irrep).count(spin);
            else
                return false;
        }

        ///Returns the set of irreps indexing the symmetry axis
        std::set<Irrep> get_irreps(void) const
        {
            std::set<Irrep> ret;
            for(const auto & it : data_)
                ret.insert(it.first);
            return ret;
        }
        
        ///Returns the spins associated with an irrep
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

        ///Returns the data with irrep \p irrep and spin \p spin
        T & get(Irrep irrep, int spin)
        {
            if(!has(irrep, spin))
               throw PulsarException("No matrix with this irrep/spin",
                                   "irrep", irrep_to_string.at(irrep), 
                                   "spin", spin);    

            return data_.at(irrep).at(spin);
        }

        ///\copydoc get
        const T & get(Irrep irrep, int spin) const
        {
            if(!has(irrep, spin))
                throw PulsarException("No matrix with this irrep/spin",
                                    "irrep", irrep_to_string.at(irrep),
                                    "spin", spin);    

            return data_.at(irrep).at(spin);
        }

        ///Sets the element with irrep \p irrep and spin \p spin to \p val
        void set(Irrep irrep, int spin, const T & val);
        
        ///\copydoc set
        void set(Irrep irrep, int spin, T && val);

        ///Returns an iterator over SpinMaps
        iterator begin(void) { return data_.begin(); }

        ///Returns const iterator over SpinMaps
        const_iterator begin(void) const { return data_.begin(); }

        ///Returns iterator just past the last SpinMap
        iterator end(void) { return data_.end(); }

        ///Returns const Iterator just past the las SpinMap
        const_iterator end(void) const { return data_.end(); }


        //! Does this object have same irrep/spin structure as another
        template<typename U>
        bool same_structure(const BlockByIrrepSpin<U> & rhs) const;


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
        
        ///Prints the tensor by passing underlying tensor type to ostream
        std::ostream& print(std::ostream& os)const;

    private:
        IrrepSpinMap data_;///<The actual data

        //! \name Serialization
        ///@{

        DECLARE_SERIALIZATION_FRIENDS
        BPHASH_DECLARE_HASHING_FRIENDS

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

/**************** Implementations *************************/
template<typename T>
void BlockByIrrepSpin<T>::set(Irrep irrep, int spin, const T & val)
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


template<typename T>
void BlockByIrrepSpin<T>::set(Irrep irrep, int spin, T && val)
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



template<typename T>
template<typename U>
bool BlockByIrrepSpin<T>::same_structure(const BlockByIrrepSpin<U> & rhs) const
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

template<typename T>
std::ostream& BlockByIrrepSpin<T>::print(std::ostream& os)const
{
    for(const auto& irr: data_)
    {
        for(const typename SpinMap::value_type& spin: irr.second)
            os<<irrep_to_string.at(irr.first)<<" "<<spin.first
              <<" "<<std::endl<<spin.second<<std::endl;
    }
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os,const pulsar::BlockByIrrepSpin<T>& tensor)
{
    return tensor.print(os);
}

} // close namespace pulsar


#endif
