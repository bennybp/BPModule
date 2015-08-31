#ifndef _GUARD_ATOMS_HPP_
#define _GUARD_ATOMS_HPP_

#include <string>

namespace bpmodule {
namespace molecule {

int AtomSymbolToZ(const std::string & sym);

std::string AtomZToSymbol(int Z);

} // close namespace molecule
} // close namespace bpmodule


#endif
