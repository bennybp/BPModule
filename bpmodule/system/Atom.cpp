#include "bpmodule/system/Atom.hpp"
#include "bpmodule/system/AtomicInfo.hpp"

namespace bpmodule {
namespace system {

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


} // close namespace system
} // close namespace bpmodule

