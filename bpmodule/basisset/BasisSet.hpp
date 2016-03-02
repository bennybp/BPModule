#ifndef BPMODULE_GUARD_BASISSET__BASISSET_HPP_
#define BPMODULE_GUARD_BASISSET__BASISSET_HPP_

#include <vector>
#include <functional>

#include "bpmodule/basisset/BasisSetShell.hpp"

namespace bpmodule {
namespace basisset {


class BasisSet
{
    public:
        typedef std::vector<BasisSetShell>::iterator iterator;
        typedef std::vector<BasisSetShell>::const_iterator const_iterator;

        BasisSet(void);

        // compiler generated ok
        BasisSet(const BasisSet & rhs)             = default;
        BasisSet(BasisSet && rhs)                  = default;
        BasisSet & operator=(const BasisSet & rhs) = default;
        BasisSet & operator=(BasisSet && rhs)      = default;

        void AddShell(const BasisShell & bshell,
                      unsigned long center,
                      const BasisSetShell::CoordType & xyz);


        int NShell(void) const noexcept;

        BasisSetShell Shell(int i) const;

        int NPrim(void) const;
        int NCartesian(void) const;
        int NFunctions(void) const;

        int MaxNPrim(void) const;
        int MaxAM(void) const;
        int MaxNCartesian(void) const;
        int MaxNFunctions(void) const;

        //! \todo make a printer class?
        void Print(void) const;

        BasisSet Transform(std::function<BasisSetShell(const BasisSetShell &)> transformer) const;

        // iterate over shells
        const_iterator begin(void) const;
        const_iterator end(void) const;

    private:
        unsigned long curid_;
        std::vector<BasisSetShell> shells_;

        void AddShell_(const BasisSetShell & bsshell);
};


} // close namespace basisset
} // close namespace bpmodule


#endif
