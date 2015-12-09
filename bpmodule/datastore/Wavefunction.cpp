#include <atomic>
#include "bpmodule/datastore/Wavefunction.hpp"
#include "bpmodule/basisset/BasisSet.hpp"
#include "bpmodule/molecule/Molecule.hpp"
#include "bpmodule/tensor/Matrix.hpp"

namespace {
    std::atomic<unsigned long> curid_;
}

namespace bpmodule {
namespace datastore {

Wavefunction::Wavefunction(const basisset::BasisSet & basis,
                           const molecule::Molecule & molecule,
                           const tensor::DistMatrixD & cmat,
                           const tensor::DistMatrixD & epsilon)
    : uid_(curid_++), 
      basis_(new basisset::BasisSet(basis)), 
      molecule_(new molecule::Molecule(molecule)),
      cmat_(new tensor::DistMatrixD(cmat)),
      epsilon_(new tensor::DistMatrixD(epsilon))
{ }



} // close namespace datastore
} // close namespace bpmodule
