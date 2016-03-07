#include "bpmodule/pragma.h"
#include "bpmodule/exception/Assert.hpp"
#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/system/BasisShellBase.hpp"
#include "bpmodule/system/NCartesian.hpp"

using bpmodule::exception::Assert;
using bpmodule::exception::BasisSetException;


namespace bpmodule {
namespace system {


BasisShellBase::BasisShellBase(ShellType type, int am, bool cart, int nprim, int ngen)
    : type_(type), am_(am), cart_(cart), nprim_(nprim), ngen_(ngen),
      alphas_(nullptr), coefs_(nullptr)
{
    // If this is sp, spd, spdf, etc, then ngen
    // must be exactly 2, 3, 4 respectively
    if(am < 0 && ((-am)+1) != ngen)
        throw BasisSetException("Invalid number of general contractions for special AM",
                                           "am", am, "expected_ngen", ((-am)+1));
}


void BasisShellBase::ValidateInput_(int n, int i) const
{
    ValidateInput_(i);

    if(n >= ngen_ || n < 0)
        throw BasisSetException("Attempt to access general contraction that does not exist",
                                           "igen", n, "ngen", ngen_);
}

void BasisShellBase::ValidateInput_(int i) const
{
    Assert<BasisSetException>(alphas_ != nullptr && coefs_ != nullptr, "Null pointers in BasisShellBase");
    if(i >= nprim_ || i < 0)
        throw BasisSetException("Attempt to access primitive that does not exist",
                                           "iprim", i, "nprim", nprim_);
}

void BasisShellBase::SetPtrs_(double * alphaptr, double * coefptr)
{
    alphas_ = alphaptr;
    coefs_ = coefptr;
    Assert<BasisSetException>(alphas_ != nullptr && coefs_ != nullptr, "Null pointers in BasisShellBase");
}


ShellType BasisShellBase::GetType(void) const noexcept
{
    return type_;
}

int BasisShellBase::AM(void) const noexcept
{
    return am_;
}

bool BasisShellBase::IsCombinedAM(void) const noexcept
{
    return am_ < 0;
}

int BasisShellBase::NPrim(void) const noexcept
{
    return nprim_;
}

int BasisShellBase::NCoef(void) const noexcept
{
    return ngen_*nprim_;
}

int BasisShellBase::NGeneral(void) const noexcept
{
    return ngen_;
}

int BasisShellBase::NCartesian(void) const noexcept
{
    if(IsCombinedAM())
        return NCARTESIAN(am_);
    else
        return ngen_*NCARTESIAN(am_);
}

int BasisShellBase::NSpherical(void) const noexcept
{
    if(IsCombinedAM())
        return NSPHERICAL(am_);
    else
        return ngen_*NSPHERICAL(am_);
}

int BasisShellBase::NFunctions(void) const noexcept
{
    return (cart_ ? NCartesian() : NSpherical());
}

bool BasisShellBase::IsCartesian(void) const noexcept
{
    return cart_;
}

bool BasisShellBase::IsSpherical(void) const noexcept
{
    return !cart_;
}


///////////////////////////////////
// Raw, fast, unsafe
// Here lies segfaults if you don't use it right
///////////////////////////////////
const double & BasisShellBase::Alpha(int i) const
{
    Assert<BasisSetException>(alphas_ != nullptr && coefs_ != nullptr, "Null pointers in BasisShellBase");
    Assert<BasisSetException>(i < nprim_ && i >= 0, "Primitive index out of range", "idx", i, "nprim", nprim_);
    return alphas_[i]; 
}

double & BasisShellBase::Alpha(int i)
{
    Assert<BasisSetException>(alphas_ != nullptr && coefs_ != nullptr, "Null pointers in BasisShellBase");
    Assert<BasisSetException>(i < nprim_ && i >= 0, "Primitive index out of range", "idx", i, "nprim", nprim_);
    return alphas_[i]; 
}

const double & BasisShellBase::Coef(int n, int i) const
{
    Assert<BasisSetException>(alphas_ != nullptr && coefs_ != nullptr, "Null pointers in BasisShellBase");
    Assert<BasisSetException>(i < nprim_ && i >= 0, "Primitive index out of range", "idx", i, "nprim", nprim_);
    Assert<BasisSetException>(n < ngen_ && n >= 0, "General contraction index out of range", "idx", i, "nprim", nprim_);
    return coefs_[n*nprim_+i]; 
}

double & BasisShellBase::Coef(int n, int i)
{
    Assert<BasisSetException>(alphas_ != nullptr && coefs_ != nullptr, "Null pointers in BasisShellBase");
    Assert<BasisSetException>(i < nprim_ && i >= 0, "Primitive index out of range", "idx", i, "nprim", nprim_);
    Assert<BasisSetException>(n < ngen_ && n >= 0, "General contraction index out of range", "idx", i, "nprim", nprim_);
    return coefs_[n*nprim_+i]; 
}

double const * BasisShellBase::AlphaPtr(void) const
{
    Assert<BasisSetException>(alphas_ != nullptr && coefs_ != nullptr, "Null pointers in BasisShellBase");
    return alphas_;
}

double * BasisShellBase::AlphaPtr(void)
{
    Assert<BasisSetException>(alphas_ != nullptr && coefs_ != nullptr, "Null pointers in BasisShellBase");
    return alphas_;
}

double const * BasisShellBase::CoefPtr(int n) const
{
    Assert<BasisSetException>(alphas_ != nullptr && coefs_ != nullptr, "Null pointers in BasisShellBase");
    Assert<BasisSetException>(n < ngen_ && n >= 0, "General contraction index out of range", "idx", n, "ngen", ngen_);
    return coefs_ + n*nprim_;
}

double * BasisShellBase::CoefPtr(int n)
{
    Assert<BasisSetException>(alphas_ != nullptr && coefs_ != nullptr, "Null pointers in BasisShellBase");
    Assert<BasisSetException>(n < ngen_ && n >= 0, "General contraction index out of range", "idx", n, "ngen", ngen_);
    return coefs_ + n*nprim_;
}

double const * BasisShellBase::AllCoefsPtr(void) const
{
    Assert<BasisSetException>(alphas_ != nullptr && coefs_ != nullptr, "Null pointers in BasisShellBase");
    return coefs_;
}

double * BasisShellBase::AllCoefsPtr(void)
{
    Assert<BasisSetException>(alphas_ != nullptr && coefs_ != nullptr, "Null pointers in BasisShellBase");
    return coefs_;
}


//////////////////////////////////////////
// Slow, safe access with defensive copy
//////////////////////////////////////////
double BasisShellBase::GetAlpha(int i) const
{
    ValidateInput_(i);
    return Alpha(i);
}

void BasisShellBase::SetAlpha(int i, double alpha)
{
    ValidateInput_(i);
    Alpha(i) = alpha;
}

double BasisShellBase::GetCoef(int n, int i) const
{
    ValidateInput_(n, i);
    return Coef(n, i);
}

void BasisShellBase::SetCoef(int n, int i, double coef)
{
    ValidateInput_(n, i);
    Coef(n, i) = coef;
}

std::vector<double> BasisShellBase::GetAlphas(void) const
{
    return std::vector<double>(alphas_, alphas_+nprim_);
}

void BasisShellBase::SetAlphas(const std::vector<double> & alphas)
{
    if(static_cast<int>(alphas.size()) != nprim_)
        throw BasisSetException("Incompatible dimensions for alphas", "nprim", nprim_, "given", alphas.size());

    std::copy(alphas.begin(), alphas.end(), alphas_);
}


std::vector<double> BasisShellBase::GetCoefs(int n) const
{
    if(n >= ngen_ || n < 0)
        throw BasisSetException("Attempt to access general contraction that does not exist",
                                "igen", n, "ngen", ngen_);

    return std::vector<double>(coefs_+n*nprim_,
                               coefs_+(n+1)*nprim_);
}

void BasisShellBase::SetCoefs(int n, const std::vector<double> & coefs)
{
    if(n >= ngen_ || n < 0)
        throw BasisSetException("Attempt to access general contraction that does not exist",
                                "igen", n, "ngen", ngen_);

    if(static_cast<int>(coefs.size()) != nprim_)
        throw BasisSetException("Incompatible dimensions for coefficients", "nprim", nprim_, "given", coefs.size());

    std::copy(coefs.begin(), coefs.end(), coefs_ + n*nprim_);
}

std::vector<double> BasisShellBase::GetAllCoefs(void) const
{
    return std::vector<double>(coefs_, coefs_+ngen_*nprim_);
}

void BasisShellBase::SetAllCoefs(const std::vector<double> & coefs)
{
    if(static_cast<int>(coefs.size()) != nprim_*ngen_)
        throw BasisSetException("Incompatible dimensions for coefficients", "nprim", nprim_, "ngen", ngen_, "given", coefs.size());
    std::copy(coefs.begin(), coefs.end(), coefs_);
}


void BasisShellBase::SetPrimitive(int i, double alpha, double coef)
{
    SetPrimitive(i, alpha, std::vector<double>{coef});
}


void BasisShellBase::SetPrimitive(int i, double alpha, const std::vector<double> & coefs)
{
    ValidateInput_(i);

    const int ncoefs = static_cast<int>(coefs.size());

    if(ncoefs != ngen_)
        throw BasisSetException("Incompatible dimensions for coefficients", "ngeneral", ngen_, "given", ncoefs);

    alphas_[i] = alpha;

    for(int n = 0; n < ngen_; n++)
        Coef(n, i) = coefs[n];
}


bool BasisShellBase::operator==(const BasisShellBase & rhs) const
{
    PRAGMA_WARNING_PUSH
    PRAGMA_WARNING_IGNORE_FP_EQUALITY

    // ordered by what is most likely to fail first (takes advantage
    // of short circuiting). When we get to alphas, we check to see if the pointers
    // are the same first. If they are different, we compare elementwise. This should
    // be save, since nprim_ and ngen_ are already shown to be equal
    return (
               am_ == rhs.am_ &&
               ngen_ == rhs.ngen_ &&
               nprim_ == rhs.nprim_ &&
               type_ == rhs.type_ &&
               cart_ == rhs.cart_ &&
               ( (alphas_ == rhs.alphas_) || std::equal(alphas_, alphas_+nprim_, rhs.alphas_) ) &&
               ( (coefs_ == rhs.coefs_) || std::equal(coefs_, coefs_+nprim_*ngen_, rhs.coefs_) )
           );

    PRAGMA_WARNING_POP
}


} // close namespace system
} // close namespace bpmodule

