#include "bpmodule/basisset/BasisSet.hpp"
#include "bpmodule/exception/BasisSetException.hpp"


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


GaussianShell BasisSet::Shell(int i) const
{
    if(static_cast<size_t>(i) < shells_.size() )
        return shells_[i];
    else
        throw exception::BasisSetException("Shell index out of range",
                                           "index", i, "nshells", shells_.size());
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
