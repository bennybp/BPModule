/*! \file
 *
 * \brief Base class for basis set shells (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_SYSTEM__BASISSHELLBASE_HPP_
#define PULSAR_GUARD_SYSTEM__BASISSHELLBASE_HPP_

#include <vector>

#include "pulsar/system/ShellType.hpp"
#include "pulsar/system/NFunction.hpp"
#include "pulsar/util/Serialization.hpp"
#include "pulsar/util/bphash/Hasher.hpp"
#include "pulsar/exception/Assert.hpp"
#include "pulsar/exception/Exceptions.hpp"


namespace pulsar{
namespace system {


/*! \brief Base class for basis set shells
 *
 * This is used to implement the base interface for shells, particularly
 * all of the functionality for getting / setting information  about the shell.
 *
 * This class provides two different accesses. The first is a safe, although
 * slower, access via GetAlpha(), etc. These functions check bounds access
 * and return copies of the underlying data.
 *
 * For fast, unsafe access, raw pointers and references to the underlying
 * data can also be obtained.
 *
 * \todo We need developers notes about "special" combined shells
 */
class BasisShellBase
{
    /*
     * Since this class is never expected to be
     * instantiated by itself, serialization is done
     * completely through the derived classes. This
     * is due to the derived classes owning the 
     * that we posize_t to.
     */
    public:
        virtual ~BasisShellBase() = default;


        /*! \name General properties */
        ///@{ 

        /// Get the type of shell
        ShellType GetType(void) const noexcept { return type_; }

        /// Get this shells angular momentum
        int AM(void) const noexcept { return am_; }

        /*! Get the AM of a specific general contraction
         *
         * \throw pulsar::exception::BasisSetException on out-of-bounds access
         */
        int GeneralAM(size_t n) const
        {
            ValidateGenIdx_(n);

            // if IsCombinedAM(), first is s, second is p, ...
            return (IsCombinedAM()) ? n : am_;
        }

        /// Get the number of primitives in this shell
        size_t NPrim(void) const noexcept { return nprim_; }

        /*! \brief Get the number of coefficients in this shell
         * 
         * This takes into account general contractions and combined shells,
         * so it is not necessarily equal to the number of primitives
         */
        size_t NCoef(void) const noexcept { return nprim_ * ngen_; }

        /// Get the level of general contraction of this shell
        size_t NGeneral(void) const noexcept { return ngen_; }


        /*! \brief Get the number of functions represented by this shell
         *
         * Depends on the type of shell (ie SphericalGaussian or CartesianGaussian).
         *
         * This takes into account general contractions and combined shells, so it
         * is not simply based on the AM alone
         */
        size_t NFunctions(void) const noexcept { return nfunc_; }


        /*! Get the number of functions for a given general contraction
         *
         * \throw pulsar::exception::BasisSetException on out-of-bounds access
         */
        size_t GeneralNFunctions(size_t n) const { return system::NFunction(type_, GeneralAM(n)); }


        /*! \brief Is this shell a combined AM shell
         * 
         * A combined AM shell is a generally-contracted shell where
         * the general contractions represent different angular momentum.
         * For example, sp and spd shells.
         */
        bool IsCombinedAM(void) const noexcept { return am_ < 0; }

        ///@}



        /*! \name Raw, fast, unsafe access */
        ///@{ 

        /*! \brief Get a single value for alpha (exponent)
         *
         * \param [in] i Index of the primitive to get
         */
        double Alpha(size_t i) const ASSERTIONS_ONLY
        {
            AssertPtrs_();
            AssertPrimIdx_(i);
            return alphas_[i]; 
        }


        /*! \brief Get a single value of a coefficient
         *
         * \param [in] n Index of the general contraction
         * \param [in] i Index in the segmented contraction
         */
        double Coef(size_t n, size_t i) const ASSERTIONS_ONLY
        {
            AssertPtrs_();
            AssertPrimIdx_(i);
            AssertGenIdx_(n);
            return coefs_[n*nprim_+i]; 
        }


