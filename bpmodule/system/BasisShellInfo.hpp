#ifndef BPMODULE_GUARD_SYSTEM__BASISSHELLINFO_HPP_
#define BPMODULE_GUARD_SYSTEM__BASISSHELLINFO_HPP_

#include <map>

#include "bpmodule/system/BasisShellBase.hpp"

namespace bpmodule {
namespace system {




// Meant to be stored with the system
class BasisShellInfo : public BasisShellBase
{
    public:
        BasisShellInfo(ShellType type, int am, bool cart, int nprim, int ngen);


        // compiler generated ok
        BasisShellInfo(const BasisShellInfo &)             = default;
        BasisShellInfo(BasisShellInfo &&)                  = default;
        BasisShellInfo & operator=(const BasisShellInfo &) = default;
        BasisShellInfo & operator=(BasisShellInfo &&)      = default;

        bool operator==(const BasisShellInfo & rhs) const;


    private:
        std::vector<double> alphas_; //!< Exponents (storage)
        std::vector<double> coefs_;  //!< Coefficients (storage)
};


typedef std::vector<BasisShellInfo> BasisShellInfoVector;
typedef std::map<std::string, BasisShellInfoVector> BasisShellInfoMap;


} // close namespace system
} // close namespace bpmodule


#endif
