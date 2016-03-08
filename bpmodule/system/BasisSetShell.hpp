#ifndef BPMODULE_GUARD_SYSTEM__BASISSETSHELL_HPP_
#define BPMODULE_GUARD_SYSTEM__BASISSETSHELL_HPP_

#include <array>
#include "bpmodule/system/BasisShellBase.hpp"

namespace bpmodule {
namespace system {


// To be stored in the BasisSet object
class BasisSetShell : public BasisShellBase
{
    public:
        typedef std::array<double, 3> CoordType;

        BasisSetShell(const BasisSetShell & bshell, double * alphaptr, double * coefptr);

        BasisSetShell(const BasisShellBase & bshell,
                      double * alphaptr, double * coefptr,
                      unsigned long id,
                      unsigned long center, const CoordType & xyz);


        // compiler generated ok
        BasisSetShell(const BasisSetShell & rhs)             = default;
        BasisSetShell(BasisSetShell && rhs)                  = default;
        BasisSetShell & operator=(const BasisSetShell & rhs) = default;
        BasisSetShell & operator=(BasisSetShell && rhs)      = default;
        
        unsigned long GetID(void) const noexcept;
        unsigned long GetCenter(void) const noexcept;

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
