#include "bpmodule/pragma.h"
#include "bpmodule/system/BasisSetShell.hpp"

namespace bpmodule {
namespace system {


BasisSetShell::BasisSetShell(const BasisSetShell & bshell,
                             double * alphaptr, double * coefptr)
    : BasisSetShell(bshell.ID(), alphaptr, coefptr, bshell, bshell.Center(), bshell.GetCoords())
{ }


BasisSetShell::BasisSetShell(unsigned long id, double * alphaptr, double * coefptr,
                             const BasisShellBase & bshell,
                             unsigned long center, const CoordType & xyz)
    : BasisShellBase(bshell.GetType(), bshell.AM(), bshell.IsCartesian(),
                     bshell.NPrim(), bshell.NGeneral()),
      id_(id), center_(center), xyz_(xyz)
{
    BasisShellBase::SetPtrs_(alphaptr, coefptr); 

    //! \todo inefficient copy. Consider rvalue refs in base class
    BasisShellBase::SetAlphas(bshell.GetAlphas());
    BasisShellBase::SetAllCoefs(bshell.GetAllCoefs());
}


unsigned long BasisSetShell::ID(void) const noexcept
{
    return id_;
}
unsigned long BasisSetShell::Center(void) const noexcept
{
    return center_;
}

BasisSetShell::CoordType BasisSetShell::GetCoords(void) const
{
    return xyz_;
}


bool BasisSetShell::operator==(const BasisSetShell & rhs) const
{
    // this is done manually (rather than "using")
    // prevent implicit comparison between one type and another
    return static_cast<BasisShellBase>(*this) == static_cast<BasisShellBase>(rhs);
}


} // close namespace system
} // close namespace bpmodule

