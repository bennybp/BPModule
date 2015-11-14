#include "bpmodule/molecule/Atom.hpp"
#include "bpmodule/molecule/AtomicInfo.hpp"

namespace bpmodule {
namespace molecule {

double Atom::Mass(void) const
{
    return IsotopeMassFromZ(z, isonum);
}

std::string Atom::Name(void) const
{
    return AtomicNameFromZ(z);

}

std::string Atom::Symbol(void) const
{
    return AtomicSymFromZ(z);
}


} // close namespace molecule
} // close namespace bpmodule

