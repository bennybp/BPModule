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
        typedef std::vector<BasisSetShell>::iterator iterator;
        typedef std::vector<BasisSetShell>::const_iterator const_iterator;
        typedef BasisSetShell value_type; // for iteration
        typedef std::function<BasisSetShell (const BasisSetShell &)> TransformerFunc;

        BasisSet(size_t nshells, size_t nprim, size_t ncoef);

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

        void ResetPointers_(void);

        void Allocate_(size_t nshells, size_t nprim, size_t ncoef);


        //! \name Serialization
        ///@{

        DECLARE_SERIALIZATION_FRIENDS

        template<class Archive>
        void save(Archive & ar) const
        {
            // stores shell info, ID, center ID, and XYZ
            typedef std::tuple<BasisShellInfo, ID_t, ID_t, CoordType> ShellTuple;

            std::vector<ShellTuple> sinfo;
            sinfo.reserve(shells_.size());

            for(const auto & it : *this)
            {
                // can convert BasisShellBase to BasisShellInfo, but
                // will slice id, center, and xyz
                BasisShellInfo bsinfo(it);
                sinfo.push_back(std::make_tuple(std::move(bsinfo), it.GetID(), it.GetCenter(), it.GetCoords()));
            }
            
            ar(sinfo);
        }

        template<class Archive>
        void load(Archive & ar)
        {
            storage_.clear();
            // stores shell info, ID, center ID, and XYZ
            typedef std::tuple<BasisShellInfo, ID_t, ID_t, CoordType> ShellTuple;

            std::vector<ShellTuple> sinfo;
            ar(sinfo);

            // find how much storage we need
            size_t nshells = sinfo.size();
            size_t nprim = 0;
            size_t ncoef = 0;

            for(const auto & it : sinfo)
            {
                const BasisShellInfo & bsi = std::get<0>(it);
                nprim += bsi.NPrim();
                ncoef += bsi.NCoef();
            }

            Allocate_(nshells, nprim, ncoef);


            // now add all elements of the vector, unpacking the tuple
            for(const auto & it : sinfo)
                AddShell_(std::get<0>(it), std::get<1>(it),
                          std::get<2>(it), std::get<3>(it));
        }

        ///@}
};


//! The common tag for a null basis set (ie, for 3- and 2-center ERI)
#define NULL_BASIS "NULL_BASIS"



} // close namespace system
} // close namespace bpmodule


#endif