        /*! \brief Get the pointer to the exponents
         */
        double const * AlphaPtr(void) const ASSERTIONS_ONLY
        {
            AssertPtrs_();
            return alphas_;
        }

        /// \copydocs AlphaPtr(void) const
        double * AlphaPtr(void) ASSERTIONS_ONLY
        {
            AssertPtrs_();
            return alphas_;
        }


        /*! \brief Get the pointer to the coefficients of a particular general contraction
         * 
         * The coefficients for a particular general contraction are
         * assumed to be contiguous
         *
         * \param [in] n Index of the general contraction
         */
        double const * CoefPtr(size_t n) const ASSERTIONS_ONLY
        {
            AssertPtrs_();
            AssertGenIdx_(n);
            return coefs_ + n*nprim_;
        }

        /// \copydocs CoefPtr(size_t n) const
        double * CoefPtr(size_t n) ASSERTIONS_ONLY
        {
            AssertPtrs_();
            AssertGenIdx_(n);
            return coefs_ + n*nprim_;
        }



        /*! \brief Get the pointer to all coefficients
         * 
         * The coefficients are assumed to be contiguous, with the
         * general contraction index being the slowest index.
         */
        double const * AllCoefsPtr(void) const ASSERTIONS_ONLY
        {
            AssertPtrs_();
            return coefs_;
        }

        /// \copydocs AllCoefsPtr(void) const
        double * AllCoefsPtr(void) ASSERTIONS_ONLY
        {
            AssertPtrs_();
            return coefs_;
        }

        ///@}




        /*! \name Slow, safe element access */
        ///@{


        /*! \brief Get a single value for alpha (exponent)
         *
         * \throw pulsar::exception::BasisSetException on out-of-bounds access
         *
         * \param [in] i Index of the primitive to get
         */
        double GetAlpha(size_t i) const
        {
            AssertPtrs_();
            ValidatePrimIdx_(i);
            return Alpha(i);
        }


        
        /*! \brief Set a single value for alpha (exponent)
         *
         * \throw pulsar::exception::BasisSetException on out-of-bounds access
         *
         * \param [in] i Index of the primitive to set
         * \param [in] alpha The value to set the exponent to
         */
        void SetAlpha(size_t i, double alpha)
        {
            AssertPtrs_();
            ValidatePrimIdx_(i);
            alphas_[i] = alpha;
        }



        /*! \brief Get a single value of a coefficient
         *
         * \throw pulsar::exception::BasisSetException on out-of-bounds access
         *
         * \param [in] n Index of the general contraction
         * \param [in] i Index in the segmented contraction
         */
        double GetCoef(size_t n, size_t i) const
        {
            AssertPtrs_();
            ValidatePrimIdx_(i);
            ValidateGenIdx_(n);
            return Coef(n, i);
        }



        /*! \brief Set a single value of a coefficient
         *
         * \throw pulsar::exception::BasisSetException on out-of-bounds access
         *
         * \param [in] n Index of the general contraction
         * \param [in] i Index in the segmented contraction
         * \param [in] coef The value to set the coefficient to
         */
        void SetCoef(size_t n, size_t i, double coef)
        {
            AssertPtrs_();
            ValidatePrimIdx_(i);
            ValidateGenIdx_(n);
            coefs_[n*nprim_+i] = coef;
        }


        /// Get all exponents as a vector
        std::vector<double> GetAlphas(void) const
        {
            AssertPtrs_();
            return std::vector<double>(alphas_, alphas_+nprim_);
        }


        /*! \brief Set all exponents via a vector
         *
         * \throw pulsar::exception::BasisSetException if the vector is not of the right length
         */ 
        void SetAlphas(const std::vector<double> & alphas)
        {
            using namespace exception;
            AssertPtrs_();

            if(alphas.size() != nprim_)
                throw BasisSetException("Incompatible dimensions for alphas", "nprim", nprim_, "given", alphas.size());

            std::copy(alphas.begin(), alphas.end(), alphas_);
        }


