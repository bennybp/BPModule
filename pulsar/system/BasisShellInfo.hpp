/*! \file
 *
 * \brief Standalone class for basis shell information (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_SYSTEM__BASISSHELLINFO_HPP_
#define PULSAR_GUARD_SYSTEM__BASISSHELLINFO_HPP_

#include "pulsar/system/BasisShellBase.hpp"


namespace pulsar{

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
        ///Preallocates the memory for the parameters
        BasisShellInfo(ShellType type, int am, size_t nprim, size_t ngen)
            : BasisShellInfo(type, am, nprim, ngen,
                             std::move(std::vector<double>(nprim)),
                             std::move(std::vector<double>(nprim*ngen)))
        { }

        ///Copies the parameters into the class
        BasisShellInfo(ShellType type, int am, size_t nprim, size_t ngen,
                       const std::vector<double> & alphas, const std::vector<double> & allcoefs)
            : BasisShellInfo(type, am, nprim, ngen,
                             std::move(std::vector<double>(alphas)),
                             std::move(std::vector<double>(allcoefs)))
        { }

        ///Moves the parameters into the class
        BasisShellInfo(ShellType type, int am, size_t nprim, size_t ngen,
                       std::vector<double> && alphas, std::vector<double> && allcoefs)
            : BasisShellBase(type, am, nprim, ngen),
              alphas_(std::move(alphas)),
              coefs_(std::move(allcoefs))
        {
            

            if(alphas_.size() != n_primitives())
                throw PulsarException("Incompatible dimensions for alphas", "nprim", n_primitives(),
                                        "given", alphas_.size());

            if(coefs_.size() != n_coefficients())
                throw PulsarException("Incompatible dimensions for coefficients",
                                         "nprim", n_primitives(), "ngen", n_general_contractions(), "given", coefs_.size());

            BasisShellBase::set_ptrs_(alphas_.data(), coefs_.data());
        }


        /*! \brief For serialization only
         * 
         * \warning NOT FOR USE OUTSIDE OF SERIALIZATION
         * \todo Replace if cereal fixes this
         */
        BasisShellInfo() = default;

        ///Deep copies data through BasisShellBase API
        BasisShellInfo(const BasisShellBase & rhs)
            : BasisShellInfo(rhs.get_type(), rhs.am(),
                            rhs.n_primitives(), rhs.n_general_contractions(),
                            rhs.get_alphas(), rhs.get_all_coefs())
        { }

        ///Deep copies data
        BasisShellInfo(const BasisShellInfo & rhs)
            : BasisShellInfo(static_cast<const BasisShellBase &>(rhs))
        { }

        ///Deep copies data
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

        ///Compares exponents and coefs for equality
        bool operator==(const BasisShellInfo & rhs) const
        {
            // prevent implicit comparison between one type and another
            return BasisShellBase::base_compare_(rhs);
        }

        ///Negates operator== returns result
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
            BasisShellBase::set_ptrs_(alphas_.data(), coefs_.data());
        }

        // No need to hash anything here. Will be hashed by the base class 

        ///@}
};

} // close namespace pulsar


#endif
