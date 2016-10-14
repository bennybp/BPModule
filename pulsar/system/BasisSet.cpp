/*! \file
 *
 * \brief Main BasisSet object (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "pulsar/system/BasisSet.hpp"
#include "pulsar/system/NFunction.hpp"
#include "pulsar/output/Output.hpp"
#include "pulsar/output/GlobalOutput.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/exception/Assert.hpp"

#include "bphash/types/memory.hpp"
#include "bphash/types/vector.hpp"

using pulsar::psr_assert;
using pulsar::BasisSetException;

/*! \todo std::max_element requires that the container not be empty. Clean that up */

namespace pulsar{
namespace system {


BasisSet::BasisSet(size_t nshells, size_t nprim, size_t ncoef, size_t nxyz)
{
    allocate_(nshells, nprim, ncoef, nxyz);
}


BasisSet::BasisSet(const BasisSet & rhs)
    : unique_shells_(rhs.unique_shells_),
      storage_(rhs.storage_),
      shellstart_(rhs.shellstart_),
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
        ptrdiff_t alpha_offset = it.alpha_ptr()    - rhs.storage_.data();
        ptrdiff_t coef_offset =  it.all_coefs_ptr() - rhs.storage_.data();
        ptrdiff_t xyz_offset =   it.coords_ptr()   - rhs.storage_.data();
        double * sbegin = storage_.data();
        shells_.push_back(std::move(BasisSetShell(it, sbegin + alpha_offset,
                                                  sbegin + coef_offset,
                                                  sbegin + xyz_offset)));
    }

    // double check
    psr_assert<BasisSetException>(shells_ == rhs.shells_, "Developer error. Inconsistent basis set copying");
}



void BasisSet::allocate_(size_t /*nshells*/, size_t nprim, size_t ncoef, size_t nxyz)
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


bphash::HashValue BasisSet::my_hash(void) const
{
    return bphash::make_hash(bphash::HashType::Hash128, *this);
}

void BasisSet::hash(bphash::Hasher & h) const
{
    h(shells_, unique_shells_, storage_);
}


std::set<ShellType> BasisSet::get_types(void) const
{
    std::set<ShellType> types;
    for(const auto & s : shells_)
        types.insert(s.get_type());
    return types;
}

size_t BasisSet::shell_start(size_t i) const
{
    psr_assert<BasisSetException>(shells_.size() == shellstart_.size(),
                              "Developer error. nshells != length of shellstart",
                              "nshells", shells_.size(), "nshellstart", shellstart_.size());

    if(i < shellstart_.size())
        return shellstart_[i];
    else
        throw BasisSetException("Shell index out of range",
                                "index", i, "nshells", shellstart_.size());
}


void BasisSet::add_shell_(const BasisShellBase & bshell,
                         const CoordType & xyz)
{
    // have the coordinates been added already?
    auto it = std::find_if(shells_.begin(), shells_.end(),
                           [& xyz](const BasisSetShell & b)
                           { return b.get_coords() == xyz; });

    double * my_xyz = nullptr;

    if(it != shells_.end())
        my_xyz = it->coords_ptr();
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
                      [& bshell](const BasisSetShell & b) { return b.base_compare_(bshell); });


    if(it != shells_.end())
    {
        // equivalent shell already exists! Use the primitives,
        // but copy coords, etc from from bshell
        shells_.push_back(BasisSetShell(bshell,
                                        it->alpha_ptr(), it->all_coefs_ptr(),
                                        my_xyz));
    }
    else
    {
        if(alpha_pos_ + bshell.n_primitives() > max_nalpha_)
            throw BasisSetException("Not enough storage for this shell: too may primitives",
                                               "max", max_nalpha_,
                                               "current", alpha_pos_, "toadd", bshell.n_primitives());  

        if(coef_pos_ + bshell.n_coefficients() > max_ncoef_)
            throw BasisSetException("Not enough storage for this shell: too many coefficients",
                                               "max", max_ncoef_,
                                               "current", coef_pos_, "toadd", bshell.n_coefficients());  

        // copy the primitives from bshell
        double const * const old_alphaptr = bshell.alpha_ptr();
        double const * const old_coefptr = bshell.all_coefs_ptr();
        double * const my_alpha = alpha_base_ptr_ + alpha_pos_;
        double * const my_coef = coef_base_ptr_ + coef_pos_;
        std::copy(old_alphaptr, old_alphaptr+bshell.n_primitives(), my_alpha);
        std::copy(old_coefptr, old_coefptr+bshell.n_coefficients(), my_coef);

        // unique_shells_ stores the index in the shells_ vector
        // the index of this new shell will be shells_.size()
        unique_shells_.push_back(shells_.size());

        // actually add the shell
        shells_.push_back(BasisSetShell(bshell, my_alpha, my_coef, my_xyz));


        // advance these
        alpha_pos_ += bshell.n_primitives();
        coef_pos_ += bshell.n_coefficients();
    }

    // add the starting point
    // (which is the previous starting point plus the previous number of functions)
    // Don't forget, we added the new shell already, so we want the next to last
    // shell in shells_
    if(shellstart_.size() == 0)
        shellstart_.push_back(0);
    else
        shellstart_.push_back(shellstart_.back() + shells_[shells_.size()-2].n_functions());
}


