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


bool BasisShellInfo::operator==(const BasisShellInfo & rhs) const
{
    // this is done manually (rather than "using")
    // prevent implicit comparison between one type and another
    return BasisShellBase::operator==(rhs);
}


} // close namespace system
} // close namespace bpmodule

