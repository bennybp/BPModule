#include "bpmodule/pragma.h"
#include "bpmodule/exception/Assert.hpp"
#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/system/BasisShellInfo.hpp"
#include "bpmodule/system/NCartesian.hpp"

using bpmodule::exception::Assert;
using bpmodule::exception::BasisSetException;


namespace bpmodule {
namespace system {

BasisShellInfo::BasisShellInfo(ShellType type, int am, bool cart, int nprim, int ngen)
    : type_(type), am_(am), cart_(cart), nprim_(nprim), ngen_(ngen)
{
    // If this is sp, spd, spdf, etc, then ngen
    // must be exactly 2, 3, 4 respectively
    if(am < 0 && ((-am)+1) != ngen)
        throw BasisSetException("Invalid number of general contractions for special AM",
                                           "am", am, "expected_ngen", ((-am)+1));

    alphas_.resize(nprim);
    coefs_.resize(nprim * ngen);
}


void BasisShellInfo::ValidateInput_(int n, int i) const
{
    ValidateInput_(i);

    if(n >= ngen_ || n < 0)
        throw BasisSetException("Attempt to access general contraction that does not exist",
                                           "igen", n, "ngen", ngen_);
}

void BasisShellInfo::ValidateInput_(int i) const
{
    Assert<BasisSetException>(static_cast<int>(alphas_.size()) == nprim_, "Incompatible dimensions on alphas_ array");
    Assert<BasisSetException>(static_cast<int>(coefs_.size()) == ngen_*nprim_, "Incompatible dimensions on coefs_ array");

    if(i >= nprim_ || i < 0)
        throw BasisSetException("Attempt to access primitive that does not exist",
                                           "iprim", i, "nprim", nprim_);
}


ShellType BasisShellInfo::GetType(void) const
{
    return type_;
}

int BasisShellInfo::AM(void) const noexcept
{
    return am_;
}

bool BasisShellInfo::IsCombinedAM(void) const noexcept
{
    return am_ < 0;
}

int BasisShellInfo::NPrim(void) const noexcept
{
    return nprim_;
}

int BasisShellInfo::NGeneral(void) const noexcept
{
    return ngen_;
}

int BasisShellInfo::NCartesian(void) const noexcept
{
    if(IsCombinedAM())
        return NCARTESIAN(am_);
    else
        return ngen_*NCARTESIAN(am_);
}

int BasisShellInfo::NSpherical(void) const noexcept
{
    if(IsCombinedAM())
        return NSPHERICAL(am_);
    else
        return ngen_*NSPHERICAL(am_);
}

int BasisShellInfo::NFunctions(void) const noexcept
{
    return (cart_ ? NCartesian() : NSpherical());
}

bool BasisShellInfo::IsCartesian(void) const noexcept
{
    return cart_;
}

bool BasisShellInfo::IsSpherical(void) const noexcept
{
    return !cart_;
}


double BasisShellInfo::GetAlpha(int i) const
{
    ValidateInput_(i);
    return alphas_.at(i);
}

void BasisShellInfo::SetAlpha(int i, double alpha)
{
    ValidateInput_(i);
    alphas_.at(i) = alpha;
}

std::vector<double> BasisShellInfo::GetAlphas(void) const
{
    return alphas_;
}

void BasisShellInfo::SetAlphas(const std::vector<double> & alphas)
{
    if(static_cast<int>(alphas.size()) != nprim_)
        throw BasisSetException("Incompatible dimensions for alphas", "nprim", nprim_, "given", alphas.size());

    alphas_ = alphas;
}


double BasisShellInfo::GetCoef(int n, int i) const
{
    ValidateInput_(n, i);
    return coefs_.at(n*NPrim()+i);
}

void BasisShellInfo::SetCoef(int n, int i, double coef)
{
    ValidateInput_(n, i);
    coefs_.at(n*NPrim()+i) = coef;
}

std::vector<double> BasisShellInfo::GetCoefs(int n) const
{
    return std::vector<double>(coefs_.begin()+n*nprim_,
                               coefs_.begin()+(n+1)*nprim_);
}

void BasisShellInfo::SetCoefs(int n, const std::vector<double> & coefs)
{
    if(n >= ngen_ || n < 0)
        throw BasisSetException("Attempt to access general contraction that does not exist",
                                "igen", n, "ngen", ngen_);

    if(static_cast<int>(coefs.size()) != nprim_)
        throw BasisSetException("Incompatible dimensions for coefficients", "nprim", nprim_, "given", coefs.size());

    std::copy(coefs.begin(), coefs.end(), coefs_.begin() + n*nprim_);
}

std::vector<double> BasisShellInfo::GetAllCoefs(void) const
{
    return coefs_;
}




void BasisShellInfo::SetPrimitive(int i, double alpha, double coef)
{
    SetPrimitive(i, alpha, std::vector<double>{coef});
}

void BasisShellInfo::SetPrimitive(int i, double alpha, const std::vector<double> & coefs)
{
    ValidateInput_(i);

    const int ncoefs = static_cast<int>(coefs.size());

    if(ncoefs != ngen_)
        throw BasisSetException("Incompatible dimensions for coefficients", "ngeneral", ngen_, "given", ncoefs);

    alphas_[i] = alpha;

    for(int n = 0; n < ngen_; n++)
        coefs_[n*nprim_+i] = coefs[n];
}

bool BasisShellInfo::operator==(const BasisShellInfo & rhs) const
{
    PRAGMA_WARNING_PUSH
    PRAGMA_WARNING_IGNORE_FP_EQUALITY

    return (
               am_ == rhs.am_ &&
               ngen_ == rhs.ngen_ &&
               nprim_ == rhs.nprim_ &&
               type_ == rhs.type_ &&
               cart_ == rhs.cart_ &&
               alphas_ == rhs.alphas_ &&
               coefs_ == rhs.coefs_
           );

    PRAGMA_WARNING_POP
}


} // close namespace system
} // close namespace bpmodule

