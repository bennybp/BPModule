/*! \file
 *
 * \brief Standalone class for basis shell information (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_SYSTEM__BASISSHELLINFO_HPP_
#define PULSAR_GUARD_SYSTEM__BASISSHELLINFO_HPP_

#include "pulsar/system/BasisShellBase.hpp"


namespace pulsar{
namespace system {


/*! \brief Standalone shell information
 *
 * This class is self contained (ie, does not share data with other objects).
 * This is meant to be stored in System or to be otherwise used by developers
 * without fear of aliasing with other objects.
 *
 * As such, is would generally be "slow" (for some value of slow).
 * In addition, information about the coordinates are not stored here.
 *
 * Data is accessed through the BasisShellBase base class.
 */
class BasisShellInfo : public BasisShellBase
{
    public:
        BasisShellInfo(ShellType type, int am, size_t nprim, size_t ngen)
            : BasisShellInfo(type, am, nprim, ngen,
                             std::move(std::vector<double>(nprim)),
                             std::move(std::vector<double>(nprim*ngen)))
        { }

        BasisShellInfo(ShellType type, int am, size_t nprim, size_t ngen,
                       const std::vector<double> & alphas, const std::vector<double> & allcoefs)
            : BasisShellInfo(type, am, nprim, ngen,
                             std::move(std::vector<double>(alphas)),
                             std::move(std::vector<double>(allcoefs)))
        { }


        BasisShellInfo(ShellType type, int am, size_t nprim, size_t ngen,
                       std::vector<double> && alphas, std::vector<double> && allcoefs)
            : BasisShellBase(type, am, nprim, ngen),
              alphas_(std::move(alphas)),
              coefs_(std::move(allcoefs))
        {
            using namespace exception;

            if(alphas_.size() != NPrim())
                throw BasisSetException("Incompatible dimensions for alphas", "nprim", NPrim(),
                                        "given", alphas_.size());

            if(coefs_.size() != NCoef())
                throw BasisSetException("Incompatible dimensions for coefficients",
                                         "nprim", NPrim(), "ngen", NGeneral(), "given", coefs_.size());

            BasisShellBase::SetPtrs_(alphas_.data(), coefs_.data());
        }


        /*! \brief For serialization only
         * 
         * \warning NOT FOR USE OUTSIDE OF SERIALIZATION
         * \todo Replace if cereal fixes this
         */
        BasisShellInfo() = default;


        BasisShellInfo(const BasisShellBase & rhs)
            : BasisShellInfo(rhs.GetType(), rhs.AM(),
                            rhs.NPrim(), rhs.NGeneral(),
                            rhs.GetAlphas(), rhs.GetAllCoefs())
        { }

        BasisShellInfo(const BasisShellInfo & rhs)
            : BasisShellInfo(static_cast<const BasisShellBase &>(rhs))
        { }


        BasisShellInfo & operator=(const BasisShellInfo & rhs)
        {
            using std::swap;

            if(this == &rhs)
                return *this;

            BasisShellInfo tmp(rhs);
            swap(*this, tmp);
            return *this;
        }

        // These are ok though
        BasisShellInfo(BasisShellInfo &&)                  = default;
        BasisShellInfo & operator=(BasisShellInfo &&)      = default;

        bool operator==(const BasisShellInfo & rhs) const
        {
            // prevent implicit comparison between one type and another
            return BasisShellBase::BaseCompare_(rhs);
        }

        bool operator!=(const BasisShellInfo & rhs) const
        {
            return !((*this) == rhs);
        }


    private:
        std::vector<double> alphas_; //!< Exponents (storage)
        std::vector<double> coefs_;  //!< Coefficients (storage)



        //! \name Serialization and Hashing
        ///@{

        DECLARE_SERIALIZATION_FRIENDS


        template<class Archive>
        void serialize(Archive & ar)
        {
            // we aren't serializing the base class, so we do this manually
            ar(cereal::base_class<BasisShellBase>(this), alphas_, coefs_);

            // if we are unserializing
            BasisShellBase::SetPtrs_(alphas_.data(), coefs_.data());
        }

        // No need to hash anything here. Will be hashed by the base class 

        ///@}
};


/// A vector of shell information
typedef std::vector<BasisShellInfo> BasisShellInfoVector;


} // close namespace system
} // close namespace pulsar


#endif
