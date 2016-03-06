#ifndef BPMODULE_GUARD_BASISSET__CREATORS_HPP_
#define BPMODULE_GUARD_BASISSET__CREATORS_HPP_

#include <map>
#include <vector>

#include "bpmodule/system/BasisSet.hpp"
#include "bpmodule/system/Molecule.hpp"

namespace bpmodule {
namespace system {

namespace detail {

typedef std::map<int, BasisShellInfoVector> BasisMap;

BasisMap ReadBasisFile(ShellType type, const std::string & path);

}




Molecule CreateSimpleBasisSet(const std::string & basispath,
                              const Molecule & mol,
                              ShellType type,
                              const std::string & basislabel);



} // close namespace system
} // close namespace bpmodule


#endif
