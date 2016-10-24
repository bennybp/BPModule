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
using pulsar::SystemException;


// Instantiate the MathSet, etc, templates
template class pulsar::math::Universe<pulsar::system::Atom>;
template class pulsar::math::MathSet<pulsar::system::Atom>;


namespace pulsar{
namespace system {

void System::SetDefaults_(void)
{
    charge=get_sum_charge();
    nelectrons=get_sum_n_electrons();
    mass=get_sum_mass();
    //! \todo default multiplicity
    multiplicity=1.0;
}

System::System(std::shared_ptr<const AtomSetUniverse> universe,bool fill)
    :atoms_(universe,fill)
{
    SetDefaults_();
}

System::System(AtomSetUniverse&& universe,bool fill):
    System(std::make_shared<AtomSetUniverse>(std::move(universe)),fill)
{
}

System::System(const AtomSetUniverse& universe,bool fill):
    System(std::make_shared<AtomSetUniverse>(universe),fill)
{
}

//Macro for common code that adds stuff up
#define SUM_STUFF(varname,fxn_name)\
double System::fxn_name(void) const{\
    return std::accumulate(this->begin(),this->end(),static_cast<double>(0.0),\
                        [](double sum,const Atom& a){ return sum+a.varname; });}

SUM_STUFF(charge,get_sum_charge)
SUM_STUFF(nelectrons,get_sum_n_electrons)
SUM_STUFF(mass,get_sum_mass)

#undef SUM_STUFF

//Macro for adding atoms and then calling SetDefaults_
#define ADD_ATOM(rv,fxn_name,lv,lvname)\
rv System::fxn_name(lv){\
   atoms_.fxn_name(lvname);SetDefaults_();return *this;}
ADD_ATOM(System&,insert,const Atom& atom,atom)
ADD_ATOM(System&,insert,Atom&& atom,std::move(atom))
ADD_ATOM(System&,union_assign,const System& RHS,RHS.atoms_)
ADD_ATOM(System&,intersection_assign,const System& RHS,RHS.atoms_)
ADD_ATOM(System&,difference_assign,const System& RHS,RHS.atoms_)
#undef ADD_ATOM

System System::set_union(const System& RHS) const
{
    return System(*this).union_assign(RHS);
}



System System::intersection(const System& RHS) const
{
    return System(*this).intersection_assign(RHS);
}



System System::difference(const System& RHS) const
{
    return System(*this).difference_assign(RHS);
}

//Macro for forwarding a call to the MathSet
#define CALL_ATOMS(rv,fxn_name,arg,arg2)\
rv System::fxn_name(arg)const{return atoms_.fxn_name(arg2);}

CALL_ATOMS(size_t,size,void,)
CALL_ATOMS(bool,count,const Atom& AnAtom,AnAtom)
CALL_ATOMS(std::shared_ptr<const AtomSetUniverse>,get_universe,void,)
CALL_ATOMS(AtomSetUniverse,as_universe,void,)
CALL_ATOMS(bool,is_proper_subset_of,const System& RHS,RHS.atoms_)
CALL_ATOMS(bool,is_subset_of,const System& RHS,RHS.atoms_)
CALL_ATOMS(bool,is_proper_superset_of,const System& RHS,RHS.atoms_)
CALL_ATOMS(bool,is_superset_of,const System& RHS,RHS.atoms_)

//Similar to CALL_ATOMS except the result is used to make a new system
#define WRAP_CALL(fxn_name,arg1,arg2)\
System System::fxn_name(arg1)const{System temp(*this);\
    temp.atoms_=atoms_.fxn_name(arg2);\
    temp.SetDefaults_();return temp;}

WRAP_CALL(complement,void,)
WRAP_CALL(partition,System::SelectorFunc selector,selector)
WRAP_CALL(transform,System::TransformerFunc Transformer,Transformer)

#undef WRAP_CALL
#undef CALL_ATOMS


bool System::operator==(const System& RHS)const
{
    return(compare_info(RHS) && 
            (atoms_ == RHS.atoms_ || as_universe()==RHS.as_universe() )
    );
}

bool System::compare_info(const System & RHS)const
{
    PRAGMA_WARNING_PUSH
    PRAGMA_WARNING_IGNORE_FP_EQUALITY
    return(mass == RHS.mass &&
           charge == RHS.charge &&
           multiplicity == RHS.multiplicity &&
           nelectrons == RHS.nelectrons);
    PRAGMA_WARNING_POP
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

std::ostream& System::print(std::ostream & os) const
{
    for(const auto & it:*this)
        it.print(os);
    return os;
}

bphash::HashValue System::my_hash(void) const
{
    return bphash::make_hash(bphash::HashType::Hash128, *this);
}

void System::hash(bphash::Hasher & h) const
{
    for(Atom a: *this)h(a);
    h(charge, multiplicity, nelectrons,mass);
}

math::Point center_of_mass(const System& Sys)
{
    return math::weighted_points_center<math::Point>(Sys, &Atom::mass);
}

math::Point center_of_nuclear_charge(const System& Sys)
{
    return math::weighted_points_center<math::Point>(Sys, &Atom::Z);
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

