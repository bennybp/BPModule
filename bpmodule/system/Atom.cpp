/*\file
 *
 * \brief Atoms and atomic centers (source)
*/


#include "bpmodule/system/Atom.hpp"
#include "bpmodule/system/AtomicInfo.hpp"
#include "bpmodule/math/Cast.hpp"

namespace bpmodule {

//    typedef std::set<const util::Enumeration<system::AtomProperty>*> Set_t;
//    Set_t util::Enumeration<system::AtomProperty>::Enums_=Set_t();
namespace system {


std::string Atom::GetName(void) const
{
    return AtomicNameFromZ(GetZ());

}

std::string Atom::GetSymbol(void) const
{
    return AtomicSymFromZ(GetZ());
}


bool Atom::operator==(const Atom & rhs) const
{
    PRAGMA_WARNING_PUSH
    PRAGMA_WARNING_IGNORE_FP_EQUALITY


    // order by the parts that are most likely to be different, since
    //   this should short-circuit on the first false comparison
    return GetZ() == rhs.GetZ() &&
           static_cast<const math::Point>(*this) == static_cast<const math::Point>(rhs) &&
           GetAllShells() == rhs.GetAllShells() &&
           GetIsonum() == rhs.GetIsonum() &&
           GetMass() == rhs.GetMass() &&
           GetIsotopeMass() == rhs.GetIsotopeMass() &&
           GetCharge() == rhs.GetCharge() &&
           GetMultiplicity() == rhs.GetMultiplicity() &&
           GetNElectrons() == rhs.GetNElectrons()
           ;
             
    PRAGMA_WARNING_POP
}

bool Atom::operator!=(const Atom & rhs) const
{
    return !((*this) == rhs);
}

std::ostream& operator<<(std::ostream& os,const Atom& A)
{
    os<<A.GetSymbol()<<" "<<A[0]<<" "<<A[1]<<" "<<A[2];
    return os;
}


////////////////////////////////
// Free functions
////////////////////////////////

Atom CreateAtom(size_t idx, Atom::CoordType xyz, int Z)
{
    int isonum = MostCommonIsotopeFromZ(Z);
    return CreateAtom(idx, xyz, Z, isonum);

}

Atom CreateAtom(size_t idx, Atom::CoordType xyz, int Z, int isonum)
{
    return Atom(idx,
                xyz,
                Z,
                isonum,
                0,  //! \todo default charge
                math::numeric_cast<double>(AtomicMultiplicityFromZ(Z)),
                math::numeric_cast<double>(Z));  //! 0 charge, nelectrons = Z
}

Atom CreateAtom(size_t idx, double x, double y, double z, int Z)
{
    return CreateAtom(idx, {x,y,z}, Z);
}

Atom CreateAtom(size_t idx, double x, double y, double z, int Z, int isonum)
{
    return CreateAtom(idx, {x,y,z}, Z, isonum);
}


} // close namespace system
} // close namespace bpmodule

