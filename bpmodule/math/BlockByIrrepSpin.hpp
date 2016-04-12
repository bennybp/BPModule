/*! \file
 *
 * \brief A map of spin and spatial symmetry to matrices
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef BPMODULE_GUARD_MATH__BLOCKBYIRREPSPIN_HPP_
#define BPMODULE_GUARD_MATH__BLOCKBYIRREPSPIN_HPP_

#include <map>
#include "bpmodule/util/Serialization.hpp"
#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/math/Irrep.hpp"

namespace bpmodule{
namespace math{


template<typename T>
class BlockByIrrepSpin
{
    public:
        typedef std::pair<Irrep, int> IrrepSpin;

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


    private:
        std::map<IrrepSpin, T> data_;

        //! \name Serialization
        ///@{

        DECLARE_SERIALIZATION_FRIENDS

        template<class Archive>
        void serialize(Archive & ar)
        {
            ar(data_);
        }

        ///@}
};



} // close namespace math
} // close namespace bpmodule

#endif
