#ifndef PULSAR_GUARD_SYSTEM__GENERALSHELLITERATOR_HPP_
#define PULSAR_GUARD_SYSTEM__GENERALSHELLITERATOR_HPP_

#include "pulsar/system/BasisShellBase.hpp"
#include "pulsar/exception/Assert.hpp"
#include "pulsar/exception/PulsarException.hpp"

#include <functional>

namespace pulsar {

/*! \brief Iterate over the general contractions in a collection of shells
 *
 * \todo python version
 * \todo assertion for zero functions 
 */
template<int N>
class GeneralShellIterator
{

    public:
        typedef std::reference_wrapper<const BasisShellBase> wrap_type;

        /*! \brief Constructor
         * 
         * \note References to the shells are not stored, therefore it is
         *       safe to modify, delete, etc, the shells after this
         *       object is constructed.
         *
         * \param [in] shells The shells to iterate over
         * \param [in] splitcombined Split combined angular momentum shells into
         *                           its parts (ie, split an sp shell into
         *                           an s and a p shell.
         */
        GeneralShellIterator(const std::array<wrap_type, N> shells, bool splitcombined)
        {
            

            totalnshell_ = 1;

            // fill in the information
            for(int i = 0; i < N; i++)
            {
                const BasisShellBase & sh = shells[i].get();
                if(splitcombined && sh.is_combined_am())
                {
                    ngen_[i] = 1;  // aren't splitting the combined am
                    am_[i].push_back(sh.am());
                }
                else
                {
                    ngen_[i] = sh.n_general_contractions();
                    for(size_t j = 0; j < ngen_[i]; j++)
                        am_[i].push_back(sh.general_am(j));
                }

                psr_assert(am_[i].size() > 0, "Zero angular momentum entries");
                curam_[i] = am_[i][0];

                totalnshell_ *= ngen_[i];
                genidx_[i] = 0; 
            }

            // start at zero
            totalidx_ = 0;
            valid_ = true;

        }

        GeneralShellIterator(const GeneralShellIterator &) = default;
        GeneralShellIterator & operator=(const GeneralShellIterator &) = default;
        GeneralShellIterator(GeneralShellIterator &&) = default;
        GeneralShellIterator & operator=(GeneralShellIterator &&) = default;

        template<int N>
        size_t n_general_contractionsShells(void) const noexcept
        {
            return std::get<N>(ngen_);
        }

        size_t n_general_contractionsShells(int n) const ASSERTIONS_ONLY
        {
            
            psr_assert(n < N, "Given index is beyond the number of shells I have"); 
            return ngen_[n];
        }

        size_t NTotalGeneralShells(void) const noexcept
        {
            return totalnshell_;
        }


        bool Valid(void) const noexcept
        {
            return valid_;
        }

        size_t TotalIdx(void) const noexcept
        {
            return totalidx_;
        }

        template<int N>
        size_t GeneralIdx(void) const noexcept
        {
            return std::get<N>(genidx_);
        }

        size_t GeneralIdx(int n) const ASSERTIONS_ONLY
        {
            
            psr_assert(n < N, "Given index is beyond the number of shells I have"); 
            return genidx_[n];
        }

        template<int N>
        int AM(void) const noexcept
        {
            return std::get<N>(curam_);
        }

        int AM(int n) const ASSERTIONS_ONLY
        {
            
            psr_assert(n < N, "Given index is beyond the number of shells I have");
            return curam_[n];
        }


        bool Next(void) ASSERTIONS_ONLY
        {
            

            if(!Valid())
                return false;

            totalidx_++;

            for(int i = (N-1); i >= 0; i--)
            {
                genidx_[i]++;
                if(genidx_[i] < ngen_[i])
                {
                    psr_assert(genidx_[i] < am_[i].size(), "Inconsistency in GeneralShellIterator");
                    curam_[i] = am_[i][genidx_[i]];
                    return true;
                }
                else
                {
                    genidx_[i] = 0; // and continue through the other shells

                    psr_assert(genidx_[i] < am_[i].size(), "Inconsistency in GeneralShellIterator");
                    curam_[i] = am_[i][0];
                }
            }

            // if we get here, we've turned everything back to zero
            psr_assert(totalidx_ == totalnshell_, "Inconsistency in GeneralShellIterator");

            valid_ = false;
            totalidx_ = 0;
            return false;
        }


    private:
        std::array<size_t, N> ngen_;          //! Number of general contractions in each shell
        std::array<size_t, N> genidx_;        //! Number of general contractions in each shell
        std::array<std::vector<int>, N> am_;  //! Angular momentum. am_[i][j] = the am for shell i, general contraction j
        std::array<int, N> curam_;            //! Angular momentum for the current iterator state

        size_t totalidx_;
        size_t totalnshell_;
        bool valid_;
};

} // close namespace system

#endif
