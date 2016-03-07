#ifndef BPMODULE_GUARD_BASISSET__BASISSETSHELL_HPP_
#define BPMODULE_GUARD_BASISSET__BASISSETSHELL_HPP_

#include "bpmodule/system/BasisShellInfo.hpp"

namespace bpmodule {
namespace system {


// To be stored in the BasisSet object
class BasisSetShell : public BasisShellInfo
{
    public:
        typedef std::array<double, 3> CoordType;

        BasisSetShell(unsigned long id, const BasisShellInfo & bshell,
                      unsigned long center, double x, double y, double z);

        BasisSetShell(unsigned long id, const BasisShellInfo & bshell,
                      unsigned long center, const CoordType & xyz);


        // compiler generated ok
        BasisSetShell(const BasisSetShell & rhs)             = default;
        BasisSetShell(BasisSetShell && rhs)                  = default;
        BasisSetShell & operator=(const BasisSetShell & rhs) = default;
        BasisSetShell & operator=(BasisSetShell && rhs)      = default;
        
        unsigned long ID(void) const noexcept;
        unsigned long Center(void) const noexcept;

        CoordType GetCoords(void) const;


        bool operator==(const BasisSetShell & rhs) const;


    private:
        unsigned long id_;          //!< Unique id for this shell
        unsigned long center_;      //!< ID of the center
        CoordType xyz_;             //!< XYZ coordindates of this center
    
};


} // close namespace system
} // close namespace bpmodule


#endif
