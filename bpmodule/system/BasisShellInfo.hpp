#ifndef BPMODULE_GUARD_BASISSET__BASISSHELLINFO_HPP_
#define BPMODULE_GUARD_BASISSET__BASISSHELLINFO_HPP_

#include <map>
#include <string>
#include <array>
#include <vector>
#include "bpmodule/pragma.h"
#include "bpmodule/system/NCartesian.hpp"


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
        BasisShellInfo(ShellType type, int am, bool cart)
            : type_(type), am_(am), cart_(cart)
        { }


        // compiler generated ok
        BasisShellInfo(const BasisShellInfo &)             = default;
        BasisShellInfo(BasisShellInfo &&)                  = default;
        BasisShellInfo & operator=(const BasisShellInfo &) = default;
        BasisShellInfo & operator=(BasisShellInfo &&)      = default;


        ShellType GetType(void) const { return type_; }

        int AM(void) const noexcept { return am_; }

        int NPrim(void) const noexcept { return static_cast<int>(alphas_.size()); }
        int NCartesian(void) const noexcept { return NCARTESIAN(am_); }
        int NSpherical(void) const noexcept { return NSPHERICAL(am_); }
        int NFunctions(void) const noexcept { return (cart_ ? NCartesian() : NSpherical()); } 

        bool IsCartesian(void) const noexcept { return cart_; }
        bool IsSpherical(void) const noexcept { return !cart_; }


        //! \todo check if zero nprim?
        std::vector<double> Alphas(void) const noexcept { return alphas_; }
        std::vector<double> Coefs(void) const noexcept { return coefs_; }


        //! \todo wrap exceptions
        double Alpha(int i) const { return alphas_.at(i); }
        double Coef(int i) const { return coefs_.at(i); }

        void AddPrimitive(double alpha, double coef)
        {
            alphas_.push_back(alpha);
            coefs_.push_back(coef);
        }

        bool operator==(const BasisShellInfo & rhs) const
        {
            PRAGMA_WARNING_PUSH
            PRAGMA_WARNING_IGNORE_FP_EQUALITY

            return (
                     type_ == rhs.type_ &&
                     am_ == rhs.am_ &&
                     cart_ == rhs.cart_ &&
                     alphas_ == rhs.alphas_ &&
                     coefs_ == rhs.coefs_
                   ); 

            PRAGMA_WARNING_POP
        }

    private:
        ShellType type_;
        int am_;                    //!< Angular momentum
        bool cart_;                 //!< Is cartesian?
        std::vector<double> alphas_; //!< Exponents
        std::vector<double> coefs_; //!< Exponents
};


typedef std::vector<BasisShellInfo> BasisShellInfoVector;
typedef std::map<std::string, BasisShellInfoVector> BasisShellInfoMap;


} // close namespace system
} // close namespace bpmodule


#endif
