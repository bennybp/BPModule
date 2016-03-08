/*\file
 *
 * \brief The system class (source)
*/


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
    charge_ = GetSumCharge();
    nelectrons_ = GetSumNElectrons();

    //! \todo default multiplicity
    multiplicity_ = 1.0;
}



System::System(const AtomSet & atoms)
    : atoms_(atoms)
{
    SetDefaults_();
}


System::System(std::shared_ptr<const AtomSetUniverse> universe, bool fill)
    : atoms_(universe, fill)
{
    SetDefaults_();
}


bool System::HasAtom(size_t atomidx) const
{
    for(const auto & it : atoms_)
        if(it.GetIdx() == atomidx)
            return true;
    return false;
}

Atom System::GetAtom(size_t atomidx) const
{
    for(const auto & it : atoms_)
        if(it.GetIdx() == atomidx)
            return it;

    throw SystemException("This system doesn't have an atom with this index",
                          "atomidx", atomidx);
}

int System::NAtoms(void) const
{
    return atoms_.size();
}

double System::GetSumCharge(void) const
{
    return std::accumulate(this->begin(), this->end(), static_cast<double>(0.0),
                           [](double sum, const Atom & a) { return sum + a.GetCharge(); });
}

double System::GetSumNElectrons(void) const
{
    return std::accumulate(this->begin(), this->end(), static_cast<double>(0.0),
                           [](double sum, const Atom & a) { return sum + a.GetNElectrons(); });
}

double System::GetCharge(void) const
{
    return charge_;
}

void System::SetCharge(double charge)
{
    charge_ = charge;
}

double System::GetNElectrons(void) const
{
    return nelectrons_;
}

void System::SetNElectrons(double nelectrons)
{
    nelectrons_ = nelectrons;
}

double System::GetMultiplicity(void) const
{
    return multiplicity_;
}

void System::SetMultiplicity(double m)
{
    multiplicity_ = m;
}


System System::Partition(System::SelectorFunc selector) const
{
    return System(atoms_.Partition(selector));
}

System System::Transform(System::TransformerFunc transformer) const
{
    return System(atoms_.Transform(transformer));
}

System System::Complement(void) const
{
    return System(atoms_.Complement());
}

void System::Insert(const Atom & atom)
{
    //! \todo Named functions in MathSet
    atoms_ << atom;
}

System System::Intersection(const System & rhs) const
{
    //! \todo Named functions in MathSet
    return System(atoms_ / rhs.atoms_);
}

System System::Union(const System & rhs) const
{
    //! \todo Named functions in MathSet
    return System(atoms_ + rhs.atoms_);
}

System System::Difference(const System & rhs) const
{
    //! \todo Named functions in MathSet
    return System(atoms_ - rhs.atoms_);
}


math::Point System::CenterOfMass(void) const
{
    return math::WeightedPointsCenter<math::Point>(*this, &Atom::GetMass);
}


math::Point System::CenterOfNuclearCharge(void) const
{
    return math::WeightedPointsCenter<math::Point>(*this, &Atom::GetZ);
}

bool System::HasBasisSet(const std::string & basislabel) const
{
    for(const auto & atom : *this)
        if(atom.HasShells(basislabel))
            return true;
    return false;

}

BasisSet System::GetBasisSet(const std::string & basislabel) const
{
    // get the number of primitives and storage needed in basis set
    size_t nprim = 0;
    size_t ncoef = 0;
    size_t nshell = 0;

    for(const auto & atom : *this)
    {
        nshell += atom.NShell(basislabel);

        for(const auto & bshell : atom.GetShells(basislabel))
        {
            nprim += bshell.NPrim();
            ncoef += bshell.NCoef();
        }
    }

    BasisSet bs(nshell, nprim, ncoef);

    // now add them
    for(const auto & atom : *this)
        for(const auto & bshell : atom.GetShells(basislabel))
            bs.AddShell(bshell, atom.GetIdx(), atom.GetCoords());

    // shrink the basis set
    return bs.ShrinkFit();
}


std::string System::ToString()const{
    std::stringstream ss;
    for(const auto & it : *this)
        ss<<it<<"\n";
    return ss.str();
}


} // close namespace system
} // close namespace bpmodule

