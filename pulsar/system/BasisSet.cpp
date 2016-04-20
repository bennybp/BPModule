/*! \file
 *
 * \brief Main BasisSet object (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "pulsar/system/BasisSet.hpp"
#include "pulsar/system/NFunction.hpp"
#include "pulsar/output/Output.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/exception/Assert.hpp"
#include "pulsar/util/HashSerializable.hpp"

using pulsar::exception::Assert;
using pulsar::exception::BasisSetException;

namespace pulsar{
namespace system {


BasisSet::BasisSet(size_t nshells, size_t nprim, size_t ncoef, size_t nxyz)
    : curid_(0)
{
    Allocate_(nshells, nprim, ncoef, nxyz);
}


BasisSet::BasisSet(const BasisSet & rhs)
    : curid_(rhs.curid_),
      unique_shells_(rhs.unique_shells_),
      storage_(rhs.storage_),
      max_nxyz_(rhs.max_nxyz_),
      max_nalpha_(rhs.max_nalpha_),
      max_ncoef_(rhs.max_ncoef_),
      xyz_pos_(rhs.xyz_pos_),
      alpha_pos_(rhs.alpha_pos_),
      coef_pos_(rhs.coef_pos_)
{
    // storage has been copied   
    // but all the pointers in shells_ would be incorrect
    // so we have to rebuild them
    ResetPointers_();

    shells_.reserve(rhs.shells_.size());
    for(const auto & it : rhs.shells_)
    {
        // determine offsets for this shells xyz, alpha, and coefs
        // They are currently relative to the beginning of rhs.storage_
        ptrdiff_t alpha_offset = it.AlphaPtr()    - rhs.storage_.data();
        ptrdiff_t coef_offset =  it.AllCoefsPtr() - rhs.storage_.data();
        ptrdiff_t xyz_offset =   it.CoordsPtr()   - rhs.storage_.data();
        double * sbegin = storage_.data();
        shells_.push_back(BasisSetShell(it, sbegin + alpha_offset,
                                            sbegin + coef_offset,
                                            sbegin + xyz_offset));
    }

    // double check
    Assert<BasisSetException>(shells_ == rhs.shells_, "Developer error. Inconsistent basis set copying");
}



void BasisSet::Allocate_(size_t nshells, size_t nprim, size_t ncoef, size_t nxyz)
{
    // totalstorage = number of doubles to store
    // nshells*3 = storage for xyz
    // nprim = storage for alpha
    // ncoef = storage for coefficients
    size_t totalstorage = nxyz + nprim + ncoef;

    storage_.resize(totalstorage);

    max_nalpha_ = nprim;
    max_ncoef_ = ncoef;
    max_nxyz_ = nxyz;

    ResetPointers_();
    xyz_pos_ = alpha_pos_ = coef_pos_ = 0;
}


void BasisSet::ResetPointers_(void)
{
    xyz_base_ptr_ = storage_.data();
    alpha_base_ptr_ = xyz_base_ptr_ + max_nxyz_;
    coef_base_ptr_ = alpha_base_ptr_ + max_nalpha_;
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
    return shells_ == rhs.shells_;
}

bool BasisSet::operator!=(const BasisSet & rhs) const
{
    return !((*this) == rhs);
}


util::Hash BasisSet::MyHash(void) const
{
    return util::HashSerializable(*this);
}


void BasisSet::AddShell_(const BasisShellBase & bshell,
                         ID_t id, ID_t center,
                         const CoordType & xyz)
{
    // have the coordinates been added already?
    auto it = std::find_if(shells_.begin(), shells_.end(),
                           [& xyz](const BasisSetShell & b)
                           { return b.GetCoords() == xyz; });

    double * my_xyz = nullptr;

    if(it != shells_.end())
        my_xyz = it->CoordsPtr();
    else
    {
        // do we have enough room for xyz
        if(xyz_pos_ + 3 > max_nxyz_)
            throw BasisSetException("Not enough storage for this shell: too many coordinates to store",
                                    "max", max_nxyz_,
                                    "current", xyz_pos_, "toadd", 1);

        // need a non-const temporary
        my_xyz = xyz_base_ptr_ + xyz_pos_;
        std::copy(xyz.begin(), xyz.end(), my_xyz);
        xyz_pos_ += 3; // advance where we are putting xyz coords
    }
     


    // Check to see if alpha & coefs has been added already
    it = std::find_if(shells_.begin(), shells_.end(),
                      [& bshell](const BasisSetShell & b) { return b.BaseCompare(bshell); });


    if(it != shells_.end())
    {
        // equivalent shell already exists! Use the primitives,
        // but copy center, etc, from bshell
        shells_.push_back(BasisSetShell(bshell,
                                        it->AlphaPtr(), it->AllCoefsPtr(),
                                        my_xyz, id, center));
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

        // unique_shells_ stores the index in the shells_ vector
        // the index of this new shell will be shells_.size()
        unique_shells_.push_back(shells_.size());

        // actually add the shell
        shells_.push_back(BasisSetShell(bshell, my_alpha, my_coef, my_xyz, id, center));

        // advance these
        alpha_pos_ += bshell.NPrim();
        coef_pos_ += bshell.NCoef();
    }
}


void BasisSet::AddShell_(const BasisSetShell & bshell)
{
    // Adds an existing basis set shell
    // Will copy all data
    AddShell_(bshell, bshell.GetID(), bshell.GetCenter(), bshell.GetCoords());
}


void BasisSet::AddShell(const BasisShellInfo & bshell,
                        ID_t center,
                        const CoordType & xyz)
{
    // Add from a BasisShellInfo
    // Will copy all data
    AddShell_(bshell, curid_++, center, xyz);
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
    BasisShellInfo bsi(bss.GetType(), bss.AM(),
                       bss.NPrim(), bss.NGeneral());

    bsi.SetAlphas(bss.GetAlphas());
    bsi.SetAllCoefs(bss.GetAllCoefs());
    return bsi;
}

size_t BasisSet::NPrim(void) const
{
    return std::accumulate(this->begin(), this->end(), static_cast<size_t>(0),
                           [](size_t sum, const BasisSetShell & sh)
                           { return sum + sh.NPrim(); } );
}

size_t BasisSet::NCoef(void) const
{
    return std::accumulate(this->begin(), this->end(), static_cast<size_t>(0),
                           [](size_t sum, const BasisSetShell & sh)
                           { return sum + sh.NCoef(); } );
}

size_t BasisSet::MaxNPrim(void) const
{
    return std::max_element(this->begin(), this->end(), 
                            [](const BasisSetShell & lhs, const BasisSetShell & rhs)
                            { return lhs.NPrim() < rhs.NPrim(); })->NPrim();
}

int BasisSet::MaxAM(void) const
{
    int max = std::max_element(this->begin(), this->end(), 
                               [](const BasisSetShell & lhs, const BasisSetShell & rhs)
                               { return std::abs(lhs.AM()) < std::abs(rhs.AM()); })->AM();
    return std::abs(max);
}

std::set<int> BasisSet::AllAM(void) const
{
    std::set<int> res;
    for(const auto & it : *this)
        res.insert(it.AM());
    return res;
}

size_t BasisSet::NFunctions(void) const
{
    return std::accumulate(this->begin(), this->end(), static_cast<size_t>(0),
                           [](size_t sum, const BasisSetShell & sh)
                           { return sum + sh.NFunctions(); } );
}

size_t BasisSet::MaxNFunctions(void) const
{
    return std::max_element(this->begin(), this->end(), 
                            [](const BasisSetShell & lhs, const BasisSetShell & rhs)
                            { return lhs.NFunctions() < rhs.NFunctions(); })->NFunctions();
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
    BasisSet bs(NShell(), NPrim(), NCoef(), 3*NPrim());
    for(const auto & shell : shells_)
        bs.AddShell_(transformer(shell));

    return bs.ShrinkFit();
}

BasisSet BasisSet::ShrinkFit(void) const
{
    using std::swap;

    // Be super safe
    // We check <= , since the *_pos variables represent where we would put
    // the next one. If pos_ == max_n, then it is full. If it is greater, then
    // one was already placed where it wasn't supposed to go...
    Assert<BasisSetException>(xyz_pos_ <= max_nxyz_,
                              "Developer error. Too many xyz in basis set",
                              "pos", xyz_pos_, "max", max_nxyz_);
    Assert<BasisSetException>(alpha_pos_ <= max_nalpha_,
                              "Developer error. Too many alpha in basis set",
                              "pos", alpha_pos_, "max", max_nalpha_);
    Assert<BasisSetException>(coef_pos_ <= max_ncoef_,
                              "Developer error. Too many coefficients in basis set",
                              "pos", coef_pos_, "max", max_ncoef_);


    // We need to know the actual number of stored primitives and coefficients. These
    // are stored in *_pos_ variables.
    // shells_.size() is self explanatory
    BasisSet newbs(shells_.size(), alpha_pos_, coef_pos_, xyz_pos_);

    // Just push through what we have
    // using the private functions. This will check for overflow, etc
    for(const auto & it : shells_)
        newbs.AddShell_(it);

    // Set the curid_
    newbs.curid_ = curid_;

    return newbs;
}


void BasisSet::Print(std::ostream & os) const
{
    using namespace pulsar::output;

    size_t nshell = NShell();

    Output(os, "Basis set with %? shells\n", nshell);
    Output(os, "NFunc = %? , MaxAM = %?\n", NFunctions(), MaxAM());
    Output(os, "MaxNFunction = %? , MaxNPrim = %?\n", MaxNFunctions(), MaxNPrim());
    Debug(os, "Space usage: XYZ: %?/%?  Alpha: %?/%?  Coef %?/%?\n", xyz_pos_, max_nxyz_,
                                                                     alpha_pos_, max_nalpha_,
                                                                     coef_pos_, max_ncoef_);


    for(size_t i = 0; i < nshell; i++)
    {
        const auto & shell = Shell(i);
        Output(os, "Shell %?  AM=%?  NPrim=%? NGen=%?\n", i, shell.AM(), shell.NPrim(), shell.NGeneral());
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
} // close namespace pulsar
