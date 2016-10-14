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
#include "bphash/Hasher.hpp"
#include "pulsar/exception/Assert.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/pragma.h"


namespace pulsar{
namespace system {


/*! \brief Base class for basis set shells
 *
 * This is used to implement the base interface for shells, particularly
 * all of the functionality for getting / setting information  about the shell.
 *
 * This class provides two different accesses. The first is a safe, although
 * slower, access via get_alpha(), etc. These functions check bounds access
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
        ShellType get_type(void) const noexcept { return type_; }

        /// Get this shells angular momentum
        int am(void) const noexcept { return am_; }

        /*! Get the AM of a specific general contraction
         *
         * \throw pulsar::BasisSetException on out-of-bounds access
         */
        int general_am(size_t n) const
        {
            validate_gen_idx_(n);

            // if is_combined_am(), first is s, second is p, ...
            return (is_combined_am()) ? (int) n : (int) am_;
        }

        /// Get the number of primitives in this shell
        size_t n_primitives(void) const noexcept { return nprim_; }

        /*! \brief Get the number of coefficients in this shell
         * 
         * This takes into account general contractions and combined shells,
         * so it is not necessarily equal to the number of primitives
         */
        size_t n_coefficients(void) const noexcept { return nprim_ * ngen_; }

        /// Get the level of general contraction of this shell
        size_t n_general_contractions(void) const noexcept { return ngen_; }


        /*! \brief Get the number of functions represented by this shell
         *
         * Depends on the type of shell (ie SphericalGaussian or CartesianGaussian).
         *
         * This takes into account general contractions and combined shells, so it
         * is not simply based on the AM alone
         */
        size_t n_functions(void) const noexcept { return nfunc_; }


        /*! Get the number of functions for a given general contraction
         *
         * \throw pulsar::BasisSetException on out-of-bounds access
         */
        size_t general_n_functions(size_t n) const { return system::n_functions(type_, general_am(n)); }


        /*! \brief Is this shell a combined AM shell
         * 
         * A combined AM shell is a generally-contracted shell where
         * the general contractions represent different angular momentum.
         * For example, sp and spd shells.
         */
        bool is_combined_am(void) const noexcept { return am_ < 0; }

        ///@}



        /*! \name Raw, fast, unsafe access */
        ///@{ 

        /*! \brief Get a single value for alpha (exponent)
         *
         * \param [in] i Index of the primitive to get
         */
        double alpha(size_t i) const ASSERTIONS_ONLY
        {
            assert_ptrs_();
            assert_prim_idx_(i);
            return alphas_[i]; 
        }


        /*! \brief Get a single value of a coefficient
         *
         * \param [in] n Index of the general contraction
         * \param [in] i Index in the segmented contraction
         */
        double coef(size_t n, size_t i) const ASSERTIONS_ONLY
        {
            assert_ptrs_();
            assert_prim_idx_(i);
            assert_gen_idx_(n);
            return coefs_[n*nprim_+i]; 
        }


        /*! \brief Get the pointer to the exponents
         */
        double const * alpha_ptr(void) const ASSERTIONS_ONLY
        {
            assert_ptrs_();
            return alphas_;
        }

        /// \copydocs alpha_ptr(void) const
        double * alpha_ptr(void) ASSERTIONS_ONLY
        {
            assert_ptrs_();
            return alphas_;
        }


        /*! \brief Get the pointer to the coefficients of a particular general contraction
         * 
         * The coefficients for a particular general contraction are
         * assumed to be contiguous
         *
         * \param [in] n Index of the general contraction
         */
        double const * coef_ptr(size_t n) const ASSERTIONS_ONLY
        {
            assert_ptrs_();
            assert_gen_idx_(n);
            return coefs_ + n*nprim_;
        }

        /// \copydocs coef_ptr(size_t n) const
        double * coef_ptr(size_t n) ASSERTIONS_ONLY
        {
            assert_ptrs_();
            assert_gen_idx_(n);
            return coefs_ + n*nprim_;
        }



        /*! \brief Get the pointer to all coefficients
         * 
         * The coefficients are assumed to be contiguous, with the
         * general contraction index being the slowest index.
         */
        double const * all_coefs_ptr(void) const ASSERTIONS_ONLY
        {
            assert_ptrs_();
            return coefs_;
        }

