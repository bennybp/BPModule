#include "bpmodule/system/Atom.hpp"
#include "bpmodule/system/AtomicInfo.hpp"
#include "bpmodule/math/Cast.hpp"

namespace bpmodule {

//    typedef std::set<const util::Enumeration<system::AtomProperty>*> Set_t;
//    Set_t util::Enumeration<system::AtomProperty>::Enums_=Set_t();
namespace system {

double Atom::GetMass(void) const
{
    return AtomicMassFromZ(GetZ());
}

double Atom::GetIsotopeMass(void) const
{
    return IsotopeMassFromZ(GetZ(), GetIsonum());
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

    return GetZ() == rhs.GetZ() &&
           GetIsonum() == rhs.GetIsonum() &&
           GetCharge() == rhs.GetCharge() &&
           GetMultiplicity() == rhs.GetMultiplicity() &&
           GetNElectrons() == rhs.GetNElectrons() &&
           static_cast<math::Point>(*this) == static_cast<math::Point>(rhs)
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


Atom CreateAtom(size_t idx, Atom::CoordType xyz, int Z, const Atom::TagsType & tags)
{
    int isonum = MostCommonIsotopeFromZ(Z);
    return CreateAtom(idx, xyz, Z, isonum, tags);

}

Atom CreateAtom(size_t idx, Atom::CoordType xyz, int Z, int isonum, const Atom::TagsType & tags)
{
    return Atom(idx,
                xyz,
                Z,
                isonum,
                0,  //! \todo default charge
                math::numeric_cast<double>(AtomicMultiplicityFromZ(Z)),
                math::numeric_cast<double>(Z),  //! 0 charge, nelectrons = Z
                tags);
}

Atom CreateAtom(size_t idx, double x, double y, double z, int Z, const Atom::TagsType & tags)
{
    return CreateAtom(idx, {x,y,z}, Z, tags);
}

Atom CreateAtom(size_t idx, double x, double y, double z, int Z, int isonum, const Atom::TagsType & tags)
{
    return CreateAtom(idx, {x,y,z}, Z, isonum, tags);
}


/*
size_t DefaultMult(size_t Z){
    switch(Z){
        case 0 :{return 1;}
        case 1:{return 2;}
        case 2:{return 1;}
        case 3:{return 2;}
        case 4:{return 1;}
        case 5:{return 2;}
        case 6:{return 3;}
        case 7:{return 4;}
        case 8:{return 3;}
        case 9:{return 2;}
        case 10:{return 1;}
        default:{return 0;}
    }
}    




const AtomProperty AtomProperty::Mass("MASS");
const AtomProperty AtomProperty::Z("ATOMIC NUMBER");
const AtomProperty AtomProperty::Charge("CHARGE");
const AtomProperty AtomProperty::Mult("MULTIPLICITY");
const AtomProperty AtomProperty::NElec("NUMBER OF ELECTRONS");


std::string Atom::Symbol() const{
    return AtomicSymFromZ((int)Z());
}

Atom::Atom(size_t ZIn,double x,double y,double z):
    Base_t(x,y,z){
        AddWeight(AtomProperty::Z,(double)ZIn);
        SetMass(AtomicMassFromZ((int)ZIn));
        SetChargeAndMult(0.0,DefaultMult((int)ZIn));
        SetNElec((double)ZIn);
}

bool Atom::operator==(const Atom& RHS)const{
            if(Z()!=RHS.Z())return false;
            if((*this)[0]!=RHS[0]||
               (*this)[1]!=RHS[1]||
               (*this)[2]!=RHS[2])return false;
            if(NElec()!=RHS.NElec()||Charge()!=RHS.NElec())return false;
            if(Mass()!=RHS.Mass())return false;
            return true;
}

*/
    

/*
double & Atom::Mass(void)
{
    return Point_t.Weight(AtomWeights::Mass);
}


const double & Atom::Mass(void) const
{
    return Point_t.Weight(AtomWeights::Mass);
}


*/

} // close namespace system
} // close namespace bpmodule

