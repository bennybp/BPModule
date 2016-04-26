/*! \file
 *
 * \brief Basis shell from a basis set object (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_SYSTEM__BASISSETSHELL_HPP_
#define PULSAR_GUARD_SYSTEM__BASISSETSHELL_HPP_

#include "pulsar/types.h"
#include "pulsar/system/BasisShellBase.hpp"
#include "pulsar/system/CoordType.hpp"
#include "pulsar/util/bphash/Hasher_fwd.hpp"

namespace pulsar{
namespace system {

// for friend
class BasisSet;


/*! \brief Shell information stored in a BasisSet object
 *
 * This class is not self contained. Instead, the data for
 * alpha, etc, are held elsewhere (in the BasisSet object). As such,
 * it is generally "faster", for various reasons. However,
 * Different objects can share data, so modifying data should probably
 * never be done.
 *
 * \todo Could make non-const stuff protected, and then forward...
 */
class BasisSetShell : public BasisShellBase
{
    /*
     * Since this class is meant to be paired with a basis set, it is not
     * meant to be serialized by itself
     */
    public:

        /*! \brief Constructor
         *
         * Will copy data from \p bshell to the new storage space
         * 
         * \param [in] bshell Another shell to copy
         * \param [in] alphaptr Pointer to where this shell's exponents are
         * \param [in] coefptr Pointer to where this shell's coefficients are
         * \param [in] xyzptr Pointer to where this shell's coordinates are
         */
        BasisSetShell(const BasisSetShell & bshell,
                      double * alphaptr, double * coefptr, double * xyzptr) ASSERTIONS_ONLY
            : BasisSetShell(bshell, alphaptr, coefptr, xyzptr,
                            bshell.GetID(), bshell.GetCenter())
        { }

        BasisSetShell(const BasisShellBase & bshell,
                      double * alphaptr, double * coefptr, double * xyzptr,
                      ID_t id, ID_t center) ASSERTIONS_ONLY
            : BasisShellBase(bshell.GetType(), bshell.AM(), bshell.NPrim(), bshell.NGeneral()),
              id_(id), center_(center)
        {
            SetPtrs_(alphaptr, coefptr, xyzptr);
        }


        // compiler generated ok for move only
        BasisSetShell(const BasisSetShell & rhs)             = delete;
        BasisSetShell(BasisSetShell && rhs)                  = default;
        BasisSetShell & operator=(const BasisSetShell & rhs) = delete;
        BasisSetShell & operator=(BasisSetShell && rhs)      = default;

        /// Get the ID of this shell in the basis set 
        ID_t GetID(void) const noexcept { return id_; }

        /// Get the ID of the center that this shell is on
        ID_t GetCenter(void) const noexcept { return center_; }

        /// Get the coordinates of the shell
        CoordType GetCoords(void) const { return CoordType{xyz_[0], xyz_[1], xyz_[2]}; }


        ///@{ Raw, unsafe, fast

        /// Get a pointer to the coordinates of the shell
        const double * CoordsPtr(void) const ASSERTIONS_ONLY
        {
            using namespace exception;
            Assert<BasisSetException>(xyz_ != nullptr, "Null XYZ pointer in BasisSetShell");
            return xyz_;
        }

        /// Get a pointer to the coordinates of the shell
        double * CoordsPtr(void) ASSERTIONS_ONLY
        {
            using namespace exception;
            Assert<BasisSetException>(xyz_ != nullptr, "Null XYZ pointer in BasisSetShell");
            return xyz_;
        }

        ///@}

        bool operator==(const BasisSetShell & rhs) const
        {
            if(this == &rhs)
                return true;

            // this is done manually (rather than "using")
            // prevent implicit comparison between one type and another
            return (
                      id_ == rhs.id_ &&
                      center_ == rhs.center_ &&
                      std::equal(xyz_, xyz_+3, rhs.xyz_) &&
                      BasisShellBase::BaseCompare_(rhs)
                   );
        }


        bool operator!=(const BasisSetShell & rhs) const
        {
            return !((*this) == rhs);
        }


        /*! \brief For serialization only
         * 
         * \warning NOT FOR USE OUTSIDE OF SERIALIZATION
         * \todo Replace if cereal fixes this
         */
        BasisSetShell() = default;

        util::Hash MyHash(void) const
        {
            return util::MakeHash(*this);
        } 


    private:
        friend class BasisSet;

        ID_t id_;          //!< Unique id for this shell
        ID_t center_;      //!< ID of the center
        double * xyz_;     //!< XYZ coordindates of this center

        void SetPtrs_(double * alphaptr, double * coefptr, double * xyzptr)
        {
            BasisShellBase::SetPtrs_(alphaptr, coefptr); 
            xyz_ = xyzptr;
        }


        //! \name Serialization
        ///@{

        DECLARE_SERIALIZATION_FRIENDS
        DECLARE_HASHING_FRIENDS

        template<class Archive>
        void serialize(Archive & ar)
        {
            ar(cereal::base_class<BasisShellBase>(this), id_, center_);
        }

       
        void hash(util::Hasher & h) const
        {
            h(static_cast<const BasisShellBase &>(*this),
              util::HashPointer(xyz_, 3),
              id_, center_); 
        }
 
        ///@}
    
};


} // close namespace system
} // close namespace pulsar


#endif
