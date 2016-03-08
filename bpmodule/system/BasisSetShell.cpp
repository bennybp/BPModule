#include "bpmodule/pragma.h"
#include "bpmodule/system/BasisSetShell.hpp"

namespace bpmodule {
namespace system {


BasisSetShell::BasisSetShell(const BasisSetShell & bshell,
                             double * alphaptr, double * coefptr)
    : BasisSetShell(bshell, alphaptr, coefptr,
                    bshell.GetID(), bshell.GetCenter(), bshell.GetCoords())
{
}


BasisSetShell::BasisSetShell(const BasisShellBase & bshell,
                             double * alphaptr, double * coefptr,
                             unsigned long id, 
                             unsigned long center, const CoordType & xyz)
    : BasisShellBase(bshell),
      id_(id), center_(center), xyz_(xyz)
{
    BasisShellBase::SetPtrs_(alphaptr, coefptr); 

    double const * const src_alpha = bshell.AlphaPtr();
    double const * const src_coef = bshell.AllCoefsPtr();
    std::copy(src_alpha, src_alpha + NPrim(), alphaptr); 
    std::copy(src_coef, src_coef + NCoef(), coefptr); 
}


unsigned long BasisSetShell::GetID(void) const noexcept
{
    return id_;
}

unsigned long BasisSetShell::GetCenter(void) const noexcept
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
    return BasisShellBase::operator==(rhs);
}


} // close namespace system
} // close namespace bpmodule