        /*! \brief Get all coefficients of a general contraction as a vector
         * 
         * \param [in] n Index of the general contraction
         */
        std::vector<double> GetCoefs(size_t n) const
        {
            AssertPtrs_();
            ValidateGenIdx_(n);

            return std::vector<double>(coefs_+n*nprim_,
                                       coefs_+(n+1)*nprim_);
        }


        /*! \brief Set all coefficients for a general contraction via a vector
         * 
         * \throw pulsar::exception::BasisSetException if the vector is not of the right length
         */
        void SetCoefs(size_t n, const std::vector<double> & coefs)
        {
            using namespace exception;
            AssertPtrs_();
            ValidateGenIdx_(n);

            if(coefs.size() != nprim_)
                throw BasisSetException("Incompatible dimensions for coefficients",
                                        "nprim", nprim_, "given", coefs.size());

            std::copy(coefs.begin(), coefs.end(), coefs_ + n*nprim_);
        }

        /// Get all coefficints for all general contractions as a vector
        std::vector<double> GetAllCoefs(void) const
        {
            AssertPtrs_();
            return std::vector<double>(coefs_, coefs_ + NCoef());
        }


        /*! \brief Set all coefficients for all general contractions via a vector
         * 
         * \throw pulsar::exception::BasisSetException if the vector is not of the right length
         */
        void SetAllCoefs(const std::vector<double> & coefs)
        {
            using namespace exception;
            AssertPtrs_();
            if(coefs.size() != NCoef())
                throw BasisSetException("Incompatible dimensions for coefficients",
                                        "nprim", nprim_, "ngen", ngen_, "given", coefs.size());

            std::copy(coefs.begin(), coefs.end(), coefs_);
        }


        /*! \brief Set a primitive for a shell with no general contractions
         * 
         * \throw pulsar::exception::BasisSetException if the shell has more than
         *        one general contraction or if there is out-of-bounds access
         * 
         * \param [in] i Index of the primitive
         * \param [in] alpha The exponent of the primitive
         * \param [in] coef The coefficient of the single general contraction
         */ 
        void SetPrimitive(size_t i, double alpha, double coef)
        {
            SetPrimitive(i, alpha, std::vector<double>{coef});
        }


        /*! \brief Set all information for a primitive
         * 
         * \throw pulsar::exception::BasisSetException if \p coefs is not the right length
         *        or if there is out-of-bounds access
         *
         * \param [in] i Index of the primitive
         * \param [in] alpha The exponent of the primitive
         * \param [in] coefs Coefficients for the general contractions of this primitive
         */
        void SetPrimitive(size_t i, double alpha, const std::vector<double> & coefs)
        {
            using namespace exception;
            AssertPtrs_();
            ValidatePrimIdx_(i);

            const size_t ncoefs = coefs.size();

            if(ncoefs != ngen_)
                throw BasisSetException("Incompatible dimensions for coefficients",
                                        "ngeneral", ngen_, "given", ncoefs);

            alphas_[i] = alpha;

            for(size_t n = 0; n < ngen_; n++)
                SetCoef(n, i, coefs[n]);
        }
        ///@}


        util::Hash MyHash(void) const
        {
            return util::MakeHash(*this);
        } 


    protected:

        /*! \brief Constructor
         * 
         * \param [in] type Type of the shell
         * \param [in] am Angular momentum of the shell
         * \param [in] nprim Number of primitives
         * \param [in] ngen Number of general contractions
         */
        BasisShellBase(ShellType type, int am, size_t nprim, size_t ngen)
            : type_(type), am_(am), nprim_(nprim), ngen_(ngen), nfunc_(0),
              alphas_(nullptr), coefs_(nullptr)
        {
            using namespace exception;

            // If this is sp, spd, spdf, etc, then ngen
            // must be exactly 2, 3, 4 respectively
            if(am < 0 && ((-am)+1) != ngen)
                throw BasisSetException("Invalid number of general contractions for special AM",
                                                   "am", am, "expected_ngen", ((-am)+1));

            // calculate the number of functions
            for(size_t i = 0; i < ngen_; i++)
                nfunc_ += GeneralNFunctions(i);   
        }


