#ifndef BPMODULE_GUARD_SYSTEM__BASISSET_HPP_
#define BPMODULE_GUARD_SYSTEM__BASISSET_HPP_

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

        BasisSet(size_t nshells, size_t nprim, size_t ncoef);

        BasisSet(const BasisSet & rhs);
        BasisSet & operator=(const BasisSet & rhs);
        BasisSet(BasisSet && rhs)                  = default;
        BasisSet & operator=(BasisSet && rhs)      = default;

        int NShell(void) const noexcept;
        int NUniqueShell(void) const noexcept;

        const BasisSetShell & Shell(int i) const;
        const BasisSetShell & UniqueShell(int i) const;

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
                      const CoordType & xyz);


        //! \todo make a printer class?
        void Print(std::ostream & os) const;

        BasisSet ShrinkFit(void) const;

        BasisSet Transform(TransformerFunc transformer) const;
        BasisSet TransformUnique(TransformerFunc transformer) const;

        // iterate over shells
        const_iterator begin(void) const;
        const_iterator end(void) const;

    private:
        unsigned long curid_;
        std::vector<BasisSetShell> shells_;
        std::vector<size_t> unique_shells_;
        std::vector<double> storage_; // storage for alpha and coef

        // for filling
        size_t max_nxyz_;
        size_t max_nalpha_;
        size_t max_ncoef_;
        double * xyz_base_ptr_;
        double * alpha_base_ptr_;
        double * coef_base_ptr_;

        size_t xyz_pos_;
        size_t alpha_pos_;
        size_t coef_pos_;

        void AddShell_(const BasisShellBase & bshell, unsigned long id,
                       unsigned long center, const CoordType & xyz);
        void AddShell_(const BasisSetShell & bshell);

        bool IsUniqueShell_(size_t i) const;
};


} // close namespace system
} // close namespace bpmodule


#endif
