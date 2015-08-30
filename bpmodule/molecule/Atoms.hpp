#ifndef ATOMS_HPP
#define ATOMS_HPP

#include <string>

namespace bpmodule {

int AtomSymbolToZ(const std::string & sym);

std::string AtomZToSymbol(int Z);

} // close namespace bpmodule


#endif
