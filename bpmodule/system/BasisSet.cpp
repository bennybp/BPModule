#include "bpmodule/system/BasisSet.hpp"
#include "bpmodule/system/NCartesian.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/exception/Assert.hpp"
#include "bpmodule/util/HashSerializable.hpp"

using bpmodule::exception::Assert;
using bpmodule::exception::BasisSetException;

namespace bpmodule {
namespace system {


void BasisSet::Allocate_(size_t nshells, size_t nprim, size_t ncoef)
{
    // totalstorage = number of doubles to store
    // nshells*3 = storage for xyz
    // nprim = storage for alpha
    // ncoef = storage for coefficients
    size_t totalstorage = nshells*3 + nprim + ncoef;

    storage_.resize(totalstorage);

    // for debugging, you can fill with a random value
    //std::fill(storage_.begin(), storage_.end(), 0.0);

    max_nalpha_ = nprim;
    max_ncoef_ = ncoef;
    max_nxyz_ = nshells*3;

    ResetPointers_();
}


BasisSet::BasisSet(size_t nshells, size_t nprim, size_t ncoef)
    : curid_(0),
      xyz_base_ptr_(nullptr),
      alpha_base_ptr_(nullptr),
      coef_base_ptr_(nullptr)
{
    Allocate_(nshells, nprim, ncoef);
}


BasisSet::BasisSet(const BasisSet & rhs)
    : curid_(rhs.curid_),
      storage_(rhs.storage_.size()),
      max_nxyz_(rhs.max_nxyz_),
      max_nalpha_(rhs.max_nalpha_),
      max_ncoef_(rhs.max_ncoef_)
{
    // storage has been copied   
    // but all the pointers in shells_ would be incorrect
    // so we have to rebuild them
    ResetPointers_();

    // This should leave all the IDs as they are,
    // so we copy curid_ above
    for(const auto & it : rhs)
        AddShell_(it);

    // double check
    if(alpha_pos_ != rhs.alpha_pos_ ||
       coef_pos_ != rhs.coef_pos_ ||
       xyz_pos_ != rhs.xyz_pos_)
        throw BasisSetException("Developer error. Inconsistent basis set copying");

    // triple check
    Assert<BasisSetException>(shells_ == rhs.shells_, "Developer error. Inconsistent basis set copying");
}


void BasisSet::ResetPointers_(void)
{
    xyz_base_ptr_ = storage_.data();
    alpha_base_ptr_ = xyz_base_ptr_ + max_nxyz_;
    coef_base_ptr_ = alpha_base_ptr_ + max_nalpha_;
    xyz_pos_ = alpha_pos_ = coef_pos_ = 0;
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

bool BasisSet::operator==(const BasisSet & rhs) const
{
    // this will take into account if one is shrinkfit and the
    // other isn't. Will also be true even if storage is different
    // size, etc.
    //! \todo Will not take ordering into account. Is that desired?
    return shells_ == rhs.shells_;
}


util::Hash BasisSet::MyHash(void) const
{
    return util::HashSerializable(*this);
}


void BasisSet::AddShell_(const BasisShellBase & bshell,
                         ID_t id, ID_t center,
                         const CoordType & xyz)
{
    // always have to add xyz, so we always have to check this
    if(xyz_pos_ + 3 > max_nxyz_)
        throw BasisSetException("Not enough storage for this shell: too many coordinates to store",
                                           "max", max_nxyz_,
                                           "current", xyz_pos_, "toadd", 1);  

    // we always have to add the xyz coordinates
    double * const my_xyz = xyz_base_ptr_ + xyz_pos_;
    std::copy(xyz.begin(), xyz.end(), my_xyz);
    xyz_pos_ += 3; // advance where we are putting xyz coords

    // Check to see if the other prim data has been added already
    auto it = std::find_if(shells_.begin(), shells_.end(),
                           [& bshell](const BasisSetShell & b) { return b.BaseCompare(bshell); });


    if(it != shells_.end())
    {
        // equivalent shell already exists! Use the primitives,
        // but copy center, etc, from bshell
        shells_.push_back(BasisSetShell(bshell,
                                        it->AlphaPtr(),
                                        it->AllCoefsPtr(),
                                        my_xyz,
                                        id, center));
    }
    else
    {
        if(alpha_pos_ + bshell.NPrim() > max_nalpha_)
            throw BasisSetException("Not enough storage for this shell: too may primitives",
                                               "max", max_nalpha_,
                                               "current", alpha_pos_, "toadd", bshell.NPrim());  

        if(coef_pos_ + bshell.NCoef() > max_ncoef_)
            throw BasisSetException("Not enough storage for this shell: too many coefficients",
                                               "max", max_ncoef_,
                                               "current", coef_pos_, "toadd", bshell.NCoef());  

        // copy the primitives from bshell
        double const * const old_alphaptr = bshell.AlphaPtr();
        double const * const old_coefptr = bshell.AllCoefsPtr();
        double * const my_alpha = alpha_base_ptr_ + alpha_pos_;
        double * const my_coef = coef_base_ptr_ + coef_pos_;
        std::copy(old_alphaptr, old_alphaptr+bshell.NPrim(), my_alpha);
        std::copy(old_coefptr, old_coefptr+bshell.NCoef(), my_coef);

        // the index of this new shell will be shells_.size()
        unique_shells_.push_back(shells_.size());
        shells_.push_back(BasisSetShell(bshell, my_alpha, my_coef, my_xyz, id, center));

        // advance these
        alpha_pos_ += bshell.NPrim();
        coef_pos_ += bshell.NCoef();
    }

}


void BasisSet::AddShell_(const BasisSetShell & bshell)
{
    AddShell_(bshell, bshell.GetID(), bshell.GetCenter(), bshell.GetCoords());
}


void BasisSet::AddShell(const BasisShellInfo & bshell,
                        ID_t center,
                        const CoordType & xyz)
{
    AddShell_(bshell, curid_++, center, xyz);
}

bool BasisSet::IsUniqueShell_(size_t i) const
{
    return (std::find(unique_shells_.begin(), unique_shells_.end(), i) != unique_shells_.end());
}



size_t BasisSet::NShell(void) const noexcept
{
    return shells_.size();
}

size_t BasisSet::NUniqueShell(void) const noexcept
{
    return unique_shells_.size();
}


const BasisSetShell & BasisSet::Shell(size_t i) const
{
    if(i < shells_.size())
        return shells_[i];
    else
        throw BasisSetException("Shell index out of range",
                                "index", i, "nshells", shells_.size());
}

const BasisSetShell & BasisSet::UniqueShell(size_t i) const
{
    if(i < unique_shells_.size())
        return Shell(unique_shells_.at(i));
    else
        throw BasisSetException("Unique shell index out of range",
                                "index", i, "nshells", unique_shells_.size());
}

BasisShellInfo BasisSet::ShellInfo(size_t i) const
{
    const BasisSetShell & bss = Shell(i);
    BasisShellInfo bsi(bss.GetType(), bss.AM(), bss.IsCartesian(),
                       bss.NPrim(), bss.NGeneral());

    bsi.SetAlphas(bss.GetAlphas());
    bsi.SetAllCoefs(bss.GetAllCoefs());
    return bsi;
}

size_t BasisSet::NPrim(void) const
{
    return std::accumulate(this->begin(), this->end(), 0,
                           [](size_t sum, const BasisSetShell & sh) { return sum + sh.NPrim(); } );
}

size_t BasisSet::NCoef(void) const
{
    return std::accumulate(this->begin(), this->end(), 0,
                           [](size_t sum, const BasisSetShell & sh) { return sum + sh.NCoef(); } );
}

size_t BasisSet::MaxNPrim(void) const
{
    return std::max_element(this->begin(), this->end(), 
                    [](const BasisSetShell & lhs, const BasisSetShell & rhs) { return lhs.NPrim() < rhs.NPrim(); })->NPrim();
}

int BasisSet::MaxAM(void) const
{
    int max = std::max_element(this->begin(), this->end(), 
                    [](const BasisSetShell & lhs, const BasisSetShell & rhs)
                      { return std::abs(lhs.AM()) < std::abs(rhs.AM()); })->AM();
    return std::abs(max);
}

size_t BasisSet::NCartesian(void) const
{
    return std::accumulate(this->begin(), this->end(), 0,
                           [](size_t sum, const BasisSetShell & sh) { return sum + sh.NCartesian(); } );
}

size_t BasisSet::NFunctions(void) const
{
    return std::accumulate(this->begin(), this->end(), 0,
                           [](size_t sum, const BasisSetShell & sh) { return sum + sh.NFunctions(); } );
}

size_t BasisSet::MaxNCartesian(void) const
{
    return NCARTESIAN(MaxAM());
}

size_t BasisSet::MaxNFunctions(void) const
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
    BasisSet bs(NShell(), NPrim(), NCoef());
    for(const auto & shell : shells_)
        bs.AddShell_(transformer(shell));
    return bs;
}

BasisSet BasisSet::TransformUnique(BasisSet::TransformerFunc transformer) const
{
    BasisSet bs(NShell(), NPrim(), NCoef());
    for(size_t i = 0; i < shells_.size(); i++)
    {
        const auto & shell = shells_[i];
        if(IsUniqueShell_(i))
            bs.AddShell_(transformer(shell));
        else
            bs.AddShell_(shell);
    }
    return bs;
}


BasisSet BasisSet::ShrinkFit(void) const
{
    using std::swap;

    // Be super safe
    // We check <= , since the *_pos variables represent where we would put
    // the next one. If pos_ == max_n, then it is full. If it is greater, then
    // one was already placed where it wasn't supposed to go...
    Assert<BasisSetException>(xyz_pos_ == (shells_.size()*3),
                              "Developer error. Inconsistent number of coordinates in basis set",
                              "nxyz", xyz_pos_, "nshells", shells_.size());
    Assert<BasisSetException>(xyz_pos_ <= max_nxyz_,
                              "Developer error. Too many xyz in basis set",
                              "pos", xyz_pos_, "max", max_nxyz_);
    Assert<BasisSetException>(alpha_pos_ <= max_nalpha_,
                              "Developer error. Too many alpha in basis set",
                              "pos", alpha_pos_, "max", max_nalpha_);
    Assert<BasisSetException>(coef_pos_ <= max_ncoef_,
                              "Developer error. Too many coefficients in basis set",
                              "pos", coef_pos_, "max", max_ncoef_);


    // alpha_pos_ = current position of alpha. This corresponds to
    //              the number of primitives (each primitive has only one alpha)
    // coef_pos_ = current position of coefficients. This would represent the number
    //             of coefficients
    // shells_.size() is self explanatory
    BasisSet newbs(shells_.size(), alpha_pos_, coef_pos_);


    // Just push through what we have
    // using the private functions
    for(const auto & it : shells_)
        newbs.AddShell_(it);

    // Set the curid_
    newbs.curid_ = curid_;

    return newbs;
}


void BasisSet::Print(std::ostream & os) const
{
    using namespace bpmodule::output;

    size_t nshell = NShell();

    Output(os, "Basis set with %? shells\n", nshell);
    Output(os, "NCart = %? , MaxAM = %?\n", NCartesian(), MaxAM());
    Output(os, "MaxNCart = %? , MaxNPrim = %?\n", MaxNCartesian(), MaxNPrim());
    Debug(os, "Space usage: XYZ: %?/%?  Alpha: %?/%?  Coef %?/%?\n", xyz_pos_, max_nxyz_,
                                                                 alpha_pos_, max_nalpha_,
                                                                 coef_pos_, max_ncoef_);


    for(size_t i = 0; i < nshell; i++)
    {
        const auto & shell = Shell(i);
        Output(os, "Shell %?  AM=%?  Cart=%?  NPrim=%? NGen=%?\n", i, shell.AM(), shell.IsCartesian(), shell.NPrim(), shell.NGeneral());
        Output(os, "Coordinates: % 16.8? % 16.8? % 16.8?\n", shell.GetCoords()[0], shell.GetCoords()[1], shell.GetCoords()[2]);
        for(size_t j = 0; j < shell.NPrim(); ++j)
        {
            Output(os, "    % 16.8?", shell.GetAlpha(j));
            for(size_t n = 0; n < shell.NGeneral(); n++)
                Output(os, "    % 16.8?", shell.GetCoef(n, j));
            Output(os, "\n");
        }
    }
    Output(os, "\n");
}


} // close namespace system
} // close namespace bpmodule
