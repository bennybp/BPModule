#ifndef BPMODULE_GUARD_SYSTEM__BASISSHELLBASE_HPP_
#define BPMODULE_GUARD_SYSTEM__BASISSHELLBASE_HPP_

#include <string>
#include <vector>


namespace bpmodule {
namespace system {



enum class ShellType
{
    Gaussian,
    Slater
};





class BasisShellBase
{
    public:
        virtual ~BasisShellBase() = default;

        /*! \name General properties */
        ///@{ 
        ShellType GetType(void) const noexcept;

        int AM(void) const noexcept;

        int NPrim(void) const noexcept;
        int NCoef(void) const noexcept;
        int NGeneral(void) const noexcept;
        int NCartesian(void) const noexcept;
        int NSpherical(void) const noexcept;
        int NFunctions(void) const noexcept;

        bool IsCombinedAM(void) const noexcept;
        bool IsCartesian(void) const noexcept;
        bool IsSpherical(void) const noexcept;

        std::string GetDesc(void) const;

        bool operator==(const BasisShellBase & rhs) const;

        bool BaseCompare(const BasisShellBase & rhs) const;

        ///@}

        /*! \name Raw, fast, unsafe access */
        ///@{ 

        const double & Alpha(int i) const;
        double & Alpha(int i);

        const double & Coef(int n, int i) const;
        double & Coef(int n, int i) ;

        double const * AlphaPtr(void) const;
        double * AlphaPtr(void);

        double const * CoefPtr(int n) const;
        double * CoefPtr(int n);

        double const * AllCoefsPtr(void) const;
        double * AllCoefsPtr(void);

        ///@}


        /*! \name Slow, safe element access */
        ///@{

        double GetAlpha(int i) const;
        void SetAlpha(int i, double alpha);
        double GetCoef(int n, int i) const;
        void SetCoef(int n, int i, double coef);


        std::vector<double> GetAlphas(void) const;
        void SetAlphas(const std::vector<double> & alphas);

        std::vector<double> GetCoefs(int n) const;
        void SetCoefs(int n, const std::vector<double> & coefs);

        std::vector<double> GetAllCoefs(void) const;
        void SetAllCoefs(const std::vector<double> & coefs);

        void SetPrimitive(int i, double alpha, double coef);
        void SetPrimitive(int i, double alpha, const std::vector<double> & coefs);

        ///@}

    protected:
        BasisShellBase(ShellType type, int am, bool cart, int nprim,
                       int ngen, const std::string & desc);

        // compiler generated ok
        BasisShellBase(const BasisShellBase &)             = default;
        BasisShellBase(BasisShellBase &&)                  = default;
        BasisShellBase & operator=(const BasisShellBase &) = default;
        BasisShellBase & operator=(BasisShellBase &&)      = default;


        void SetPtrs_(double * alphaptr, double * coefptr);


    private:
        ShellType type_;             //!< Gaussian, Slater, etc
        int am_;                     //!< Angular momentum
        bool cart_;                  //!< Is cartesian?
        int nprim_;                  //!< Number of primitives
        int ngen_;                   //!< Number of general contractions
        double * alphas_;            //!< Exponents
        double * coefs_;             //!< Coefficients

        std::string desc_;           //!< Short description of where this came from

        void AssertPtrs_(void) const;
        void AssertPrimIdx_(int i) const;
        void AssertGenIdx_(int n) const;
        void ValidatePrimIdx_(int i) const;
        void ValidateGenIdx_(int n) const;
};




} // close namespace system
} // close namespace bpmodule


#endif
