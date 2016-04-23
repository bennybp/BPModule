/*\file
 *
 * \brief Atoms and atomic centers (source)
*/


#include "pulsar/system/Atom.hpp"
#include "pulsar/system/AtomicInfo.hpp"
#include "pulsar/math/Cast.hpp"
#include "pulsar/output/Output.hpp"

#include "pulsar/util/bphash/types/string.hpp"
#include "pulsar/util/bphash/types/set.hpp"
#include "pulsar/util/bphash/types/map.hpp"
#include "pulsar/util/bphash/types/vector.hpp"

using namespace pulsar::output;

namespace pulsar{

//    typedef std::set<const util::Enumeration<AtomProperty>*> Set_t;
//    Set_t util::Enumeration<AtomProperty>::Enums_=Set_t();
namespace system {


Atom::Atom(size_t idx, CoordType xyz, int Z, int isonum, double mass,
           double isotopemass, double charge, double multiplicity,
           double nelectrons,double covradius, double vdwradius)
{
    // we do it this way in case we change where the info is stored
    idx_ = idx;
    SetCoords(xyz);
    SetZ(Z);
    SetIsonum(isonum);
    SetMass(mass);
    SetIsotopeMass(isotopemass);
    SetCharge(charge);
    SetMultiplicity(multiplicity);
    SetNElectrons(nelectrons);
    SetCovRadius(covradius);
    SetVDWRadius(vdwradius);
}

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
           //bshells_ == rhs.bshells_ &&
           GetIsonum() == rhs.GetIsonum() &&
           GetMass() == rhs.GetMass() &&
           GetIsotopeMass() == rhs.GetIsotopeMass() &&
           GetCharge() == rhs.GetCharge() &&
           GetMultiplicity() == rhs.GetMultiplicity() &&
           GetNElectrons() == rhs.GetNElectrons() &&
           GetCovRadius() == rhs.GetCovRadius() &&
           GetVDWRadius() == rhs.GetVDWRadius()
           ;
             
    PRAGMA_WARNING_POP
}

bool Atom::operator!=(const Atom & rhs) const
{
    return !((*this) == rhs);
}


void Atom::Print(std::ostream & os) const
{
    Output(os, "%-5?    %16.8?  %16.8?  %16.8?\n",
           GetSymbol(), GetCoords()[0], GetCoords()[1], GetCoords()[2]);
}

bool Atom::BasisInfo_::operator==(const BasisInfo_ & rhs) const
{
    // note we have to check sizes first for std::equal 
    return (
            shells == rhs.shells &&
            description.size() == rhs.description.size() &&
            std::equal(description.begin(), description.end(), rhs.description.begin())
           );
}

util::Hash Atom::MyHash(void) const
{
    return util::MakeHash(*this);
} 

void Atom::hash(util::Hasher & h) const
{
    h(static_cast<const math::Point &>(*this),
           idx_, Z_, isonum_,
           mass_, isotopemass_,
           charge_, multiplicity_,
           nelectrons_, covradius_,
           vdwradius_, bshells_);
}

void Atom::BasisInfo_::hash(util::Hasher & h) const
{
    h(description, shells);
}


std::ostream& operator<<(std::ostream& os,const Atom& A)
{
    A.Print(os);
    return os;
}


////////////////////////////////
// Free functions
////////////////////////////////

Atom CreateAtom(size_t idx, CoordType xyz, int Z)
{
    int isonum = MostCommonIsotopeFromZ(Z);
    return CreateAtom(idx, xyz, Z, isonum);

}

Atom CreateAtom(size_t idx, CoordType xyz, int Z, int isonum)
{
    return Atom(idx,
                xyz,
                Z,
                isonum,
                AtomicMassFromZ(Z),
                IsotopeMassFromZ(Z, isonum),
                0,  //! \todo default charge
                math::numeric_cast<double>(AtomicMultiplicityFromZ(Z)),
                math::numeric_cast<double>(Z),
                CovRadiusFromZ(Z),
                VDWRadiusFromZ(Z)
                );  //! 0 charge, nelectrons = Z
}

Atom CreateAtom(size_t idx, double x, double y, double z, int Z)
{
    return CreateAtom(idx, {x,y,z}, Z);
}

Atom CreateAtom(size_t idx, double x, double y, double z, int Z, int isonum)
{
    return CreateAtom(idx, {x,y,z}, Z, isonum);
}

Atom MakeGhost(size_t idx,const Atom& AtomI){
  Atom AtomJ(AtomI);//copy basis
  AtomJ.SetZ(0);
  AtomJ.SetIsonum(0);
  AtomJ.SetMass(0.0);
  AtomJ.SetIsotopeMass(0.0);
  AtomJ.SetCharge(0.0);
  AtomJ.SetMultiplicity(0.0);
  AtomJ.SetNElectrons(0.0);
  AtomJ.SetCovRadius(0.0);
  return AtomJ;
}

Atom MakeDummy(size_t idx,CoordType xyz){
      return CreateAtom(idx,xyz,-1);
}

Atom MakeCharge(size_t idx,double charge,CoordType xyz){
    return CreateAtom(idx,xyz,-2);
}

bool IsGhost(const Atom& AtomI){
    Atom AtomJ=MakeGhost(0,AtomI);
    return AtomJ==AtomI;
}

bool IsDummy(const Atom& AtomI){
    Atom AtomJ=MakeDummy(0,AtomI);
    return AtomJ==AtomI;
}

bool IsCharge(const Atom& AtomI){
    Atom AtomJ=MakeCharge(0,AtomI);
    return AtomJ==AtomI;
}

} // close namespace system
} // close namespace pulsar

