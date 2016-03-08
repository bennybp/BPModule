#include <algorithm>

#include "bpmodule/system/BasisSet.hpp"
#include "bpmodule/system/NCartesian.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/exception/Assert.hpp"

using bpmodule::exception::Assert;
using bpmodule::exception::BasisSetException;

namespace bpmodule {
namespace system {

BasisSet::BasisSet(size_t nprim, size_t ncoef)
    : curid_(0), nprim_(nprim)
{
    size_t totalstorage = nprim + ncoef;

    storage_.resize(totalstorage);
    std::fill(storage_.begin(), storage_.end(), 0.0);

    alpha_pos_ = 0;
    coef_pos_ = nprim_;
}

BasisSet::BasisSet(const BasisSet & rhs)
    : curid_(rhs.curid_),
      storage_(rhs.storage_),
      nprim_(rhs.nprim_)
{
    alpha_pos_ = 0;
    coef_pos_ = nprim_;

    // storage has been copied   
    // but all the pointers in shells_ would be incorrect
    // so we have to rebuild them

    // This should leave all the IDs as they are, so we
    // copy curid_ above
    for(const auto & it : rhs)
        AddShell_(it);

    // double check
    if(alpha_pos_ != rhs.alpha_pos_ || coef_pos_ != rhs.coef_pos_)
        throw BasisSetException("Developer error. Inconsistent basis set copying");

    // triple check
    Assert<BasisSetException>(shells_ == rhs.shells_, "Developer error. Inconsistent basis set copying");
}

BasisSet & BasisSet::operator=(const BasisSet & rhs)
{
    using std::swap;

    if(this == &rhs)
        return *this;

    BasisSet tmp(rhs);
    swap(*this, tmp);
    return *this;
}


void BasisSet::ValidateAddition_(const BasisShellBase & bshell) const
{
    // Does this fit?
    if(alpha_pos_ + bshell.NPrim() > nprim_)
        throw BasisSetException("Not enough storage for this shell: too may primitives",
                                           "nprim", nprim_,
                                           "current", alpha_pos_, "toadd", bshell.NPrim());  

    if(coef_pos_ + bshell.NCoef() > storage_.size())
        throw BasisSetException("Not enough storage for this shell: too many coefficients",
                                           "nprim", nprim_,
                                           "current", coef_pos_, "toadd", bshell.NCoef());  
}


void BasisSet::AddShell_(const BasisSetShell & bshell)
{
    // Check to see if the prim data has been added already
    auto it = std::find_if(shells_.begin(), shells_.end(),
                           [& bshell](const BasisSetShell & b) { return b.BaseCompare(bshell); });
    if(it != shells_.end())
    {
        // equivalent shell already exists! Use the primitives,
        // but copy center, etc, from bshell
        shells_.push_back(BasisSetShell(bshell,
                                        it->AlphaPtr(),
                                        it->AllCoefsPtr()));
    }
    else
    {
        ValidateAddition_(bshell);

        shells_.push_back(BasisSetShell(bshell,
                                        storage_.data() + alpha_pos_,
                                        storage_.data() + coef_pos_));

        alpha_pos_ += bshell.NPrim();
        coef_pos_ += bshell.NCoef();
    }
}


void BasisSet::AddShell(const BasisShellInfo & bshell,
                        unsigned long center,
                        const BasisSetShell::CoordType & xyz)
{
    auto it = std::find_if(shells_.begin(), shells_.end(),
                           [& bshell](const BasisSetShell & b) { return b.BaseCompare(bshell); });
    if(it != shells_.end())
    {
        // equivalent shell already exists! Use the primitives,
        // but copy center, etc, from bshell
        shells_.push_back(BasisSetShell(bshell,
                                        it->AlphaPtr(),
                                        it->AllCoefsPtr(),
                                        curid_++, center, xyz));
    }
    else
    {
        ValidateAddition_(bshell);

        shells_.push_back(BasisSetShell(bshell,
                                        storage_.data() + alpha_pos_,
                                        storage_.data() + coef_pos_,
                                        curid_++, center, xyz));


        alpha_pos_ += bshell.NPrim();
        coef_pos_ += bshell.NCoef();
    }
}




int BasisSet::NShell(void) const noexcept
{
    return static_cast<int>(shells_.size());
}


const BasisSetShell & BasisSet::GetShell(int i) const
{
    if(static_cast<size_t>(i) < shells_.size() )
        return shells_[i];
    else
        throw BasisSetException("Shell index out of range",
                                           "index", i, "nshells", shells_.size());
}

int BasisSet::NPrim(void) const
{
    return std::accumulate(this->begin(), this->end(), 0,
                           [](int sum, const BasisSetShell & sh) { return sum + sh.NPrim(); } );
}

int BasisSet::NCoef(void) const
{
    return std::accumulate(this->begin(), this->end(), 0,
                           [](int sum, const BasisSetShell & sh) { return sum + sh.NCoef(); } );
}

int BasisSet::MaxNPrim(void) const
{
    return std::max_element(this->begin(), this->end(), 
                    [](const BasisSetShell & lhs, const BasisSetShell & rhs) { return lhs.NPrim() < rhs.NPrim(); })->NPrim();
}

int BasisSet::MaxAM(void) const
{
    int max = std::max_element(this->begin(), this->end(), 
                    [](const BasisSetShell & lhs, const BasisSetShell & rhs) { return std::abs(lhs.AM()) < std::abs(rhs.AM()); })->AM();
    return std::abs(max);
}

int BasisSet::NCartesian(void) const
{
    return std::accumulate(this->begin(), this->end(), 0,
                           [](int sum, const BasisSetShell & sh) { return sum + sh.NCartesian(); } );
}

int BasisSet::NFunctions(void) const
{
    return std::accumulate(this->begin(), this->end(), 0,
                           [](int sum, const BasisSetShell & sh) { return sum + sh.NFunctions(); } );
}

int BasisSet::MaxNCartesian(void) const
{
    return NCARTESIAN(MaxAM());
}

int BasisSet::MaxNFunctions(void) const
{
    return std::max_element(this->begin(), this->end(), 
                    [](const BasisSetShell & lhs, const BasisSetShell & rhs) { return lhs.NFunctions() < rhs.NFunctions(); })->NFunctions();
}

BasisSet::const_iterator BasisSet::begin(void) const
{
    return shells_.begin();
}

BasisSet::const_iterator BasisSet::end(void) const
{
    return shells_.end();
}

BasisSet BasisSet::Transform(BasisSet::TransformerFunc transformer) const
{
    BasisSet bs(NPrim(), NCoef());
    for(auto shell : bs.shells_)
        bs.AddShell_(transformer(shell));
    return bs;
}


void BasisSet::Shrink(void)
{
    using std::swap;

    // index math...
    // nprim_ represents the number of primitives we were expected to
    // store. storage_ + nprim_ = start of coefficients

    // Calculate how many primitives and coefficients are stored, taking
    // into account duplicates
    size_t actual_nprim = alpha_pos_;
    size_t actual_ncoef = coef_pos_ - nprim_;

    std::vector<double> newstorage;
    newstorage.resize(actual_nprim + actual_ncoef);

    // copy over alphas
    std::copy(storage_.begin(),
              storage_.begin()+actual_nprim,
              newstorage.begin());

    // copy over coefficients
    std::copy(storage_.begin()+nprim_,
              storage_.begin()+nprim_+actual_ncoef,
              newstorage.begin()+actual_nprim);

    // Reset all the pointers for the shells
    std::vector<BasisSetShell> newshells;
    newshells.reserve(shells_.size());

    // Offsets in the old storage
    double const * const alpha_base = storage_.data();
    double const * const coef_base = storage_.data() + nprim_;

    for(const auto & it : shells_)
    {
        // calculate the old offset
        size_t alpha_offset = it.AlphaPtr() - alpha_base; 
        size_t coef_offset = it.AllCoefsPtr() - coef_base; 

        // convert to the new pointers
        newshells.push_back(BasisSetShell(it,
                                          newstorage.data()+alpha_offset,
                                          newstorage.data()+actual_nprim+coef_offset));
    }

    // make sure to set this
    nprim_ = actual_nprim;

    // alpha_pos_ stays the same, but coef_pos_ must change
    coef_pos_ = nprim_ + actual_ncoef;

    // swap out the data
    swap(storage_, newstorage);
    swap(shells_, newshells); 
}


void BasisSet::Print(void) const
{
    int nshell = NShell();

    output::Output("Basis set with %1% shells\n", nshell);
    output::Output("NCart = %1% , MaxAM = %2%\n", NCartesian(), MaxAM());
    output::Output("MaxNCart = %1% , MaxNPrim = %2%\n", MaxNCartesian(), MaxNPrim());
    output::Debug("Space usage: Alpha: %1%/%2%\n", alpha_pos_, nprim_);
    output::Debug("Space usage: Coefs: %1%/%2%\n", coef_pos_-nprim_, storage_.size()-nprim_);
    output::Debug("Space usage: Total: %1%/%2%\n", alpha_pos_+coef_pos_-nprim_, storage_.size());


    for(int i = 0; i < nshell; i++)
    {
        const auto & shell = GetShell(i);
        output::Output("Shell %1%  AM=%2%  Cart=%3%  NPrim=%4% NGen=%5%\n", i, shell.AM(), shell.IsCartesian(), shell.NPrim(), shell.NGeneral());
        for(int j = 0; j < shell.NPrim(); ++j)
        {
            output::Output("    %1%", shell.GetAlpha(j));
            for(int n = 0; n < shell.NGeneral(); n++)
                output::Output("    %1%", shell.GetCoef(n, j));
            output::Output("\n");
        }
    }
    output::Output("\n");
}


} // close namespace system
} // close namespace bpmodule
