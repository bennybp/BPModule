#include "bpmodule/system/Molecule.hpp"
#include "bpmodule/system/AtomicInfo.hpp"

namespace bpmodule {
namespace system {


/*Molecule::Molecule(void)
    : curid_(0)
{
}


void Molecule::AddAtom(int z, CoordType xyz)
{
    int isonum = MostCommonIsotopeFromZ(z);

    atoms_.push_back(Atom{curid_, z, isonum, xyz});
    curid_++;
}


//! \todo wrap exceptions
Atom Molecule::GetAtom(int i) const
{
    return atoms_.at(i);
}

int Molecule::NAtoms(void) const noexcept
{
    return static_cast<int>(atoms_.size());
}


Molecule::const_iterator Molecule::begin(void) const
{
    return atoms_.begin();
}


Molecule::const_iterator Molecule::end(void) const
{
    return atoms_.end();
}*/

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


} // close namespace system
} // close namespace bpmodule

