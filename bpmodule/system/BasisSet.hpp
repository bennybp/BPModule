/*! \file
 *
 * \brief Main BasisSet object (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef BPMODULE_GUARD_SYSTEM__BASISSET_HPP_
#define BPMODULE_GUARD_SYSTEM__BASISSET_HPP_

#include <functional>

#include "bpmodule/system/BasisSetShell.hpp"
#include "bpmodule/system/BasisShellInfo.hpp"

// forward declare
namespace bpmodule {
namespace util {
class Hash;
} }


namespace bpmodule {
namespace system {


class BasisSet
{
    public:
        typedef std::vector<BasisSetShell>::const_iterator const_iterator;
        typedef BasisSetShell value_type;
        typedef std::function<BasisSetShell (const BasisSetShell &)> TransformerFunc;

        BasisSet(size_t nshells, size_t nprim, size_t ncoef, size_t nxyz);

        BasisSet(const BasisSet & rhs);
        BasisSet & operator=(const BasisSet & rhs);
        BasisSet(BasisSet && rhs)                  = default;
        BasisSet & operator=(BasisSet && rhs)      = default;

        bool operator==(const BasisSet & rhs) const;

        size_t NShell(void) const noexcept;
        size_t NUniqueShell(void) const noexcept;

        const BasisSetShell & Shell(size_t i) const;
        const BasisSetShell & UniqueShell(size_t i) const;
        BasisShellInfo ShellInfo(size_t i) const;

        size_t NPrim(void) const;
        size_t NCoef(void) const;
        size_t NCartesian(void) const;
        size_t NFunctions(void) const;

        size_t MaxNPrim(void) const;
        int MaxAM(void) const;
        size_t MaxNCartesian(void) const;
        size_t MaxNFunctions(void) const;

        void AddShell(const BasisShellInfo & bshell,
                      ID_t center,
                      const CoordType & xyz);


        //! \todo make a printer class?
        void Print(std::ostream & os) const;

        BasisSet ShrinkFit(void) const;

        BasisSet Transform(TransformerFunc transformer) const;
        BasisSet TransformUnique(TransformerFunc transformer) const;

        // iterate over shells
        const_iterator begin(void) const;
        const_iterator end(void) const;

        /*! \brief Obtain a hash of this BasisSet */
        util::Hash MyHash(void) const;


        /*! \brief For serialization only
         * 
         * \warning NOT FOR USE OUTSIDE OF SERIALIZATION
         * \todo Replace if cereal fixes this
         */
        BasisSet() = default;


    private:
        ID_t curid_;
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


        void AddShell_(const BasisShellBase & bshell, ID_t id,
                       ID_t center, const CoordType & xyz);

        void AddShell_(const BasisSetShell & bshell);

        bool IsUniqueShell_(size_t i) const;


        /*! \brief Set the internal pointers to the proper locations */
        void ResetPointers_(void);


        /*! \brief Allocate enough memory for the given information
         *
         * Also set up some pointers
         */
        void Allocate_(size_t nshells, size_t nprim, size_t ncoef, size_t nxyz);


        //! \name Serialization
        ///@{

        DECLARE_SERIALIZATION_FRIENDS

        template<class Archive>
        void save(Archive & ar) const
        {
            // serialize the size info
            ar(max_nxyz_, max_nalpha_, max_ncoef_);
            ar(xyz_pos_, alpha_pos_, coef_pos_);

            // serialize the storage and shell info
            ar(storage_, shells_, unique_shells_);

            // offsets for xyz, alpha, and coef for each shell
            std::vector<ptrdiff_t> offsets;
            offsets.reserve(3*shells_.size());

            const uintptr_t base = reinterpret_cast<uintptr_t>(storage_.data());
            for(const auto & it : shells_)
            {
                offsets.push_back(reinterpret_cast<uintptr_t>(it.AlphaPtr())-base);
                offsets.push_back(reinterpret_cast<uintptr_t>(it.AllCoefsPtr())-base);
                offsets.push_back(reinterpret_cast<uintptr_t>(it.CoordsPtr())-base);
            }

            ar(offsets);
        }

        template<class Archive>
        void load(Archive & ar)
        {
            // load the size info
            ar(max_nxyz_, max_nalpha_, max_ncoef_);
            ar(xyz_pos_, alpha_pos_, coef_pos_);

            // storage and shell info
            ar(storage_, shells_, unique_shells_);

            // offsets for xyz, alpha, and coef for each shell
            std::vector<ptrdiff_t> offsets;
            ar(offsets);

            // now loop over the shells and set the pointers
            const uintptr_t base = reinterpret_cast<uintptr_t>(storage_.data());

            size_t offsetidx = 0;
            for(auto & it : shells_)
            {
                it.SetPtrs_(reinterpret_cast<double *>(base + offsets.at(offsetidx)),
                            reinterpret_cast<double *>(base + offsets.at(offsetidx+1)),
                            reinterpret_cast<double *>(base + offsets.at(offsetidx+2)));
                offsetidx += 3;
            }
        }

        ///@}
};


//! The common tag for a null basis set (ie, for 3- and 2-center ERI)
#define NULL_BASIS "NULL_BASIS"



} // close namespace system
} // close namespace bpmodule


#endif
