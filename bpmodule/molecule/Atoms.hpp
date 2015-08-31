#ifndef ATOMS_HPP
#define ATOMS_HPP

#include <string>

namespace bpmodule {
namespace molecule {

int AtomSymbolToZ(const std::string & sym);

std::string AtomZToSymbol(int Z);

} // close namespace molecule
} // close namespace bpmodule


#endif
