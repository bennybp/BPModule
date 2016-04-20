/*! \file
 *
 * \brief Basis shell from a basis set object (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "pulsar/system/BasisSetShell.hpp"

namespace pulsar{
namespace system {


BasisSetShell::BasisSetShell(const BasisSetShell & bshell,
                             double * alphaptr, double * coefptr, double * xyzptr) ASSERTIONS_ONLY
    : BasisSetShell(bshell, alphaptr, coefptr, xyzptr,
                    bshell.GetID(), bshell.GetCenter())
{
}


BasisSetShell::BasisSetShell(const BasisShellBase & bshell,
                             double * alphaptr, double * coefptr, double * xyzptr,
                             ID_t id, 
                             ID_t center) ASSERTIONS_ONLY
    : BasisShellBase(bshell), id_(id), center_(center)
{
    SetPtrs_(alphaptr, coefptr, xyzptr);
}

void BasisSetShell::SetPtrs_(double * alphaptr, double * coefptr, double * xyzptr)
{
    BasisShellBase::SetPtrs_(alphaptr, coefptr); 
    xyz_ = xyzptr;
}


ID_t BasisSetShell::GetID(void) const noexcept
{
    return id_;
}

ID_t BasisSetShell::GetCenter(void) const noexcept
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

double * BasisSetShell::CoordsPtr(void) noexcept
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
              BasisShellBase::BaseCompare(rhs)
           );
}

bool BasisSetShell::operator!=(const BasisSetShell & rhs) const
{
    return !((*this) == rhs);
}

} // close namespace system
} // close namespace pulsar

