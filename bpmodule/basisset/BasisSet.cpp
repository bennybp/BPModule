#include <algorithm>

#include "bpmodule/output/Output.hpp"
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

int BasisSet::MaxNPrim(void) const
{
    return std::max_element(this->begin(), this->end(), 
                    [](const GaussianShell & lhs, const GaussianShell & rhs) { return lhs.NPrim() < rhs.NPrim(); })->NPrim();
}

int BasisSet::MaxAM(void) const
{
    return std::max_element(this->begin(), this->end(), 
                    [](const GaussianShell & lhs, const GaussianShell & rhs) { return lhs.AM() < rhs.AM(); })->AM();
}

BasisSet::const_iterator BasisSet::begin(void) const
{
    return shells_.begin();
}

BasisSet::const_iterator BasisSet::end(void) const
{
    return shells_.end();
}


void BasisSet::Print(void) const
{
    int nshell = NShell();

    output::Output("Basis set with %1% shells\n", nshell);
    for(int i = 0; i < nshell; i++)
    {
        const auto shell = Shell(i);
        output::Output("Shell %1%  AM=%2%  Cart=%3%  NPrim=%4%\n", i, shell.AM(), shell.IsCartesian(), shell.NPrim());
        for(int j = 0; j < shell.NPrim(); ++j)
            output::Output("    %1%    %2%\n", shell.Alpha(j), shell.Coef(j));
    }
    output::Output("\n");
}


} // close namespace basisset
} // close namespace bpmodule
