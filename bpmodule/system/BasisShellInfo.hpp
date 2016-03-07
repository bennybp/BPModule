#ifndef BPMODULE_GUARD_BASISSET__BASISSHELLINFO_HPP_
#define BPMODULE_GUARD_BASISSET__BASISSHELLINFO_HPP_

#include <map>
#include <string>
#include <vector>


namespace bpmodule {
namespace system {



enum class ShellType
{
    Gaussian,
    Slater
};


// Meant to be stored with the molecule
class BasisShellInfo
{
    public:
        BasisShellInfo(ShellType type, int am, bool cart, int nprim, int ngen);


        // compiler generated ok
        BasisShellInfo(const BasisShellInfo &)             = default;
        BasisShellInfo(BasisShellInfo &&)                  = default;
        BasisShellInfo & operator=(const BasisShellInfo &) = default;
        BasisShellInfo & operator=(BasisShellInfo &&)      = default;


        ShellType GetType(void) const;

        int AM(void) const noexcept;

        int NPrim(void) const noexcept;
        int NGeneral(void) const noexcept;
        int NCartesian(void) const noexcept;
        int NSpherical(void) const noexcept;
        int NFunctions(void) const noexcept;

        bool IsCombinedAM(void) const noexcept;
        bool IsCartesian(void) const noexcept;
        bool IsSpherical(void) const noexcept;


        std::vector<double> GetAlphas(void) const;
        void SetAlphas(const std::vector<double> & alphas);

        std::vector<double> GetCoefs(int n) const;
        void SetCoefs(int n, const std::vector<double> & coefs);

        std::vector<double> GetAllCoefs(void) const;


        double GetAlpha(int i) const;
        void SetAlpha(int i, double alpha);
        double GetCoef(int n, int i) const;
        void SetCoef(int n, int i, double coef);

        void SetPrimitive(int i, double alpha, double coef);
        void SetPrimitive(int i, double alpha, const std::vector<double> & coefs);

        bool operator==(const BasisShellInfo & rhs) const;


    private:
        ShellType type_;             //!< Gaussian, Slater, etc
        int am_;                     //!< Angular momentum
        bool cart_;                  //!< Is cartesian?
        int nprim_;                  //!< Number of primitives
        int ngen_;                   //!< Number of general contractions
        std::vector<double> alphas_; //!< Exponents
        std::vector<double> coefs_;  //!< Coefficients


        
        void ValidateInput_(int n, int i) const;
        void ValidateInput_(int i) const;
};


typedef std::vector<BasisShellInfo> BasisShellInfoVector;
typedef std::map<std::string, BasisShellInfoVector> BasisShellInfoMap;


} // close namespace system
} // close namespace bpmodule


#endif
