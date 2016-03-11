#include "bpmodule/system/BasisSetShell.hpp"

namespace bpmodule {
namespace system {


BasisSetShell::BasisSetShell(const BasisSetShell & bshell,
                             double * alphaptr, double * coefptr, double * xyzptr)
    : BasisSetShell(bshell, alphaptr, coefptr, xyzptr,
                    bshell.GetID(), bshell.GetCenter())
{
}


BasisSetShell::BasisSetShell(const BasisShellBase & bshell,
                             double * alphaptr, double * coefptr, double * xyzptr,
                             unsigned long id, 
                             unsigned long center)
    : BasisShellBase(bshell), id_(id), center_(center)
{
    BasisShellBase::SetPtrs_(alphaptr, coefptr); 
    xyz_ = xyzptr;
}


unsigned long BasisSetShell::GetID(void) const noexcept
{
    return id_;
}

unsigned long BasisSetShell::GetCenter(void) const noexcept
{
    return center_;
}

CoordType BasisSetShell::GetCoords(void) const
{
    return CoordType{*xyz_, *(xyz_+1), *(xyz_+2)};
}

const double * BasisSetShell::CoordsPtr(void) const noexcept
{
    return xyz_;
}

bool BasisSetShell::operator==(const BasisSetShell & rhs) const
{
    if(this == &rhs)
        return true;

    // this is done manually (rather than "using")
    // prevent implicit comparison between one type and another
    return (
              id_ == rhs.id_ &&
              center_ == rhs.center_ &&
              std::equal(xyz_, xyz_+3, rhs.xyz_) &&
              BasisShellBase::operator==(rhs)
           );
}


} // close namespace system
} // close namespace bpmodule

