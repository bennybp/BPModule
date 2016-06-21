/*\file
 *
 * \brief The system class (source)
 */

#include "pulsar/pragma.h"
#include "pulsar/system/System.hpp"
#include "pulsar/system/BasisSet.hpp"
#include "pulsar/system/AtomicInfo.hpp"
#include "pulsar/constants.h"
#include "pulsar/output/GlobalOutput.hpp"
#include "pulsar/math/PointManipulation.hpp"

using pulsar::output::print_global_debug;
using pulsar::exception::SystemException;


// Instantiate the MathSet, etc, templates
template class pulsar::math::Universe<pulsar::system::Atom>;
template class pulsar::math::MathSet<pulsar::system::Atom>;


namespace pulsar{
namespace system {

void System::SetDefaults_(void)
{
    charge_=get_sum_charge();
    nelectrons_=get_sum_n_electrons();

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


size_t System::size(void) const
{
    return atoms_.size();
}

double System::get_sum_charge(void) const
{
    return std::accumulate(this->begin(),this->end(),static_cast<double>(0.0),
                           [](double sum,const Atom & a)
                           { return sum+a.charge; });
}

double System::get_sum_n_electrons(void) const
{
    return std::accumulate(this->begin(),this->end(),static_cast<double>(0.0),
                           [](double sum,const Atom & a)
                           { return sum+a.nelectrons; });
}

double System::get_charge(void) const
{
    return charge_;
}

void System::set_charge(double charge)
{
    charge_=charge;
}

double System::get_n_electrons(void) const
{
    return nelectrons_;
}

void System::set_n_electrons(double nelectrons)
{
    nelectrons_=nelectrons;
}

double System::get_multiplicity(void) const
{
    return multiplicity_;
}

void System::set_multiplicity(double m)
{
    multiplicity_=m;
}

Space System::get_space(void) const
{
    return Space_;
}

void System::set_space(const Space& S)
{
    Space_=S;
}


bool System::count(const Atom& AnAtom)const
{
    return atoms_.count(AnAtom);
}

System& System::insert(const Atom & atom)
{
    atoms_.insert(atom);
    SetDefaults_();
    return *this;
}

System& System::insert(Atom && atom)
{
    atoms_.insert(std::move(atom));
    SetDefaults_();
    return *this;
}

System & System::union_assign(const System& RHS)
{
    atoms_.union_assign(RHS.atoms_);
    SetDefaults_();
    return *this;
}

System System::set_union(const System& RHS) const
{
    return System(*this).union_assign(RHS);
}

System& System::intersection_assign(const System& RHS)
{
    atoms_.intersection_assign(RHS.atoms_);
    SetDefaults_();
    return *this;
}

System System::intersection(const System& RHS) const
{
    return System(*this).intersection_assign(RHS);
}

System& System::difference_assign(const System& RHS)
{
    atoms_.difference_assign(RHS.atoms_);
    SetDefaults_();
    return *this;
}

System System::difference(const System& RHS) const
{
    return System(*this).difference_assign(RHS);
}

System System::complement(void) const
{
    return System(atoms_.complement());
}

System System::partition(System::SelectorFunc selector) const
{
    return System(atoms_.partition(selector));
}
System System::transform(System::TransformerFunc Transformer) const
{
    return System(atoms_.transform(Transformer));
}

//! \todo will only be true if the universes are the same
bool System::operator==(const System& RHS)const
{
    return(compare_info(RHS) && atoms_ == RHS.atoms_);
}

bool System::compare_info(const System & RHS)const
{
    PRAGMA_WARNING_PUSH
    PRAGMA_WARNING_IGNORE_FP_EQUALITY
    return(charge_ == RHS.charge_ &&
           multiplicity_ == RHS.multiplicity_ &&
           nelectrons_ == RHS.nelectrons_);
    PRAGMA_WARNING_POP
}

bool System::is_proper_subset_of(const System& RHS)const
{
    return atoms_.is_proper_subset_of(RHS.atoms_);
}

bool System::is_subset_of(const System& RHS)const
{
    return atoms_.is_subset_of(RHS.atoms_);
}

bool System::is_proper_superset_of(const System& RHS)const
{
    return atoms_.is_proper_superset_of(RHS.atoms_);
}

bool System::is_superset_of(const System& RHS)const
{
    return atoms_.is_superset_of(RHS.atoms_);
}


System& System::operator+=(const System& rhs) { return union_assign(rhs); }
System System::operator+(const System& rhs)const { return set_union(rhs); }
System& System::operator/=(const System& rhs) { return intersection_assign(rhs); }
System System::operator/(const System& rhs)const { return intersection(rhs); }
System& System::operator-=(const System& rhs) { return difference_assign(rhs); }
System System::operator-(const System& rhs)const { return difference(rhs); }
bool System::operator<=(const System& rhs)const { return is_subset_of(rhs); }
bool System::operator<(const System& rhs)const { return is_proper_subset_of(rhs); }
bool System::operator>=(const System& rhs)const { return is_superset_of(rhs); }
bool System::operator>(const System& rhs)const { return is_proper_superset_of(rhs); }




math::Point System::center_of_mass(void) const
{
    return math::weighted_points_center<math::Point>(*this, &Atom::mass);
}

math::Point System::center_of_nuclear_charge(void) const
{
    return math::weighted_points_center<math::Point>(*this, &Atom::Z);
}

bool System::has_basis_set(const std::string & basislabel) const
{
    for(const auto & atom:*this)
        if(atom.basis_sets.count(basislabel))
            return true;

    return false;

}

BasisSet System::get_basis_set(const std::string & basislabel) const
{
    if(!has_basis_set(basislabel))
        throw SystemException("Attempted to get missing basis label", "label", basislabel);

    // get the number of primitives and storage needed in basis set
    size_t nprim=0;
    size_t ncoef=0;
    size_t nshell=0;

    for(const auto & atom:*this)
    {
        if(atom.basis_sets.count(basislabel))
        {
            const BasisInfo & binfo = atom.basis_sets.at(basislabel);
            nshell += binfo.shells.size();

            for(const auto & shell : binfo.shells)
            {
                nprim += shell.n_primitives();
                ncoef += shell.n_coefficients();
            }
        }
    }

    BasisSet bs(nshell, nprim, ncoef, 3*nprim);

    // now add them
    for(const auto & atom:*this)
    {
        if(atom.basis_sets.count(basislabel))
        {
            for(const auto & shell : atom.basis_sets.at(basislabel).shells)
                bs.add_shell(shell, atom.get_coords());
        }
    }

    // shrink the basis set
    return bs.shrink_fit();
}

void System::print(std::ostream & os) const
{
    for(const auto & it:*this)
        it.print(os);
}

std::string System::to_string()const
{
    std::stringstream ss;
    print(ss);
    return ss.str();
}


bphash::HashValue System::my_hash(void) const
{
    return bphash::make_hash(bphash::HashType::Hash128, *this);
}

void System::hash(bphash::Hasher & h) const
{
    h(atoms_, charge_, multiplicity_, nelectrons_);
}

///Returns the distance between each pair of atoms in sys
DistMat_t get_distance(const System& sys)
{
    DistMat_t DM;
    for(const Atom& atomI:sys)
    {
        for(const Atom& atomJ:sys)
        {
            if(atomJ==atomI)break;
            double dist=atomI.distance(atomJ);
            DM.emplace(std::make_pair(atomI,atomJ),dist);
            DM.emplace(std::make_pair(atomJ,atomI),dist);
        }
    }
    return DM;
}

Conn_t get_connectivity(const System& sys,double Tolerance)
{
    Conn_t Conns;
    
    for(const Atom& AtomI:sys)Conns[AtomI]=std::unordered_set<Atom>();

    for(const Atom& atomI:sys)
    {
        double radI=atomI.cov_radius;
        for(const Atom& atomJ:sys)
        {
            if(atomI==atomJ)break;
            if(atomI.distance(atomJ)<Tolerance*(radI+atomJ.cov_radius))
            {
                Conns[atomI].insert(atomJ);
                Conns[atomJ].insert(atomI);
            }
        }
    }

    return Conns;
}

std::array<double,9> inertia_tensor(const System& Mol){
    std::array<double,9> I;
    double mass=0.0;
    for(const Atom& AtomI: Mol){
        double massI=AtomI.mass;
        mass+=massI;
        for(size_t j=0;j<3;++j)
            for(size_t i=0;i<3;++i)
                I[j*3+j]+=massI*AtomI[i]*AtomI[i];
    }
    std::vector<double> TempI=math::moment<2>(Mol, &Atom::mass);
    for(size_t i=0;i<9;++i)I[i]-=mass*TempI[i];
    return I;
    
}

System system_to_angstroms(const System& Sys){
    return Sys.transform([](const Atom& i)->Atom{
        std::array<double,3> NewCoords;
        std::transform(i.begin(),i.end(),NewCoords.begin(),
                       [](const double& a){return a*BOHR_RADIUS_ANGSTROMS;});
        Atom Clone(i);
        Clone.set_coords(NewCoords);
        return Clone;
    });
}

} // close namespace system
} // close namespace pulsar

