#include "bpmodule/system/Molecule.hpp"
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

Molecule::TagsType Molecule::GetAllTags(void) const
{
    std::set<std::string> tags;
    for(const auto & it : *this)
    {
        TagsType atomtags = it.GetTags();
        tags.insert(atomtags.begin(), atomtags.end());
    }
    return tags;
}

Molecule Molecule::GetFragment(const std::string & tag) const
{
    Molecule ret = atoms_.Partition([tag](const Atom & a) { return a.HasTag(tag); });
    if(ret.NAtoms() == 0)
        throw SystemException("This molecule does not have atoms with this tag",
                              "tag", tag);
    return ret;
}

Molecule::FragMapType Molecule::GetAllFragments(void) const
{
    FragMapType ret;
    TagsType alltags = GetAllTags();

    for(const auto & tag : alltags)
        ret.emplace(tag, GetFragment(tag));
    return ret;
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


Molecule Molecule::Complement(void) const
{
    return Molecule(atoms_.Complement());
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


std::string Molecule::ToString()const{
    std::stringstream ss;
    for(const auto & it : *this)
        ss<<it<<"\n";
    return ss.str();
}


} // close namespace system
} // close namespace bpmodule

