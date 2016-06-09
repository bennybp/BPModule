/*\file
 *
 * \brief Atoms and atomic centers (source)
*/


#include "pulsar/system/Atom.hpp"
#include "pulsar/system/AtomicInfo.hpp"
#include "pulsar/math/Cast.hpp"
#include "pulsar/output/Output.hpp"

#include "bphash/types/string.hpp"
#include "bphash/types/set.hpp"
#include "bphash/types/map.hpp"
#include "bphash/types/vector.hpp"

using namespace pulsar::output;

namespace pulsar{

//    typedef std::set<const util::Enumeration<AtomProperty>*> Set_t;
//    Set_t util::Enumeration<AtomProperty>::Enums_=Set_t();
namespace system {


Atom::Atom(CoordType xyz, int Z, int isonum, double mass,
           double isotopemass, double charge, double multiplicity,
           double nelectrons,double covradius, double vdwradius)
{
    // we do it this way in case we change where the info is stored
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

bphash::HashValue Atom::MyHash(void) const
{
    return bphash::MakeHash(bphash::HashType::Hash128, *this);
} 

void Atom::hash(bphash::Hasher & h) const
{
    h(static_cast<const math::Point &>(*this),
           Z_, isonum_,
           mass_, isotopemass_,
           charge_, multiplicity_,
           nelectrons_, covradius_,
           vdwradius_, bshells_);
}

void Atom::BasisInfo_::hash(bphash::Hasher & h) const
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

Atom CreateAtom(CoordType xyz, int Z)
{
    int isonum = MostCommonIsotopeFromZ(Z);
    return CreateAtom(xyz, Z, isonum);

}

Atom CreateAtom(CoordType xyz, int Z, int isonum)
{
    return Atom(xyz,
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

Atom CreateAtom(double x, double y, double z, int Z)
{
    return CreateAtom({x,y,z}, Z);
}

Atom CreateAtom(double x, double y, double z, int Z, int isonum)
{
    return CreateAtom({x,y,z}, Z, isonum);
}

Atom MakeGhost(const Atom& AtomI){
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

Atom MakeDummy(CoordType xyz){
      return CreateAtom(xyz,-1);
}

Atom MakeCharge(double charge,CoordType xyz){
    return CreateAtom(xyz,-2);
}

bool IsGhost(const Atom& AtomI){
    Atom AtomJ=MakeGhost(AtomI);
    return AtomJ==AtomI;
}

bool IsDummy(const Atom& AtomI){
    Atom AtomJ=MakeDummy(AtomI);
    return AtomJ==AtomI;
}

bool IsCharge(const Atom& AtomI){
    Atom AtomJ=MakeCharge(AtomI);
    return AtomJ==AtomI;
}

} // close namespace system
} // close namespace pulsar