void BasisSet::add_shell_(const BasisSetShell & bshell)
{
    // Adds an existing basis set shell
    // Will copy all data
    add_shell_(bshell, bshell.get_coords());
}


void BasisSet::add_shell(const BasisShellInfo & bshell,
                        const CoordType & xyz)
{
    // Add from a BasisShellInfo
    // Will copy all data
    add_shell_(bshell, xyz);
}

size_t BasisSet::n_shell(void) const noexcept
{
    return shells_.size();
}


size_t BasisSet::n_unique_shell(void) const noexcept
{
    return unique_shells_.size();
}


const BasisSetShell & BasisSet::shell(size_t i) const
{
    psr_assert<BasisSetException>(shells_.size() == shellstart_.size(),
                              "Developer error. nshells != length of shellstart",
                              "nshells", shells_.size(), "nshellstart", shellstart_.size());

    if(i < shells_.size())
        return shells_[i];
    else
        throw BasisSetException("Shell index out of range",
                                "index", i, "nshells", shells_.size());
}

const BasisSetShell & BasisSet::unique_shell(size_t i) const
{
    if(i < unique_shells_.size())
        return shell(unique_shells_.at(i));
    else
        throw BasisSetException("Unique shell index out of range",
                                "index", i, "nshells", unique_shells_.size());
}

BasisShellInfo BasisSet::shell_info(size_t i) const
{
    const BasisSetShell & bss = shell(i);
    BasisShellInfo bsi(bss.get_type(), bss.am(),
                       bss.n_primitives(), bss.n_general_contractions());

    bsi.set_alphas(bss.get_alphas());
    bsi.set_all_coefs(bss.get_all_coefs());
    return bsi;
}

size_t BasisSet::n_primitives(void) const
{
    return std::accumulate(this->begin(), this->end(), static_cast<size_t>(0),
                           [](size_t sum, const BasisSetShell & sh)
                           { return sum + sh.n_primitives(); } );
}

size_t BasisSet::n_coefficients(void) const
{
    return std::accumulate(this->begin(), this->end(), static_cast<size_t>(0),
                           [](size_t sum, const BasisSetShell & sh)
                           { return sum + sh.n_coefficients(); } );
}

size_t BasisSet::max_n_primitives(void) const
{
    if(n_shell())
        return std::max_element(this->begin(), this->end(), 
                                [](const BasisSetShell & lhs, const BasisSetShell & rhs)
                                { return lhs.n_primitives() < rhs.n_primitives(); })->n_primitives();
    else
        return 0; //! \todo exception?
}

int BasisSet::max_am(void) const
{
    int max = 0;

    if(n_shell())
        max = std::max_element(this->begin(), this->end(), 
                               [](const BasisSetShell & lhs, const BasisSetShell & rhs)
                               { return std::abs(lhs.am()) < std::abs(rhs.am()); })->am();
    return std::abs(max);
}

std::set<int> BasisSet::all_am(void) const
{
    std::set<int> res;
    for(const auto & it : *this)
        res.insert(it.am());
    return res;
}

