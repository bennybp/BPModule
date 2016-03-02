#include "bpmodule/system/Molecule.hpp"
#include "bpmodule/basisset/BasisSet.hpp"
#include "bpmodule/system/AtomicInfo.hpp"


using bpmodule::exception::SystemException;


// Instantiate the MathSet, etc, templates
template class bpmodule::math::Universe<bpmodule::system::Atom>;
template class bpmodule::math::MathSet<bpmodule::system::Atom>;


namespace bpmodule {
namespace system {




Molecule::Molecule(const AtomSet & atoms)
    : atoms_(atoms)
{
}


Molecule::Molecule(std::shared_ptr<const AtomSetUniverse> universe, bool fill)
    : atoms_(universe, fill)
{
    // empty for now, but stuff may be added in the future
}


bool Molecule::HasAtom(size_t atomidx) const
{
    for(const auto & it : atoms_)
        if(it.GetIdx() == atomidx)
            return true;
    return false;
}

Atom Molecule::GetAtom(size_t atomidx) const
{
    for(const auto & it : atoms_)
        if(it.GetIdx() == atomidx)
            return it;
    throw SystemException("This molecule doesn't have an atom with this index",
                          "atomidx", atomidx);
    
}

int Molecule::NAtoms(void) const
{
    return atoms_.size();
}

double Molecule::GetCharge(void) const
{
    return std::accumulate(this->begin(), this->end(), static_cast<double>(0.0),
                           [](double sum, const Atom & a) { return sum + a.GetCharge(); });
}


double Molecule::GetNElectrons(void) const
{
    return std::accumulate(this->begin(), this->end(), static_cast<double>(0.0),
                           [](double sum, const Atom & a) { return sum + a.GetNElectrons(); });
}

Molecule Molecule::Partition(Molecule::SelectorFunc selector) const
{
    return Molecule(atoms_.Partition(selector));
}

Molecule Molecule::Transform(Molecule::TransformerFunc transformer) const
{
    return Molecule(atoms_.Transform(transformer));
}

Molecule Molecule::Complement(void) const
{
    return Molecule(atoms_.Complement());
}

void Molecule::Insert(const Atom & atom)
{
    //! \todo Named functions in MathSet
    atoms_ << atom;
}

Molecule Molecule::Intersection(const Molecule & rhs) const
{
    //! \todo Named functions in MathSet
    return Molecule(atoms_ / rhs.atoms_);
}

Molecule Molecule::Union(const Molecule & rhs) const
{
    //! \todo Named functions in MathSet
    return Molecule(atoms_ + rhs.atoms_);
}

Molecule Molecule::Difference(const Molecule & rhs) const
{
    //! \todo Named functions in MathSet
    return Molecule(atoms_ - rhs.atoms_);
}


math::Point Molecule::CenterOfMass(void) const
{
    return math::WeightedPointsCenter<math::Point>(*this, &Atom::GetMass);
}


math::Point Molecule::CenterOfNuclearCharge(void) const
{
    return math::WeightedPointsCenter<math::Point>(*this, &Atom::GetZ);
}

basisset::BasisSet Molecule::GetBasisSet(const std::string & basislabel) const
{
    basisset::BasisSet bs;
    for(const auto & atom : *this)
        for(const auto & bshell : atom.GetShells(basislabel))
            bs.AddShell(bshell, atom.GetIdx(), atom.GetCoords());

    return bs;
}


std::string Molecule::ToString()const{
    std::stringstream ss;
    for(const auto & it : *this)
        ss<<it<<"\n";
    return ss.str();
}


} // close namespace system
} // close namespace bpmodule

