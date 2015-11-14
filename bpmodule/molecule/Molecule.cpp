#include "bpmodule/molecule/Molecule.hpp"
#include "bpmodule/molecule/AtomicInfo.hpp"

namespace bpmodule {
namespace molecule {


Molecule::Molecule(void)
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


Molecule::const_iterator Molecule::cbegin(void) const
{
    return atoms_.cbegin();
}


Molecule::const_iterator Molecule::cend(void) const
{
    return atoms_.cend();
}



} // close namespace molecule
} // close namespace bpmodule