        /// \copydocs all_coefs_ptr(void) const
        double * all_coefs_ptr(void) ASSERTIONS_ONLY
        {
            assert_ptrs_();
            return coefs_;
        }

        ///@}




        /*! \name Slow, safe element access */
        ///@{


        /*! \brief Get a single value for alpha (exponent)
         *
         * \throw pulsar::BasisSetException on out-of-bounds access
         *
         * \param [in] i Index of the primitive to get
         */
        double get_alpha(size_t i) const
        {
            assert_ptrs_();
            validate_prim_idx_(i);
            return alpha(i);
        }


        
        /*! \brief Set a single value for alpha (exponent)
         *
         * \throw pulsar::BasisSetException on out-of-bounds access
         *
         * \param [in] i Index of the primitive to set
         * \param [in] alpha The value to set the exponent to
         */
        void set_alpha(size_t i, double alpha)
        {
            assert_ptrs_();
            validate_prim_idx_(i);
            alphas_[i] = alpha;
        }



        /*! \brief Get a single value of a coefficient
         *
         * \throw pulsar::BasisSetException on out-of-bounds access
         *
         * \param [in] n Index of the general contraction
         * \param [in] i Index in the segmented contraction
         */
        double get_coef(size_t n, size_t i) const
        {
            assert_ptrs_();
            validate_prim_idx_(i);
            validate_gen_idx_(n);
            return coef(n, i);
        }



        /*! \brief Set a single value of a coefficient
         *
         * \throw pulsar::BasisSetException on out-of-bounds access
         *
         * \param [in] n Index of the general contraction
         * \param [in] i Index in the segmented contraction
         * \param [in] coef The value to set the coefficient to
         */
        void set_coef(size_t n, size_t i, double coef)
        {
            assert_ptrs_();
            validate_prim_idx_(i);
            validate_gen_idx_(n);
            coefs_[n*nprim_+i] = coef;
        }


        /// Get all exponents as a vector
        std::vector<double> get_alphas(void) const
        {
            assert_ptrs_();
            return std::vector<double>(alphas_, alphas_+nprim_);
        }


        /*! \brief Set all exponents via a vector
         *
         * \throw pulsar::BasisSetException if the vector is not of the right length
         */ 
        void set_alphas(const std::vector<double> & alphas)
        {
            
            assert_ptrs_();

            if(alphas.size() != nprim_)
                throw BasisSetException("Incompatible dimensions for alphas", "nprim", nprim_, "given", alphas.size());

            std::copy(alphas.begin(), alphas.end(), alphas_);
        }


        /*! \brief Get all coefficients of a general contraction as a vector
         * 
         * \param [in] n Index of the general contraction
         */
        std::vector<double> get_coefs(size_t n) const
        {
            assert_ptrs_();
            validate_gen_idx_(n);

            return std::vector<double>(coefs_+n*nprim_,
                                       coefs_+(n+1)*nprim_);
        }


        /*! \brief Set all coefficients for a general contraction via a vector
         * 
         * \throw pulsar::BasisSetException if the vector is not of the right length
         */
        void set_coefs(size_t n, const std::vector<double> & coefs)
        {
            
            assert_ptrs_();
            validate_gen_idx_(n);

            if(coefs.size() != nprim_)
                throw BasisSetException("Incompatible dimensions for coefficients",
                                        "nprim", nprim_, "given", coefs.size());

            std::copy(coefs.begin(), coefs.end(), coefs_ + n*nprim_);
        }

        /// Get all coefficints for all general contractions as a vector
        std::vector<double> get_all_coefs(void) const
        {
            assert_ptrs_();
            return std::vector<double>(coefs_, coefs_ + n_coefficients());
        }


        /*! \brief Set all coefficients for all general contractions via a vector
         * 
         * \throw pulsar::BasisSetException if the vector is not of the right length
         */
        void set_all_coefs(const std::vector<double> & coefs)
        {
            
            assert_ptrs_();
            if(coefs.size() != n_coefficients())
                throw BasisSetException("Incompatible dimensions for coefficients",
                                        "nprim", nprim_, "ngen", ngen_, "given", coefs.size());

            std::copy(coefs.begin(), coefs.end(), coefs_);
        }


