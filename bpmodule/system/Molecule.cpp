#include "bpmodule/system/Molecule.hpp"
#include "bpmodule/system/AtomicInfo.hpp"

namespace bpmodule {
namespace system {


Molecule::Molecule(std::shared_ptr<const AtomSetUniverse> universe, bool fill)
    : Base_t(universe, fill)
{
    // empty for now, but stuff may be added in the future
}

int Molecule::NAtoms(void) const noexcept
{
    // from base class
    return this->size();
}

std::set<std::string> Molecule::AllTags(void) const
{
    std::set<std::string> tags;
    for(const auto & it : *this)
        tags.insert(it.GetTag());
    return tags;
}


std::map<std::string, Molecule> Molecule::Fragments(void) const
{
    std::map<std::string, Molecule> ret;
    std::set<std::string> alltags = AllTags();

    for(const auto & tag : alltags)
        ret.emplace(tag, PartitionMathSet(*this, [tag](const Atom & a) { return a.GetTag() == tag; }));
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

