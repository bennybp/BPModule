/*\file
 *
 * \brief The system class (source)
 */

#include "bpmodule/pragma.h"
#include "bpmodule/system/System.hpp"
#include "bpmodule/system/BasisSet.hpp"
#include "bpmodule/system/AtomicInfo.hpp"


using bpmodule::exception::SystemException;


// Instantiate the MathSet, etc, templates
template class bpmodule::math::Universe<bpmodule::system::Atom>;
template class bpmodule::math::MathSet<bpmodule::system::Atom>;


namespace bpmodule {
namespace system {

void System::SetDefaults_(void)
{
    charge_=GetSumCharge();
    nelectrons_=GetSumNElectrons();

    //! \todo default multiplicity
    multiplicity_=1.0;
}

System::System(const AtomSet & atoms)
    :atoms_(atoms)
{
    SetDefaults_();
}

System::System(std::shared_ptr<const AtomSetUniverse> universe,bool fill)
    :atoms_(universe,fill)
{
    SetDefaults_();
}

System::System(const AtomSetUniverse& universe,bool fill):
    System(std::shared_ptr<AtomSetUniverse>(new AtomSetUniverse(universe)),fill)
{
}


size_t System::NAtoms(void) const
{
    return size();
}

size_t System::size(void) const
{
    return atoms_.size();
}

double System::GetSumCharge(void) const
{
    return std::accumulate(this->begin(),this->end(),static_cast<double>(0.0),
                           [](double sum,const Atom&a)
    {
        return sum+a.GetCharge();
    });
}

double System::GetSumNElectrons(void) const
{
    return std::accumulate(this->begin(),this->end(),static_cast<double>(0.0),
                           [](double sum,const Atom&a)
    {
        return sum+a.GetNElectrons();
    });
}

double System::GetCharge(void) const
{
    return charge_;
}

void System::SetCharge(double charge)
{
    charge_=charge;
}

double System::GetNElectrons(void) const
{
    return nelectrons_;
}

void System::SetNElectrons(double nelectrons)
{
    nelectrons_=nelectrons;
}

double System::GetMultiplicity(void) const
{
    return multiplicity_;
}

void System::SetMultiplicity(double m)
{
    multiplicity_=m;
}

bool System::HasAtom(const Atom& AnAtom)const
{
    return atoms_.ContainsElement(AnAtom);
}

System& System::Insert(const Atom & atom)
{
    atoms_.Insert(atom);
    return *this;
}

System & System::UnionAssign(const System& RHS)
{
    atoms_.UnionAssign(RHS.atoms_);
    return *this;
}

System System::Union(const System& RHS) const
{
    return System(*this).UnionAssign(RHS);
}

System& System::IntersectionAssign(const System& RHS)
{
    atoms_.IntersectionAssign(RHS.atoms_);
    return *this;
}

System System::Intersection(const System& RHS) const
{
    return System(*this).IntersectionAssign(RHS);
}

System& System::DifferenceAssign(const System& RHS)
{
    atoms_.DifferenceAssign(RHS.atoms_);
    return *this;
}

System System::Difference(const System& RHS) const
{
    return System(*this).DifferenceAssign(RHS);
}

System System::Complement(void) const
{
    return System(atoms_.Complement());
}

System System::Partition(System::SelectorFunc selector) const
{
    return System(atoms_.Partition(selector));
}
System System::Transform(System::TransformerFunc transformer) const
{
    return System(atoms_.Transform(transformer));
}

//! \todo will only be true if the universes are the same
bool System::operator==(const System& RHS)const
{
    PRAGMA_WARNING_PUSH
    PRAGMA_WARNING_IGNORE_FP_EQUALITY
    return(charge_ == RHS.charge_ &&
           multiplicity_ == RHS.multiplicity_ &&
           nelectrons_ == RHS.nelectrons_ &&
           atoms_ == RHS.atoms_
          );
    PRAGMA_WARNING_POP
}

bool System::IsProperSubsetOf(const System& RHS)const
{
    return atoms_.IsProperSubsetOf(atoms_);
}

bool System::IsSubsetOf(const System& RHS)const
{
    return atoms_.IsSubsetOf(atoms_);
}

bool System::IsProperSupersetOf(const System& RHS)const
{
    return atoms_.IsProperSupersetOf(atoms_);
}

bool System::IsSupersetOf(const System& RHS)const
{
    return atoms_.IsSupersetOf(atoms_);
}


System& System::operator+=(const System& rhs) { return UnionAssign(rhs); }
System System::operator+(const System& rhs)const { return Union(rhs); }
System& System::operator/=(const System& rhs) { return IntersectionAssign(rhs); }
System System::operator/(const System& rhs)const { return Intersection(rhs); }
System& System::operator-=(const System& rhs) { return DifferenceAssign(rhs); }
System System::operator-(const System& rhs)const { return Difference(rhs); }
bool System::operator<=(const System& rhs)const { return IsProperSubsetOf(rhs); }
bool System::operator<(const System& rhs)const { return IsSubsetOf(rhs); }
bool System::operator>=(const System& rhs)const { return IsProperSupersetOf(rhs); }
bool System::operator>(const System& rhs)const { return IsSupersetOf(rhs); }




math::Point System::CenterOfMass(void) const
{
    return math::WeightedPointsCenter<math::Point>(*this,&Atom::GetMass);
}

math::Point System::CenterOfNuclearCharge(void) const
{
    return math::WeightedPointsCenter<math::Point>(*this,&Atom::GetZ);
}

bool System::HasBasisSet(const std::string & basislabel) const
{
    for(const auto & atom:*this)
        if(atom.HasShells(basislabel))
            return true;

    return false;

}

BasisSet System::GetBasisSet(const std::string & basislabel) const
{
    // get the number of primitives and storage needed in basis set
    size_t nprim=0;
    size_t ncoef=0;
    size_t nshell=0;

    for(const auto & atom:*this)
    {
        nshell+=atom.NShell(basislabel);

        for(const auto & bshell:atom.GetShells(basislabel))
        {
            nprim+=bshell.NPrim();
            ncoef+=bshell.NCoef();
        }
    }

    BasisSet bs(nshell,nprim,ncoef);

    // now add them
    for(const auto & atom:*this)
        for(const auto & bshell:atom.GetShells(basislabel))
            bs.AddShell(bshell,atom.GetIdx(),atom.GetCoords());

    // shrink the basis set
    return bs.ShrinkFit();
}

void System::Print(std::ostream & os) const
{
    for(const auto & it:*this)
        it.Print(os);
}

std::string System::ToString()const
{
    std::stringstream ss;
    Print(ss);
    return ss.str();
}

///Returns the distance between each pair of atoms in sys
std::vector<double> GetDistance(const System& sys)
{
    size_t NAtoms=sys.NAtoms();
    std::vector<double> DM(NAtoms*NAtoms,0.0);
    //Loop over the lower triangle of the matrix, setting upper as well
    size_t I=0;

    for(const Atom& atomI:sys)
    {
        size_t J=0;

        for(const Atom& atomJ:sys)
        {
            if(atomJ==atomI)break;

            DM[I*NAtoms+J]=atomI.Distance(atomJ);
            DM[J*NAtoms+I]=DM[I*NAtoms+J];
            ++J;
        }

        ++I;
    }

    return DM;
}

typedef std::unordered_map<Atom,std::unordered_set<Atom>> Conn_t;
Conn_t GetConns(const System& sys,double Tolerance)
{
    Conn_t Conns;

    for(const Atom& AtomI:sys)Conns[AtomI]=std::unordered_set<Atom>();

    std::vector<double> Dist=GetDistance(sys);
    size_t I=0,NAtoms=sys.NAtoms();

    for(const Atom& AtomI:sys)
    {
        size_t J=0;
        double Irad=AtomI.GetCovRadius();

        for(const Atom& AtomJ:sys)
        {
            if(AtomI==AtomJ)break;

            double Jrad=AtomJ.GetCovRadius();

            if(Dist[I*NAtoms+J]<Tolerance*(Irad+Jrad))
            {
                Conns[AtomI].insert(AtomJ);
                Conns[AtomJ].insert(AtomI);
            }

            ++J;
        }

        ++I;
    }

    return Conns;
}


} // close namespace system
} // close namespace bpmodule

