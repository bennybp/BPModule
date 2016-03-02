#ifndef BPMODULE_GUARD_BASISSET__CREATORS_HPP_
#define BPMODULE_GUARD_BASISSET__CREATORS_HPP_

#include <map>
#include <vector>

#include "bpmodule/basisset/BasisSet.hpp"
#include "bpmodule/system/Molecule.hpp"

namespace bpmodule {
namespace basisset {

namespace detail {

typedef std::map<int, ShellVector> BasisMap;

BasisMap ReadBasisFile(ShellType type, const std::string & path);

}




system::Molecule SimpleCreator(const std::string & basispath,
                               const system::Molecule & mol,
                               ShellType type,
                               const std::string & basislabel);



} // close namespace basisset
} // close namespace bpmodule


#endif
