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

    atoms_.push_back(Atom{curid_, z, isonum,
                          IsotopeMassFromZ(z, isonum),
                          xyz
                         });
    curid_++;
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

