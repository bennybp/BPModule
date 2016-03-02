#ifndef BPMODULE_GUARD_BASISSET__BASISSETSHELL_HPP_
#define BPMODULE_GUARD_BASISSET__BASISSETSHELL_HPP_

#include "bpmodule/basisset/BasisShellInfo.hpp"

namespace bpmodule {
namespace basisset {



// To be stored in the BasisSet object
class BasisSetShell : public BasisShellInfo
{
    public:
        typedef std::array<double, 3> CoordType;

        BasisSetShell(const BasisShellInfo & bshell, unsigned long id,
                      unsigned long center, double x, double y, double z)
            : BasisShellInfo(bshell), id_(id), center_(center), xyz_{x,y,z}
        { }

        BasisSetShell(ShellType type, int am, bool cart, unsigned long id,
                      unsigned long center, double x, double y, double z)
            : BasisShellInfo(type, am, cart), id_(id), center_(center), xyz_{x,y,z}
        { }

        BasisSetShell(const BasisShellInfo & bshell, unsigned long id,
                      unsigned long center, const CoordType & xyz)
            : BasisShellInfo(bshell), id_(id), center_(center), xyz_(xyz)
        { }

        BasisSetShell(ShellType type, int am, bool cart, unsigned long id,
                      unsigned long center, const CoordType & xyz)
            : BasisShellInfo(type, am, cart), id_(id), center_(center), xyz_(xyz)
        { }



        // compiler generated ok
        BasisSetShell(const BasisSetShell & rhs)             = default;
        BasisSetShell(BasisSetShell && rhs)                  = default;
        BasisSetShell & operator=(const BasisSetShell & rhs) = default;
        BasisSetShell & operator=(BasisSetShell && rhs)      = default;
        
        unsigned long ID(void) const noexcept { return id_; }
        unsigned long Center(void) const noexcept { return center_; }

        CoordType GetCoords(void) const { return xyz_; }


        bool operator==(const BasisSetShell & rhs) const
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


    private:
        unsigned long id_;          //!< Unique id for this shell
        unsigned long center_;      //!< ID of the center
        CoordType xyz_;             //!< XYZ coordindates of this center
    
};


} // close namespace basisset
} // close namespace bpmodule


#endif
