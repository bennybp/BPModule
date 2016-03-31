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

BasisShellInfo::BasisShellInfo(ShellType type, int am, bool cart, int nprim, int ngen)
    : BasisShellBase(type, am, cart, nprim, ngen)
{
    alphas_.resize(NPrim());
    coefs_.resize(NCoef());

    BasisShellBase::SetPtrs_(alphas_.data(), coefs_.data());
}

BasisShellInfo::BasisShellInfo(const BasisShellInfo & rhs)
    : BasisShellBase(rhs),
      alphas_(rhs.alphas_),
      coefs_(rhs.coefs_)
{
    BasisShellBase::SetPtrs_(alphas_.data(), coefs_.data());
}

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

