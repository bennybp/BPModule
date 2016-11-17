#ifndef PULSAR_GUARD_SYSTEM__AOITERATOR_HPP_
#define PULSAR_GUARD_SYSTEM__AOITERATOR_HPP_

#include "pulsar/system/BasisShellBase.hpp"
#include "pulsar/system/GeneralShellIterator.hpp"
#include "pulsar/exception/Assert.hpp"
#include "pulsar/exception/PulsarException.hpp"

#include <functional>

namespace pulsar {

//! \todo python version
template<int N>
class AOIterator
{

    public:
        typedef std::reference_wrapper<const BasisShellBase> wrap_type;

        AOIterator(const std::array<wrap_type, N> shells, bool splitcombined)
        {
            totalnfunc_ = 1;

            // fill in the information
            for(int i = 0; i < N; i++)
            {
                const BasisShellBase & sh = shells[i].get();

                // store the total number of functions
                totalnfunc_ *= sh.n_functions();

                genidx_[i] = funcidx_[i] = shellfuncidx_[i] = 0;

                if(splitcombined && sh.is_combined_am())
                {
                    ngen_[i] = 1;
                    nfunc_[i].push_back(sh.n_functions());
                }
                else
                {
                    ngen_[i] = sh.n_general_contractions();
                    nfunc_[i].resize(ngen_[i]);
                    for(size_t j = 0; j < ngen_[i]; j++)
                        nfunc_[i][j] = sh.general_n_functions(j);
                }
            }

            // start at zero
            totalidx_ = 0;
            valid_ = true;
        }

        AOIterator(const AOIterator &) = default;
        AOIterator & operator=(const AOIterator &) = default;
        AOIterator(AOIterator &&) = default;
        AOIterator & operator=(AOIterator &&) = default;

        size_t n_functions(void) const noexcept
        {
            return totalnfunc_;
        } 

        bool valid(void) const noexcept
        {
            return valid_;
        }

        size_t total_idx(void) const noexcept
        {
            return totalidx_;
        }

        template<int N>
        size_t general_idx(void) const noexcept
        {
            return std::get<N>(genidx_);
        }


        size_t general_idx(int n) const ASSERTIONS_ONLY
        {
            
            psr_assert(n < N, "Given index is beyond the number of shells I have"); 
            return genidx_[n];
        }


        /*! \brief Return the index of the current function in current general contraction
         */
        template<int N>
        size_t function_idx(void) const noexcept
        {
            return std::get<N>(funcidx_);
        }


        size_t function_idx(int n) const ASSERTIONS_ONLY
        {
            
            psr_assert(n >= 0 && n < N, "Given index is beyond the number of shells I have");
            return funcidx_[n];
        }

        /*! \brief Return the index of the current function with respect to the beginning of the shell
         */ 
        template<int N>
        size_t shell_function_idx(void) const noexcept
        {
            return std::get<N>(shellfuncidx_);
        }


        size_t shell_function_idx(int n) const ASSERTIONS_ONLY
        {
            
            psr_assert(n >= 0 && n < N, "Given index is beyond the number of shells I have");
            return shellfuncidx_[n];
        }


        bool next(void) noexcept
        {
            if(!valid())
                return false;

            totalidx_++;

            bool iterategen = true;
            for(int i = (N-1); i >= 0; i--)
            {
                const size_t gidx = genidx_[i];
                const size_t gnfunc = nfunc_[i][gidx]; // number of functions in this general contraction

                funcidx_[i]++;
                shellfuncidx_[i]++;

                if(funcidx_[i] < gnfunc)
                {
                    iterategen = false;
                    break;
                }
                else
                {
                    shellfuncidx_[i] -= gnfunc; // go back to the beginning
                    funcidx_[i] = 0;
                }
            }
            if(!iterategen) // we don't have to iterate the general contraction
                return true;


            // if we get here, we have to iterate the general contraction
            for(int i = (N-1); i >= 0; i--)
            {
                const size_t ngen = ngen_[i];
                const size_t gidx = genidx_[i];
                const size_t gnfunc = nfunc_[i][gidx];

                genidx_[i]++;
                if(genidx_[i] < ngen)
                {
                    shellfuncidx_[i] += gnfunc; // represents the previous iteration
                    return true;
                }
                else
                {
                    genidx_[i] = 0;
                    shellfuncidx_[i] = 0;
                }
            }

            // if we get here, we've turned everything back to zero
            valid_ = false;
            totalidx_ = 0;
            return false;
        }


    private:
        
        std::array<size_t, N> ngen_;               //! Number of general contractions in each shell
        std::array<std::vector<size_t>, N> nfunc_; //! Number of functions in each general contraction in each shell.
                                                   //  Each nfunc_[i] vector should be of length ngen_[i]


        std::array<size_t, N> genidx_;   // current index of the general contractions
        std::array<size_t, N> funcidx_;  // current index of the function
        std::array<size_t, N> shellfuncidx_;  // current index of the function, w.r.t. the beginning of the shell
        size_t totalidx_;             // Total index in this collection
        size_t totalnfunc_;           // Total number of functions in this combination
        bool valid_;
};



} // close namespace pulsar

#endif
