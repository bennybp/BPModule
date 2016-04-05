/*! \file
 *
 * \brief Base class for basis set shells (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

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

void BasisShellBase::ValidatePrimIdx_(int i) const
{
    if(i >= nprim_ || i < 0)
        throw BasisSetException("Attempt to access primitive that does not exist",
                                "iprim", i, "nprim", nprim_);
}

void BasisShellBase::ValidateGenIdx_(int n) const
{
    if(n >= ngen_ || n < 0)
        throw BasisSetException("Attempt to access general contraction that does not exist",
                                "igen", n, "ngen", ngen_);
}

void BasisShellBase::AssertPrimIdx_(int i) const
{
    Assert<BasisSetException>(i < nprim_ && i >= 0, "Attempt to access primitive that does not exist",
                                                   "iprim", i, "nprim", nprim_);
}

void BasisShellBase::AssertGenIdx_(int n) const
{
    Assert<BasisSetException>(n < ngen_ && n >= 0, "Attempt to access general contraction that does not exist",
                                                   "igen", n, "ngen", ngen_);
}

void BasisShellBase::AssertPtrs_(void) const
{
    Assert<BasisSetException>(alphas_ != nullptr && coefs_ != nullptr, "Null pointers in BasisShellBase");
}

void BasisShellBase::SetPtrs_(double * alphaptr, double * coefptr) ASSERTIONS_ONLY
{
    alphas_ = alphaptr;
    coefs_ = coefptr;
    AssertPtrs_();
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
const double & BasisShellBase::Alpha(int i) const ASSERTIONS_ONLY
{
    AssertPtrs_();
    AssertPrimIdx_(i);
    return alphas_[i]; 
}

double & BasisShellBase::Alpha(int i) ASSERTIONS_ONLY
{
    AssertPtrs_();
    AssertPrimIdx_(i);
    return alphas_[i]; 
}

const double & BasisShellBase::Coef(int n, int i) const ASSERTIONS_ONLY
{
    AssertPtrs_();
    AssertPrimIdx_(i);
    AssertGenIdx_(n);
    return coefs_[n*nprim_+i]; 
}

double & BasisShellBase::Coef(int n, int i) ASSERTIONS_ONLY
{
    AssertPtrs_();
    AssertPrimIdx_(i);
    AssertGenIdx_(n);
    return coefs_[n*nprim_+i]; 
}

double const * BasisShellBase::AlphaPtr(void) const ASSERTIONS_ONLY
{
    AssertPtrs_();
    return alphas_;
}

double * BasisShellBase::AlphaPtr(void) ASSERTIONS_ONLY
{
    AssertPtrs_();
    return alphas_;
}

double const * BasisShellBase::CoefPtr(int n) const ASSERTIONS_ONLY
{
    AssertPtrs_();
    AssertGenIdx_(n);
    return coefs_ + n*nprim_;
}

double * BasisShellBase::CoefPtr(int n) ASSERTIONS_ONLY
{
    AssertPtrs_();
    AssertGenIdx_(n);
    return coefs_ + n*nprim_;
}

double const * BasisShellBase::AllCoefsPtr(void) const ASSERTIONS_ONLY
{
    AssertPtrs_();
    return coefs_;
}

double * BasisShellBase::AllCoefsPtr(void) ASSERTIONS_ONLY
{
    AssertPtrs_();
    return coefs_;
}


//////////////////////////////////////////
// Slow, safe access with defensive copy
//////////////////////////////////////////
double BasisShellBase::GetAlpha(int i) const
{
    AssertPtrs_();
    ValidatePrimIdx_(i);
    return Alpha(i);
}

void BasisShellBase::SetAlpha(int i, double alpha)
{
    AssertPtrs_();
    ValidatePrimIdx_(i);
    Alpha(i) = alpha;
}

double BasisShellBase::GetCoef(int n, int i) const
{
    AssertPtrs_();
    ValidatePrimIdx_(i);
    ValidateGenIdx_(n);
    return Coef(n, i);
}

void BasisShellBase::SetCoef(int n, int i, double coef)
{
    AssertPtrs_();
    ValidatePrimIdx_(i);
    ValidateGenIdx_(n);
    Coef(n, i) = coef;
}

std::vector<double> BasisShellBase::GetAlphas(void) const
{
    AssertPtrs_();
    return std::vector<double>(alphas_, alphas_+nprim_);
}

void BasisShellBase::SetAlphas(const std::vector<double> & alphas)
{
    AssertPtrs_();

    if(static_cast<int>(alphas.size()) != nprim_)
        throw BasisSetException("Incompatible dimensions for alphas", "nprim", nprim_, "given", alphas.size());

    std::copy(alphas.begin(), alphas.end(), alphas_);
}


std::vector<double> BasisShellBase::GetCoefs(int n) const
{
    AssertPtrs_();
    ValidateGenIdx_(n);

    return std::vector<double>(coefs_+n*nprim_,
                               coefs_+(n+1)*nprim_);
}

void BasisShellBase::SetCoefs(int n, const std::vector<double> & coefs)
{
    AssertPtrs_();
    ValidateGenIdx_(n);

    if(static_cast<int>(coefs.size()) != nprim_)
        throw BasisSetException("Incompatible dimensions for coefficients", "nprim", nprim_, "given", coefs.size());

    std::copy(coefs.begin(), coefs.end(), coefs_ + n*nprim_);
}

std::vector<double> BasisShellBase::GetAllCoefs(void) const
{
    AssertPtrs_();
    return std::vector<double>(coefs_, coefs_ + NCoef());
}

void BasisShellBase::SetAllCoefs(const std::vector<double> & coefs)
{
    AssertPtrs_();
    if(static_cast<int>(coefs.size()) != NCoef())
        throw BasisSetException("Incompatible dimensions for coefficients", "nprim", nprim_, "ngen", ngen_, "given", coefs.size());
    std::copy(coefs.begin(), coefs.end(), coefs_);
}


void BasisShellBase::SetPrimitive(int i, double alpha, double coef)
{
    SetPrimitive(i, alpha, std::vector<double>{coef});
}


void BasisShellBase::SetPrimitive(int i, double alpha, const std::vector<double> & coefs)
{
    AssertPtrs_();
    ValidatePrimIdx_(i);

    const int ncoefs = static_cast<int>(coefs.size());

    if(ncoefs != ngen_)
        throw BasisSetException("Incompatible dimensions for coefficients", "ngeneral", ngen_, "given", ncoefs);

    alphas_[i] = alpha;

    for(int n = 0; n < ngen_; n++)
        Coef(n, i) = coefs[n];
}



bool BasisShellBase::BaseCompare(const BasisShellBase & rhs) const
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
               cart_ == rhs.cart_ &&
               ( (alphas_ == rhs.alphas_) || std::equal(alphas_, alphas_+nprim_, rhs.alphas_) ) &&
               ( (coefs_ == rhs.coefs_) || std::equal(coefs_, coefs_ + NCoef(), rhs.coefs_) )
           );

    PRAGMA_WARNING_POP
}


} // close namespace system
} // close namespace bpmodule

