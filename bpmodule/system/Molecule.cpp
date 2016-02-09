#include "bpmodule/system/Molecule.hpp"
#include "bpmodule/system/AtomicInfo.hpp"

namespace bpmodule {
namespace system {


Molecule::Molecule(void)
    : curid_(0)
{
}


//! \todo wrap exceptions
Atom Molecule::GetAtom(size_t i) const
{
    return atoms_.at(i);
}

void Molecule::SetAtom(size_t i, const Atom & a)
{
    atoms_.at(i) = a;
    charge_ += a.GetCharge();
    nelectrons_ += a.GetNElectrons();
}

void Molecule::AddAtom(const Atom & a)
{
    Atom toappend(a);
    toappend.SetID(curid_++);
    atoms_.push_back(toappend);
}

int Molecule::NAtoms(void) const noexcept
{
    return static_cast<int>(atoms_.size());
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


Molecule::const_iterator Molecule::begin(void) const
{
    return atoms_.begin();
}


Molecule::const_iterator Molecule::end(void) const
{
    return atoms_.end();
}

/*
void Molecule::AddAtom(const Atom& A){
    (*this)<<A;
    Charge_+=A.Charge();
}

std::string Molecule::ToString()const{
    std::stringstream ss;
    const_iterator AtomI=begin(),AtomEnd=end();
    for(;AtomI!=AtomEnd;++AtomI)
        ss<<*AtomI<<std::endl;
    return ss.str();
}
*/

} // close namespace system
} // close namespace bpmodule

