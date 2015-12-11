#ifndef _GUARD_BASISSET_HPP_
#define _GUARD_BASISSET_HPP_

#include <vector>

#include "bpmodule/basisset/GaussianShell.hpp"

namespace bpmodule {
namespace basisset {


class BasisSet
{
    public:
        typedef std::vector<GaussianShell>::iterator iterator;
        typedef std::vector<GaussianShell>::const_iterator const_iterator;

        BasisSet(void);

        // compiler generated ok
        BasisSet(const BasisSet & rhs)             = default;
        BasisSet(BasisSet && rhs)                  = default;
        BasisSet & operator=(const BasisSet & rhs) = default;
        BasisSet & operator=(BasisSet && rhs)      = default;

        // note - pass by copy
        void AddShell(GaussianShell gs);


        int NShell(void) const noexcept;

        GaussianShell Shell(int i) const;

        int MaxNPrim(void) const;
        int MaxAM(void) const;


        // iterate over shells
        const_iterator begin(void) const;
        const_iterator end(void) const;

    private:
        unsigned long curid_;
        std::vector<GaussianShell> shells_;
};


} // close namespace basisset
} // close namespace bpmodule


#endif
