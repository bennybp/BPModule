#ifndef BPMODULE_GUARD_SYSTEM__BASISSETSHELL_HPP_
#define BPMODULE_GUARD_SYSTEM__BASISSETSHELL_HPP_

#include "bpmodule/system/BasisShellBase.hpp"
#include "bpmodule/system/CoordType.hpp"

namespace bpmodule {
namespace system {


// To be stored in the BasisSet object
class BasisSetShell : public BasisShellBase
{
    public:
        BasisSetShell(const BasisSetShell & bshell,
                     double * alphaptr, double * coefptr, double * xyzptr);

        BasisSetShell(const BasisShellBase & bshell,
                      double * alphaptr, double * coefptr, double * xyzptr,
                      unsigned long id, unsigned long center);


        // compiler generated ok
        BasisSetShell(const BasisSetShell & rhs)             = default;
        BasisSetShell(BasisSetShell && rhs)                  = default;
        BasisSetShell & operator=(const BasisSetShell & rhs) = default;
        BasisSetShell & operator=(BasisSetShell && rhs)      = default;
        
        unsigned long GetID(void) const noexcept;
        unsigned long GetCenter(void) const noexcept;
        CoordType GetCoords(void) const;


        ///@{ Raw, unsafe, fast

        const double * CoordsPtr(void) const noexcept;

        ///@}

        bool operator==(const BasisSetShell & rhs) const;


    private:
        unsigned long id_;          //!< Unique id for this shell
        unsigned long center_;      //!< ID of the center
        double * xyz_;              //!< XYZ coordindates of this center
    
};


} // close namespace system
} // close namespace bpmodule


#endif
