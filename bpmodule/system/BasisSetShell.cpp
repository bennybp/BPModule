#include "bpmodule/pragma.h"
#include "bpmodule/system/BasisSetShell.hpp"

namespace bpmodule {
namespace system {




BasisSetShell::BasisSetShell(unsigned long id, const BasisShellInfo & bshell,
                             unsigned long center, double x, double y, double z)
    : BasisShellInfo(bshell), id_(id), center_(center), xyz_{x,y,z}
{ }

BasisSetShell::BasisSetShell(unsigned long id, const BasisShellInfo & bshell,
                             unsigned long center, const CoordType & xyz)
    : BasisShellInfo(bshell), id_(id), center_(center), xyz_(xyz)
{ }


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
    PRAGMA_WARNING_PUSH
    PRAGMA_WARNING_IGNORE_FP_EQUALITY

    return (
               static_cast<const BasisShellInfo>(*this) == static_cast<const BasisShellInfo>(rhs) &&
               id_ == rhs.id_ &&
               xyz_ == rhs.xyz_
           );

    PRAGMA_WARNING_POP
}


} // close namespace system
} // close namespace bpmodule

