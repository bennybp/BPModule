#ifndef BPMODULE_GUARD_BASISSET__BASISSETSHELL_HPP_
#define BPMODULE_GUARD_BASISSET__BASISSETSHELL_HPP_

#include "bpmodule/basisset/BasisShell.hpp"

namespace bpmodule {
namespace basisset {


// for friend class
class BasisSet;


// To be stored in the BasisSet object
class BasisSetShell : public BasisShell
{
    public:
        typedef std::array<double, 3> CoordType;


        //! \todo construct from python list
        BasisSetShell(const BasisShell & bshell, unsigned long id,
                      unsigned long center, double x, double y, double z)
            : BasisShell(bshell), id_(id), center_(center), xyz_{x,y,z}
        { }

        BasisSetShell(ShellType type, int am, bool cart, unsigned long id,
                      unsigned long center, double x, double y, double z)
            : BasisShell(type, am, cart), id_(id), center_(center), xyz_{x,y,z}
        { }

        BasisSetShell(const BasisShell & bshell, unsigned long id,
                      unsigned long center, const CoordType & xyz)
            : BasisShell(bshell), id_(id), center_(center), xyz_(xyz)
        { }

        BasisSetShell(ShellType type, int am, bool cart, unsigned long id,
                      unsigned long center, const CoordType & xyz)
            : BasisShell(type, am, cart), id_(id), center_(center), xyz_(xyz)
        { }

        // compiler generated ok
        BasisSetShell(const BasisSetShell & rhs)             = default;
        BasisSetShell(BasisSetShell && rhs)                  = default;
        BasisSetShell & operator=(const BasisSetShell & rhs) = default;
        BasisSetShell & operator=(BasisSetShell && rhs)      = default;
        
        unsigned long ID(void) const noexcept { return id_; }
        unsigned long Center(void) const noexcept { return center_; }

        CoordType Coordinates(void) const { return xyz_; }


        bool operator==(const BasisSetShell & rhs) const
        {
            PRAGMA_WARNING_PUSH
            PRAGMA_WARNING_IGNORE_FP_EQUALITY

            return (
                    static_cast<const BasisShell>(*this) == static_cast<const BasisShell>(rhs) &&
                    id_ == rhs.id_ &&
                    xyz_ == rhs.xyz_
                   );

            PRAGMA_WARNING_POP
        }


    protected:
        friend class BasisSet;

        void SetID(unsigned long id) { id_ = id; }


    private:
        unsigned long id_;          //!< Unique id for this shell
        unsigned long center_;      //!< ID of the center
        CoordType xyz_;             //!< XYZ coordindates of this center
    
};


} // close namespace basisset
} // close namespace bpmodule


#endif
