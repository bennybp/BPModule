#ifndef BPMODULE_GUARD_SYSTEM__BASISSET_HPP_
#define BPMODULE_GUARD_SYSTEM__BASISSET_HPP_

#include <vector>
#include <functional>

#include "bpmodule/system/BasisSetShell.hpp"
#include "bpmodule/system/BasisShellInfo.hpp"

namespace bpmodule {
namespace system {


class BasisSet
{
    public:
        typedef std::vector<BasisSetShell>::iterator iterator;
        typedef std::vector<BasisSetShell>::const_iterator const_iterator;
        typedef std::function<BasisSetShell (const BasisSetShell &)> TransformerFunc;

        BasisSet(size_t nprim, size_t ncoef);

        BasisSet(const BasisSet & rhs);
        BasisSet & operator=(const BasisSet & rhs);
        BasisSet(BasisSet && rhs)                  = default;
        BasisSet & operator=(BasisSet && rhs)      = default;

        int NShell(void) const noexcept;

        const BasisSetShell & GetShell(int i) const;

        int NPrim(void) const;
        int NCoef(void) const;
        int NCartesian(void) const;
        int NFunctions(void) const;

        int MaxNPrim(void) const;
        int MaxAM(void) const;
        int MaxNCartesian(void) const;
        int MaxNFunctions(void) const;

        void AddShell(const BasisShellInfo & bshell,
                      unsigned long center,
                      const BasisSetShell::CoordType & xyz);


        //! \todo make a printer class?
        void Print(void) const;

        BasisSet Transform(TransformerFunc transformer) const;

        // iterate over shells
        const_iterator begin(void) const;
        const_iterator end(void) const;

    private:
        unsigned long curid_;
        std::vector<BasisSetShell> shells_;
        std::vector<double> storage_; // storage for alpha and coef

        // for filling
        size_t nprim_;
        size_t alpha_pos_;
        size_t coef_pos_;

        void ValidateAddition_(const BasisShellBase & bshell) const;
        void AddShell_(const BasisSetShell & bshell);
};


} // close namespace system
} // close namespace bpmodule


#endif
