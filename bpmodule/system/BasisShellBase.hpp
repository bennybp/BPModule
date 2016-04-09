/*! \file
 *
 * \brief Base class for basis set shells (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef BPMODULE_GUARD_SYSTEM__BASISSHELLBASE_HPP_
#define BPMODULE_GUARD_SYSTEM__BASISSHELLBASE_HPP_

#include <vector>
#include "bpmodule/util/Serialization.hpp"
#include "bpmodule/exception/Assert.hpp"


namespace bpmodule {
namespace system {


/*! \brief Type of basis shells */
enum class ShellType
{
    Gaussian,
    Slater
};




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


        /*! \brief Compare with another BasisShellBase
         * 
         * Used instead of to prevent comparisons with derived classes when
         * called from the derived class operator==
         *
         * This compares all properties and values, looking for an exact match.
         */
        bool BaseCompare(const BasisShellBase & rhs) const;


        /*! \name General properties */
        ///@{ 

        /// Get the type of shell
        ShellType GetType(void) const noexcept;

        /// Get this shells angular momentum
        int AM(void) const noexcept;

        /// Get the number of primitives in this shell
        size_t NPrim(void) const noexcept;

        /*! \brief Get the number of coefficients in this shell
         * 
         * This takes into account general contractions and combined shells,
         * so it is not necessarily equal to the number of primitives
         */
        size_t NCoef(void) const noexcept;

        /// Get the level of general contraction of this shell
        size_t NGeneral(void) const noexcept;


        /*! \brief Get the number of cartesian functions represented by this shell
         * 
         * This takes into account general contractions and combined shells, so it
         * is not simply based on the AM alone
         */
        size_t NCartesian(void) const noexcept;


        /*! \brief Get the number of spherical functions represented by this shell
         * 
         * This takes into account general contractions and combined shells, so it
         * is not simply based on the AM alone
         */
        size_t NSpherical(void) const noexcept;


        /*! \brief Get the number of functions represented by this shell
         *
         * Returns NCartesian() if this shell represents cartesian basis functions,
         * or NSpherical() if this shell represents spherical basis functions. 
         *
         * This takes into account general contractions and combined shells, so it
         * is not simply based on the AM alone
         */
        size_t NFunctions(void) const noexcept;


        /*! \brief Is this shell a combined AM shell
         * 
         * A combined AM shell is a generally-contracted shell where
         * the general contractions represent different angular momentum.
         * For example, sp and spd shells.
         */
        bool IsCombinedAM(void) const noexcept;


        /// Does this shell represent cartesian basis functions
        bool IsCartesian(void) const noexcept;

        /// Does this shell represent spherical basis functions
        bool IsSpherical(void) const noexcept;

        ///@}



        /*! \name Raw, fast, unsafe access */
        ///@{ 

        /*! \brief Get a single value for alpha (exponent)
         *
         * \param [in] i Index of the primitive to get
         */
        const double & Alpha(size_t i) const ASSERTIONS_ONLY;

        /// \copydocs Alpha(size_t i) const
        double & Alpha(size_t i) ASSERTIONS_ONLY;



        /*! \brief Get a single value of a coefficient
         *
         * \param [in] n Index of the general contraction
         * \param [in] i Index in the segmented contraction
         */
        const double & Coef(size_t n, size_t i) const ASSERTIONS_ONLY;

        /// \copydocs Coef(size_t n, size_t i) const
        double & Coef(size_t n, size_t i) ASSERTIONS_ONLY;


        /*! \brief Get the pointer to the exponents
         */
        double const * AlphaPtr(void) const ASSERTIONS_ONLY;

        /// \copydocs AlphaPtr(void) const
        double * AlphaPtr(void) ASSERTIONS_ONLY;


        /*! \brief Get the pointer to the coefficients of a particular general contraction
         * 
         * The coefficients for a particular general contraction are
         * assumed to be contiguous
         *
         * \param [in] n Index of the general contraction
         */
        double const * CoefPtr(size_t n) const ASSERTIONS_ONLY;

        /// \copydocs CoefPtr(size_t n) const
        double * CoefPtr(size_t n) ASSERTIONS_ONLY;



        /*! \brief Get the pointer to all coefficients
         * 
         * The coefficients are assumed to be contiguous, with the
         * general contraction index being the slowest index.
         */
        double const * AllCoefsPtr(void) const ASSERTIONS_ONLY;

        /// \copydocs AllCoefsPtr(void) const
        double * AllCoefsPtr(void) ASSERTIONS_ONLY;

        ///@}




        /*! \name Slow, safe element access */
        ///@{


        /*! \brief Get a single value for alpha (exponent)
         *
         * \throw bpmodule::exception::BasisSetException on out-of-bounds access
         *
         * \param [in] i Index of the primitive to get
         */
        double GetAlpha(size_t i) const;


        
        /*! \brief Set a single value for alpha (exponent)
         *
         * \throw bpmodule::exception::BasisSetException on out-of-bounds access
         *
         * \param [in] i Index of the primitive to set
         * \param [in] alpha The value to set the exponent to
         */
        void SetAlpha(size_t i, double alpha);



        /*! \brief Get a single value of a coefficient
         *
         * \throw bpmodule::exception::BasisSetException on out-of-bounds access
         *
         * \param [in] n Index of the general contraction
         * \param [in] i Index in the segmented contraction
         */
        double GetCoef(size_t n, size_t i) const;



        /*! \brief Set a single value of a coefficient
         *
         * \throw bpmodule::exception::BasisSetException on out-of-bounds access
         *
         * \param [in] n Index of the general contraction
         * \param [in] i Index in the segmented contraction
         * \param [in] coef The value to set the coefficient to
         */
        void SetCoef(size_t n, size_t i, double coef);


        /// Get all exponents as a vector
        std::vector<double> GetAlphas(void) const;


        /*! \brief Set all exponents via a vector
         *
         * \throw bpmodule::exception::BasisSetException if the vector is not of the right length
         */ 
        void SetAlphas(const std::vector<double> & alphas);


        /*! \brief Get all coefficients of a general contraction as a vector
         * 
         * \param [in] n Index of the general contraction
         */
        std::vector<double> GetCoefs(size_t n) const;


        /*! \brief Set all coefficients for a general contraction via a vector
         * 
         * \throw bpmodule::exception::BasisSetException if the vector is not of the right length
         */
        void SetCoefs(size_t n, const std::vector<double> & coefs);

        /// Get all coefficints for all general contractions as a vector
        std::vector<double> GetAllCoefs(void) const;


        /*! \brief Set all coefficients for all general contractions via a vector
         * 
         * \throw bpmodule::exception::BasisSetException if the vector is not of the right length
         */
        void SetAllCoefs(const std::vector<double> & coefs);


        /*! \brief Set a primitive for a shell with no general contractions
         * 
         * \throw bpmodule::exception::BasisSetException if the shell has more than
         *        one general contraction or if there is out-of-bounds access
         * 
         * \param [in] i Index of the primitive
         * \param [in] alpha The exponent of the primitive
         * \param [in] coef The coefficient of the single general contraction
         */ 
        void SetPrimitive(size_t i, double alpha, double coef);


        /*! \brief Set all information for a primitive
         * 
         * \throw bpmodule::exception::BasisSetException if \p coefs is not the right length
         *        or if there is out-of-bounds access
         *
         * \param [in] i Index of the primitive
         * \param [in] alpha The exponent of the primitive
         * \param [in] coefs Coefficients for the general contractions of this primitive
         */
        void SetPrimitive(size_t i, double alpha, const std::vector<double> & coefs);

        ///@}



    protected:

        /*! \brief Constructor
         * 
         * \param [in] type Type of the shell
         * \param [in] am Angular momentum of the shell
         * \param [in] cart True if this shell is cartesian, false if spherical
         * \param [in] nprim Number of primitives
         * \param [in] ngen Number of general contractions
         */
        BasisShellBase(ShellType type, int am, bool cart, size_t nprim, size_t ngen);


        // compiler generated ok
        BasisShellBase(const BasisShellBase &)             = default;
        BasisShellBase(BasisShellBase &&)                  = default;
        BasisShellBase & operator=(const BasisShellBase &) = default;
        BasisShellBase & operator=(BasisShellBase &&)      = default;


        /// Set the pointers for the exponents and coefficients
        void SetPtrs_(double * alphaptr, double * coefptr) ASSERTIONS_ONLY;

        // For serialization only (needed by derived classes)
        BasisShellBase() = default;


    private:
        ShellType type_;             //!< Gaussian, Slater, etc
        int am_;                     //!< Angular momentum
        bool cart_;                  //!< Is cartesian?
        size_t nprim_;                  //!< Number of primitives
        size_t ngen_;                   //!< Number of general contractions
        double * alphas_;            //!< Exponents
        double * coefs_;             //!< Coefficients


        // Assertions and sanity checks
        void AssertPtrs_(void) const;
        void AssertPrimIdx_(size_t i) const;
        void AssertGenIdx_(size_t n) const;
        void ValidatePrimIdx_(size_t i) const;
        void ValidateGenIdx_(size_t n) const;


        //! \name Serialization
        ///@{

        DECLARE_SERIALIZATION_FRIENDS

        template<class Archive>
        void serialize(Archive & ar)
        {
            // of course, we don't do the pointers. They are owned
            // by somebody else. It's their responsibilty
            ar(type_, am_, cart_, nprim_, ngen_);
        }

        ///@}



};




} // close namespace system
} // close namespace bpmodule


#endif
