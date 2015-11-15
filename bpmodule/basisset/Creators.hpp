#ifndef _GUARD_CREATORS_HPP_
#define _GUARD_CREATORS_HPP_

#include <map>
#include <vector>

#include "bpmodule/basisset/BasisSet.hpp"
#include "bpmodule/molecule/Molecule.hpp"

namespace bpmodule {
namespace basisset {

namespace detail {

typedef std::map<int, std::vector<GaussianBasisShell>> BasisMap;

BasisMap ReadBasisFile(const std::string & path);

}




BasisSet SimpleCreator(const std::string & basispath, const molecule::Molecule & mol);



} // close namespace basisset
} // close namespace bpmodule


#endif