        // compiler generated ok for move only
        BasisShellBase(const BasisShellBase &)             = delete;
        BasisShellBase(BasisShellBase &&)                  = default;
        BasisShellBase & operator=(const BasisShellBase &) = delete;
        BasisShellBase & operator=(BasisShellBase &&)      = default;


        /// Set the pointers for the exponents and coefficients
        void SetPtrs_(double * alphaptr, double * coefptr) ASSERTIONS_ONLY
        {
            alphas_ = alphaptr;
            coefs_ = coefptr;
            AssertPtrs_();
        }

        // For serialization only (needed by derived classes)
        BasisShellBase() = default;

        /*! \brief Compare with another BasisShellBase
         * 
         * Used instead of to prevent comparisons with derived classes when
         * called from the derived class operator==
         *
         * This compares all properties and values, looking for an exact match.
         */
        bool BaseCompare_(const BasisShellBase & rhs) const
        {
            if(this == &rhs)
                return true;

            PRAGMA_WARNING_PUSH
            PRAGMA_WARNING_IGNORE_FP_EQUALITY

            // ordered by what is most likely to fail first (takes advantage
            // of short circuiting). When we get to alphas, we check to see if the pointers
            // are the same first. If they are different, we compare elementwise. This should
            // be safe, since nprim_ and ngen_ are already shown to be equal
            return (
                       am_ == rhs.am_ &&
                       ngen_ == rhs.ngen_ &&
                       nprim_ == rhs.nprim_ &&
                       type_ == rhs.type_ &&
                       ( (alphas_ == rhs.alphas_) || std::equal(alphas_, alphas_+nprim_, rhs.alphas_) ) &&
                       ( (coefs_ == rhs.coefs_) || std::equal(coefs_, coefs_ + NCoef(), rhs.coefs_) )
                   );

            PRAGMA_WARNING_POP
        }

    private:
        ShellType type_;   //!< Gaussian, Slater, etc
        int am_;           //!< Angular momentum
        size_t nprim_;     //!< Number of primitives
        size_t ngen_;      //!< Number of general contractions
        size_t nfunc_;     //!< Number of functions in total (stored for convenience)
        double * alphas_;  //!< Exponents
        double * coefs_;   //!< Coefficients


        // Assertions and sanity checks
        void ValidatePrimIdx_(size_t i) const
        {
            using namespace exception;
            if(i >= nprim_)
                throw BasisSetException("Attempt to access primitive that does not exist",
                                        "iprim", i, "nprim", nprim_);
        }

        void ValidateGenIdx_(size_t n) const
        {
            using namespace exception;
            if(n >= ngen_)
                throw BasisSetException("Attempt to access general contraction that does not exist",
                                        "igen", n, "ngen", ngen_);
        }

        void AssertPrimIdx_(size_t i) const
        {
            using namespace exception;
            Assert<BasisSetException>(i < nprim_, "Attempt to access primitive that does not exist",
                                                  "iprim", i, "nprim", nprim_);
        }

        void AssertGenIdx_(size_t n) const
        {
            using namespace exception;
            Assert<BasisSetException>(n < ngen_, "Attempt to access general contraction that does not exist",
                                                 "igen", n, "ngen", ngen_);
        }

        void AssertPtrs_(void) const
        {
            using namespace exception;
            Assert<BasisSetException>(alphas_ != nullptr && coefs_ != nullptr, "Null pointers in BasisShellBase");
        }



        //! \name Serialization
        ///@{

        DECLARE_SERIALIZATION_FRIENDS
        DECLARE_HASHING_FRIENDS

        template<class Archive>
        void serialize(Archive & ar)
        {
            // of course, we don't do the pointers. They are owned
            // by somebody else. It's their responsibilty
            ar(type_, am_, nprim_, ngen_, nfunc_);
        }

        void hash(util::Hasher & h) const
        {
            h(type_, am_, nprim_, ngen_, nfunc_,
                   util::HashPointer(alphas_, NPrim()),
                   util::HashPointer(coefs_, NCoef()));
        }


        ///@}



};




} // close namespace system
} // close namespace pulsar


#endif
