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
#include "bphash/Hasher.hpp"

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
         * Will NOT copy data from \p bshell to the new storage space
         * 
         * \param [in] bshell Another shell to copy
         * \param [in] alphaptr Pointer to where this shell's exponents are
         * \param [in] coefptr Pointer to where this shell's coefficients are
         * \param [in] xyzptr Pointer to where this shell's coordinates are
         */
        BasisSetShell(const BasisShellBase & bshell,
                      double * alphaptr, double * coefptr, double * xyzptr) ASSERTIONS_ONLY
            : BasisShellBase(bshell.GetType(), bshell.AM(), bshell.NPrim(), bshell.NGeneral())
        {
            SetPtrs_(alphaptr, coefptr, xyzptr);
        }


        // compiler generated ok for move only
        BasisSetShell(const BasisSetShell & rhs)             = delete;
        BasisSetShell(BasisSetShell && rhs)                  = default;
        BasisSetShell & operator=(const BasisSetShell & rhs) = delete;
        BasisSetShell & operator=(BasisSetShell && rhs)      = default;

        bool operator==(const BasisSetShell & rhs) const
        {
            if(this == &rhs)
                return true;

            // this is done manually (rather than "using")
            // prevent implicit comparison between one type and another
            return (
                      std::equal(xyz_, xyz_+3, rhs.xyz_) &&
                      BasisShellBase::BaseCompare_(rhs)
                   );
        }


        bool operator!=(const BasisSetShell & rhs) const
        {
            return !((*this) == rhs);
        }

        ///@{ Safe, slow access to data

        /// Get the coordinates of the shell
        CoordType GetCoords(void) const ASSERTIONS_ONLY
        {
            AssertXYZPtr_();
            return CoordType{xyz_[0], xyz_[1], xyz_[2]};
        }

        /// Set the coordinates of the shell
        void SetCoords(const CoordType coords) const
        {
            xyz_[0] = coords[0];
            xyz_[1] = coords[1];
            xyz_[2] = coords[2];
        }

        /// Get a single coordinate of the shell
        double GetCoord(unsigned int i) const
        {
            AssertXYZPtr_();
            ValidateXYZIdx_(i);
            return xyz_[i];
        }

        /// Set a single coordinate of the shell
        void SetCoord(unsigned int i, double val)
        {
            AssertXYZPtr_();
            ValidateXYZIdx_(i);
            xyz_[i] = val;
        }

        ///@}




        ///@{ Raw, unsafe, fast

        /// Get a pointer to the coordinates of the shell
        const double * CoordsPtr(void) const ASSERTIONS_ONLY
        {
            AssertXYZPtr_();
            return xyz_;
        }

        /// Get a pointer to the coordinates of the shell
        double * CoordsPtr(void) ASSERTIONS_ONLY
        {
            AssertXYZPtr_();
            return xyz_;
        }

        double Coord(unsigned int i) const ASSERTIONS_ONLY
        {
            AssertXYZPtr_();
            AssertXYZIdx_(i);
            return xyz_[i];
        }

        ///@}




        /*! \brief For serialization only
         * 
         * \warning NOT FOR USE OUTSIDE OF SERIALIZATION
         * \todo Replace if cereal fixes this
         */
        BasisSetShell() = default;

        bphash::HashValue MyHash(void) const
        {
            return bphash::MakeHash(bphash::HashType::Hash128, *this);
        } 


    private:
        friend class BasisSet;

        double * xyz_;     //!< XYZ coordindates of this center


        void AssertXYZPtr_(void) const
        {
            using namespace exception;
            Assert<BasisSetException>(xyz_ != nullptr, "Null pointers in BasisSetShell");
        }

        void ValidateXYZIdx_(unsigned int i) const
        {
            using namespace exception;
            if(i > 2)
                throw BasisSetException("Attempt to access coordinate direction that does not exist",
                                        "i", i);
        }

        void AssertXYZIdx_(unsigned int i) const
        {
            using namespace exception;
            Assert<BasisSetException>(i < 3, "Attempt to access coordinate direction that does not exist",
                                             "i", i);
        }


        void SetPtrs_(double * alphaptr, double * coefptr, double * xyzptr)
        {
            BasisShellBase::SetPtrs_(alphaptr, coefptr); 
            xyz_ = xyzptr;
        }


        //! \name Serialization
        ///@{

        DECLARE_SERIALIZATION_FRIENDS
        friend class bphash::Hasher;

        template<class Archive>
        void serialize(Archive & ar)
        {
            ar(cereal::base_class<BasisShellBase>(this));
        }

       
        void hash(bphash::Hasher & h) const
        {
            h(static_cast<const BasisShellBase &>(*this),
              bphash::HashPointer(xyz_, 3));
        }
 
        ///@}
    
};


} // close namespace system
} // close namespace pulsar


#endif