        /*! \brief Set a primitive for a shell with no general contractions
         * 
         * \throw pulsar::BasisSetException if the shell has more than
         *        one general contraction or if there is out-of-bounds access
         * 
         * \param [in] i Index of the primitive
         * \param [in] alpha The exponent of the primitive
         * \param [in] coef The coefficient of the single general contraction
         */ 
        void set_primitive(size_t i, double alpha, double coef)
        {
            set_primitive(i, alpha, std::vector<double>{coef});
        }


        /*! \brief Set all information for a primitive
         * 
         * \throw pulsar::BasisSetException if \p coefs is not the right length
         *        or if there is out-of-bounds access
         *
         * \param [in] i Index of the primitive
         * \param [in] alpha The exponent of the primitive
         * \param [in] coefs Coefficients for the general contractions of this primitive
         */
        void set_primitive(size_t i, double alpha, const std::vector<double> & coefs)
        {
            
            assert_ptrs_();
            validate_prim_idx_(i);

            const size_t ncoefs = coefs.size();

            if(ncoefs != ngen_)
                throw BasisSetException("Incompatible dimensions for coefficients",
                                        "ngeneral", ngen_, "given", ncoefs);

            alphas_[i] = alpha;

            for(size_t n = 0; n < ngen_; n++)
                set_coef(n, i, coefs[n]);
        }
        ///@}


        bphash::HashValue my_hash(void) const
        {
            return bphash::make_hash(bphash::HashType::Hash128, *this);
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
            

            // If this is sp, spd, spdf, etc, then ngen
            // must be exactly 2, 3, 4 respectively
            if(am < 0 && ((-am)+1) != (int)ngen)
                throw BasisSetException("Invalid number of general contractions for special AM",
                                                   "am", am, "expected_ngen", ((-am)+1));

            // calculate the number of functions
            for(size_t i = 0; i < ngen_; i++)
                nfunc_ += general_n_functions(i);   
        }


        // compiler generated ok for move only
        BasisShellBase(const BasisShellBase &)             = delete;
        BasisShellBase(BasisShellBase &&)                  = default;
        BasisShellBase & operator=(const BasisShellBase &) = delete;
        BasisShellBase & operator=(BasisShellBase &&)      = default;


        /// Set the pointers for the exponents and coefficients
        void set_ptrs_(double * alphaptr, double * coefptr) ASSERTIONS_ONLY
        {
            alphas_ = alphaptr;
            coefs_ = coefptr;
            assert_ptrs_();
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
        bool base_compare_(const BasisShellBase & rhs) const
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
                       ( (coefs_ == rhs.coefs_) || std::equal(coefs_, coefs_ + n_coefficients(), rhs.coefs_) )
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
        void validate_prim_idx_(size_t i) const
        {
            
            if(i >= nprim_)
                throw BasisSetException("Attempt to access primitive that does not exist",
                                        "iprim", i, "nprim", nprim_);
        }

        void validate_gen_idx_(size_t n) const
        {
            
            if(n >= ngen_)
                throw BasisSetException("Attempt to access general contraction that does not exist",
                                        "igen", n, "ngen", ngen_);
        }

        void assert_prim_idx_(size_t i) const
        {
            
            psr_assert<BasisSetException>(i < nprim_, "Attempt to access primitive that does not exist",
                                                  "iprim", i, "nprim", nprim_);
        }

        void assert_gen_idx_(size_t n) const
        {
            
            psr_assert<BasisSetException>(n < ngen_, "Attempt to access general contraction that does not exist",
                                                 "igen", n, "ngen", ngen_);
        }

        void assert_ptrs_(void) const
        {
            
            psr_assert<BasisSetException>(alphas_ != nullptr && coefs_ != nullptr, "Null pointers in BasisShellBase");
        }



        //! \name Serialization
        ///@{

        DECLARE_SERIALIZATION_FRIENDS
        BPHASH_DECLARE_HASHING_FRIENDS

        template<class Archive>
        void serialize(Archive & ar)
        {
            // of course, we don't do the pointers. They are owned
            // by somebody else. It's their responsibilty
            ar(type_, am_, nprim_, ngen_, nfunc_);
        }

        void hash(bphash::Hasher & h) const
        {
            h(type_, am_, nprim_, ngen_, nfunc_,
                   bphash::hash_pointer(alphas_, n_primitives()),
                   bphash::hash_pointer(coefs_, n_coefficients()));
        }


        ///@}



};




} // close namespace system
} // close namespace pulsar


#endif
