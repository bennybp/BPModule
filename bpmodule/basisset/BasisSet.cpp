#include "bpmodule/basisset/BasisSet.hpp"


namespace bpmodule {
namespace basisset {

BasisSet::BasisSet(void)
    : curid_(0)
{ }


void BasisSet::AddShell(GaussianShell gs)
{
    gs.SetID(curid_);
    curid_++;
    shells_.push_back(gs);
}

int BasisSet::NShell(void) const noexcept
{
    return static_cast<int>(shells_.size());
}


//! \todo wrap exceptions
GaussianShell BasisSet::Shell(int i) const
{
    return shells_.at(i);
}


BasisSet::const_iterator BasisSet::begin(void) const
{
    return shells_.begin();
}

BasisSet::const_iterator BasisSet::end(void) const
{
    return shells_.end();
}



} // close namespace basisset
} // close namespace bpmodule
