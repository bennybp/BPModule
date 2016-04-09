/*! \file
 *
 * \brief Standalone class for basis shell information (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bpmodule/exception/Assert.hpp"
#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/system/BasisShellInfo.hpp"
#include "bpmodule/system/NCartesian.hpp"

using bpmodule::exception::Assert;
using bpmodule::exception::BasisSetException;


namespace bpmodule {
namespace system {



BasisShellInfo::BasisShellInfo(ShellType type, int am, bool cart, size_t nprim, size_t ngen,
                               std::vector<double> && alphas, std::vector<double> && allcoefs)
    : BasisShellBase(type, am, cart, nprim, ngen),
      alphas_(std::move(alphas)),
      coefs_(std::move(allcoefs))
{
    if(alphas_.size() != NPrim())
        throw BasisSetException("Incompatible dimensions for alphas", "nprim", NPrim(), "given", alphas_.size());

    if(coefs_.size() != NCoef())
        throw BasisSetException("Incompatible dimensions for coefficients", "nprim", NPrim(), "ngen", NGeneral(), "given", coefs_.size());

    BasisShellBase::SetPtrs_(alphas_.data(), coefs_.data());
}


// Other constructors delegate to the above
BasisShellInfo::BasisShellInfo(ShellType type, int am, bool cart, size_t nprim, size_t ngen)
    : BasisShellInfo(type, am, cart, nprim, ngen,
                     std::move(std::vector<double>(nprim)),
                     std::move(std::vector<double>(nprim*ngen)))
{ }

BasisShellInfo::BasisShellInfo(ShellType type, int am, bool cart, size_t nprim, size_t ngen,
                               const std::vector<double> & alphas, const std::vector<double> & allcoefs)
    : BasisShellInfo(type, am, cart, nprim, ngen,
                     std::move(std::vector<double>(alphas)),
                     std::move(std::vector<double>(allcoefs)))
{ }

BasisShellInfo::BasisShellInfo(const BasisShellBase & rhs)
    : BasisShellInfo(rhs.GetType(), rhs.AM(), rhs.IsCartesian(),
                     rhs.NPrim(), rhs.NGeneral(), rhs.GetAlphas(), rhs.GetAllCoefs())
{ }

BasisShellInfo::BasisShellInfo(const BasisShellInfo & rhs)
    : BasisShellInfo(static_cast<const BasisShellBase &>(rhs))
{ }

BasisShellInfo & BasisShellInfo::operator=(const BasisShellInfo & rhs)
{
    using std::swap;

    if(this == &rhs)
        return *this;

    BasisShellInfo tmp(rhs);
    swap(*this, tmp);
    return *this;
}

bool BasisShellInfo::operator==(const BasisShellInfo & rhs) const
{
    // prevent implicit comparison between one type and another
    return BasisShellBase::BaseCompare(rhs);
}


} // close namespace system
} // close namespace bpmodule

