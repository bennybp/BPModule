#include <algorithm>

#include "bpmodule/system/BasisSet.hpp"
#include "bpmodule/system/NCartesian.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/exception/Exceptions.hpp"


namespace bpmodule {
namespace system {

BasisSet::BasisSet(void)
    : curid_(0)
{ }


void BasisSet::AddShell(const BasisShellInfo & bshell,
                        unsigned long center,
                        const BasisSetShell::CoordType & xyz)
{
    AddShell_(BasisSetShell(curid_++, bshell, center, xyz));
}

void BasisSet::AddShell_(const BasisSetShell & bsshell)
{
    shells_.push_back(bsshell);
}


int BasisSet::NShell(void) const noexcept
{
    return static_cast<int>(shells_.size());
}


const BasisSetShell & BasisSet::GetShell(int i) const
{
    if(static_cast<size_t>(i) < shells_.size() )
        return shells_[i];
    else
        throw exception::BasisSetException("Shell index out of range",
                                           "index", i, "nshells", shells_.size());
}

int BasisSet::NPrim(void) const
{
    return std::accumulate(this->begin(), this->end(), 0,
                           [](int sum, const BasisSetShell & sh) { return sum + sh.NPrim(); } );
}

int BasisSet::MaxNPrim(void) const
{
    return std::max_element(this->begin(), this->end(), 
                    [](const BasisSetShell & lhs, const BasisSetShell & rhs) { return lhs.NPrim() < rhs.NPrim(); })->NPrim();
}

int BasisSet::MaxAM(void) const
{
    int max = std::max_element(this->begin(), this->end(), 
                    [](const BasisSetShell & lhs, const BasisSetShell & rhs) { return std::abs(lhs.AM()) < std::abs(rhs.AM()); })->AM();
    return std::abs(max);
}

int BasisSet::NCartesian(void) const
{
    return std::accumulate(this->begin(), this->end(), 0,
                           [](int sum, const BasisSetShell & sh) { return sum + sh.NCartesian(); } );
}

int BasisSet::NFunctions(void) const
{
    return std::accumulate(this->begin(), this->end(), 0,
                           [](int sum, const BasisSetShell & sh) { return sum + sh.NFunctions(); } );
}

int BasisSet::MaxNCartesian(void) const
{
    return NCARTESIAN(MaxAM());
}

int BasisSet::MaxNFunctions(void) const
{
    return std::max_element(this->begin(), this->end(), 
                    [](const BasisSetShell & lhs, const BasisSetShell & rhs) { return lhs.NFunctions() < rhs.NFunctions(); })->NFunctions();
}

BasisSet::const_iterator BasisSet::begin(void) const
{
    return shells_.begin();
}

BasisSet::const_iterator BasisSet::end(void) const
{
    return shells_.end();
}

BasisSet BasisSet::Transform(BasisSet::TransformerFunc transformer) const
{
    BasisSet bs;
    for(const auto & shell : *this)
        bs.AddShell_(transformer(shell));
    return bs;
}


void BasisSet::Print(void) const
{
    int nshell = NShell();

    output::Output("Basis set with %1% shells\n", nshell);
    output::Output("NCart = %1% , MaxAM = %2%\n", NCartesian(), MaxAM());
    output::Output("MaxNCart = %1% , MaxNPrim = %2%\n", MaxNCartesian(), MaxNPrim());

    for(int i = 0; i < nshell; i++)
    {
        const auto & shell = GetShell(i);
        output::Output("Shell %1%  AM=%2%  Cart=%3%  NPrim=%4% NGen=%5%\n", i, shell.AM(), shell.IsCartesian(), shell.NPrim(), shell.NGeneral());
        for(int j = 0; j < shell.NPrim(); ++j)
        {
            output::Output("    %1%", shell.GetAlpha(j));
            for(int n = 0; n < shell.NGeneral(); n++)
                output::Output("    %1%", shell.GetCoef(n, j));
            output::Output("\n");
        }
    }
    output::Output("\n");
}


} // close namespace system
} // close namespace bpmodule