size_t BasisSet::n_functions(void) const
{
    return std::accumulate(this->begin(), this->end(), static_cast<size_t>(0),
                           [](size_t sum, const BasisSetShell & sh)
                           { return sum + sh.n_functions(); } );
}

size_t BasisSet::max_n_functions(void) const
{
    if(n_shell())
        return std::max_element(this->begin(), this->end(), 
                                [](const BasisSetShell & lhs, const BasisSetShell & rhs)
                                { return lhs.n_functions() < rhs.n_functions(); })->n_functions();
    else
        return 0;
}

BasisSet::const_iterator BasisSet::begin(void) const
{
    return shells_.begin();
}

BasisSet::const_iterator BasisSet::end(void) const
{
    return shells_.end();
}

size_t BasisSet::max_property(std::function<size_t(const BasisSetShell &)> func) const
{
    size_t m = 0;
    for(const auto & it : shells_)
        m = std::max(m, func(it));
    return m;
}

BasisSet BasisSet::transform(BasisSet::TransformerFunc Transformer) const
{
    BasisSet bs(n_shell(), n_primitives(), n_coefficients(), 3*n_primitives());
    for(const auto & shell : shells_)
    {
        BasisShellInfo bsi(shell);
        CoordType xyz = shell.get_coords();

        bs.add_shell(Transformer(shell, xyz), xyz);
    }

    return bs.shrink_fit();
}

BasisSet BasisSet::shrink_fit(void) const
{
    using std::swap;

    // Be super safe
    // We check <= , since the *_pos variables represent where we would put
    // the next one. If pos_ == max_n, then it is full. If it is greater, then
    // one was already placed where it wasn't supposed to go...
    psr_assert<BasisSetException>(xyz_pos_ <= max_nxyz_,
                              "Developer error. Too many xyz in basis set",
                              "pos", xyz_pos_, "max", max_nxyz_);
    psr_assert<BasisSetException>(alpha_pos_ <= max_nalpha_,
                              "Developer error. Too many alpha in basis set",
                              "pos", alpha_pos_, "max", max_nalpha_);
    psr_assert<BasisSetException>(coef_pos_ <= max_ncoef_,
                              "Developer error. Too many coefficients in basis set",
                              "pos", coef_pos_, "max", max_ncoef_);


    // We need to know the actual number of stored primitives and coefficients. These
    // are stored in *_pos_ variables.
    // shells_.size() is self explanatory
    BasisSet newbs(shells_.size(), alpha_pos_, coef_pos_, xyz_pos_);

    // Just push through what we have
    // using the private functions. This will check for overflow, etc
    for(const auto & it : shells_)
        newbs.add_shell_(it);

    return newbs;
}


void BasisSet::print(std::ostream & os) const
{
    using namespace pulsar::output;

    size_t nshell = n_shell();

    print_output(os, "Basis set with %? shells\n", nshell);
    print_output(os, "NFunc = %? , max_am = %?\n", n_functions(), max_am());
    print_output(os, "MaxNFunction = %? , max_n_primitives = %?\n", max_n_functions(), max_n_primitives());
    print_debug(os, "Space usage: XYZ: %?/%?  Alpha: %?/%?  Coef %?/%?\n", xyz_pos_, max_nxyz_,
                                                                     alpha_pos_, max_nalpha_,
                                                                     coef_pos_, max_ncoef_);


    for(size_t i = 0; i < nshell; i++)
    {
        const auto & s = shell(i);
        print_output(os, "Shell %?  AM=%?  n_primitives=%? NGen=%?\n", i, s.am(), s.n_primitives(), s.n_general_contractions());
        print_output(os, "Coordinates: % 16.8? % 16.8? % 16.8?\n", s.get_coords()[0], s.get_coords()[1], s.get_coords()[2]);
        for(size_t j = 0; j < s.n_primitives(); ++j)
        {
            print_output(os, "    % 16.8?", s.get_alpha(j));
            for(size_t n = 0; n < s.n_general_contractions(); n++)
                print_output(os, "    % 16.8?", s.get_coef(n, j));
            print_output(os, "\n");
        }
    }
    print_output(os, "\n");
}


} // close namespace system
} // close namespace pulsar
