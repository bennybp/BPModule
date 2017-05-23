/*! \file
 *
 * \brief One-electron integral implementation (header)
 * \author Ben Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MODULEBASE__ONEELECTRONINTEGRAL_HPP_
#define PULSAR_GUARD_MODULEBASE__ONEELECTRONINTEGRAL_HPP_

#include "pulsar/modulebase/ModuleBase.hpp"
#include "pulsar/system/BasisSet.hpp"
#include "pulsar/util/PythonIntegralHelper.hpp"

namespace pulsar{

/*! \brief One-electron integral implementation
 */
class TwoCenterIntegral : public ModuleBase
{
protected:
    PythonIntegralHelper helper_;
public:
        typedef TwoCenterIntegral BaseType;
        typedef std::string HashType;

        TwoCenterIntegral(ID_t id)
            : ModuleBase(id, "TwoCenterIntegral"), initialized_(false)
        { }


        /*! \brief initialize the integral computation
         *
         * \param [in] deriv Derivative to calculate
         * \param [in] wfn Wavefunction to use to calculate integrals
         * \param [in] bs1 Basis set tag to use on the first center
         * \param [in] bs2 Basis set tag to use on the second center
         */
        void initialize(unsigned int deriv,
                        const Wavefunction & wfn,
                        const BasisSet & bs1,
                        const BasisSet & bs2)
        {
            ModuleBase::call_function(&TwoCenterIntegral::uninitialized_or_throw_);
            ModuleBase::call_function(&TwoCenterIntegral::initialize_, deriv, wfn, bs1, bs2);
            helper_.initialize(bs1,bs2);
            initialized_ = true; 
        }

        HashType my_hash(unsigned int deriv,
                         const Wavefunction & wfn,
                         const BasisSet & bs1,
                         const BasisSet & bs2)
        {
            return ModuleBase::call_function(&TwoCenterIntegral::my_hash_, deriv, wfn, bs1, bs2);
        }

        /*! Return the number of components calculated by this module
         *
         * For example, something that calculates x,y,z component would return 3
         */
        unsigned int n_components(void) const
        {
            return ModuleBase::call_function(&TwoCenterIntegral::n_components_);
        }


        /*! \brief calculate an integral
         *
         * \param [in] shell1 Shell index on the first center
         * \param [in] shell2 Shell index on the second center
         * \param [in] outbuffer Where to place the completed integrals
         * \param [in] bufsize Size of \p outbuffer (as the number of doubles)
         * \return Number of integrals calculated
         */
        const double* calculate(uint64_t shell1, uint64_t shell2)
        {
            ModuleBase::call_function(&TwoCenterIntegral::initialized_or_throw_);

            return ModuleBase::call_function(&TwoCenterIntegral::calculate_,
                                              shell1, shell2);
        }

        /*! \brief calculate multiple integrals
         *
         * \param [in] shells1 Shell indices on the first center
         * \param [in] shells2 Shell indices on the second center
         * \param [in] outbuffer Where to place the completed integrals
         * \param [in] bufsize Size of \p outbuffer (as the number of doubles)
         * \return Number of integrals calculated
         */
        const double* calculate_multi(const std::vector<uint64_t> & shells1,
                                 const std::vector<uint64_t> & shells2)
        {
            ModuleBase::call_function(&TwoCenterIntegral::initialized_or_throw_);

            return ModuleBase::call_function(&TwoCenterIntegral::calculate_multi_,
                                                  shells1, shells2);
        }


        /*! \brief calculate an integral (for use from python)
         *
         * \param [in] shell1 Shell index on the first center
         * \param [in] shell2 Shell index on the second center
         * \return Integrals in a Python list
         */
        pybind11::list calculate_py(uint64_t shell1, uint64_t shell2)
        {
               const double* ints =
                ModuleBase::call_function(&TwoCenterIntegral::calculate_,
                                              shell1, shell2);
               return helper_.int_2_py(ints,shell1,shell2);
        }


        /*! \brief calculate multiple integrals (for use from python)
         *
         * \param [in] shells1 Shell indicies on the first center
         * \param [in] shells2 Shell indicies on the second center
         * \return The integrals in a Python list
         */
        pybind11::list calculate_multi_py(const std::vector<uint64_t> & shells1,
                                    const std::vector<uint64_t> & shells2)
        {
            const double* ints=
              ModuleBase::call_function(&TwoCenterIntegral::calculate_multi_,
                                           shells1, shells2);
            return helper_.multi_int_2_py(ints,shells1,shells2);
        }



        /////////////////////////////////////////
        // To be implemented by derived classes
        /////////////////////////////////////////
        //! \copydoc initialize
        virtual void initialize_(unsigned int deriv,
                                 const Wavefunction & wfn,
                                 const BasisSet & bs1,
                                 const BasisSet & bs2) = 0;

        virtual HashType my_hash_(unsigned int deriv,
                                  const Wavefunction& wfn,
                                  const BasisSet & bs1,
                                  const BasisSet & bs2)=0;

        //! \copydoc n_components
        virtual unsigned int n_components_(void) const
        {
            return 1;
        }

        //! \copydoc calculate
        virtual const double* calculate_(uint64_t shell1, uint64_t shell2) = 0;

        //! \copydoc calculate_multi
        virtual const double* calculate_multi_(const std::vector<uint64_t> & /*shells1*/,
                                          const std::vector<uint64_t> & /*shells2*/)
        {
//            //////////////////////////////////////////////////////////
//            // default implementation - just loop over and do them all
//            //////////////////////////////////////////////////////////

//            uint64_t ntotal = 0;

//            for(uint64_t s1 : shells1)
//            for(uint64_t s2 : shells2)
//            for(uint64_t s3 : shells3)
//            for(uint64_t s4 : shells4)
//            {
//                uint64_t nbatch = calculate_(s1, s2, s3, s4);
//                ntotal += nbatch;
//                outbuffer += nbatch;

//                // be safe with unsigned types
//                bufsize = ( (nbatch >= bufsize) ? 0 : (bufsize - nbatch) );
//            }

            return nullptr;
        }


    private:
        bool initialized_; //!< Has initialize() been called


        void initialized_or_throw_(void) const
        {
            if(!initialized_)
                throw PulsarException("Module is not yet initialized");
        }

        void uninitialized_or_throw_(void) const
        {
            if(initialized_)
                throw PulsarException("Module has already been initialized");
        }

};


class TwoCenterIntegral_Py : public TwoCenterIntegral
{
    public:
        using TwoCenterIntegral::TwoCenterIntegral;

        MODULEBASE_FORWARD_PROTECTED_TO_PY

        virtual void initialize_(unsigned int deriv,
                                 const Wavefunction & wfn,
                                 const BasisSet & bs1,
                                 const BasisSet & bs2)

        {
            return call_py_override<void>(this, "initialize_", deriv, wfn, bs1, bs2);
        }

        virtual HashType my_hash_(unsigned int deriv,
                                  const Wavefunction &wfn,
                                  const BasisSet &bs1,
                                  const BasisSet &bs2)
        {
            return call_py_override<HashType>(this,"my_hash_", deriv, wfn,bs1, bs2);
        }

        virtual unsigned int n_components_(void) const
        {
            if(has_py_override(this, "n_components_"))
                return call_py_override<unsigned int>(this, "n_components_");
            else
                return TwoCenterIntegral::n_components_();
        }


        virtual const double* calculate_(uint64_t shell1, uint64_t shell2)
        {
                pybind11::list ints =
                    call_py_override<pybind11::list>(this,"calculate_",shell1,shell2);

                return helper_.py_2_int(ints);
        }


        virtual const double* calculate_multi_(const std::vector<uint64_t> & shells1,
                                          const std::vector<uint64_t> & shells2)
        {

              if(has_py_override<TwoCenterIntegral>(this, "calculate_multi_"))
              {
                  pybind11::list ints=
                          call_py_override<pybind11::list>(this,"calculate_multi_",shells1,shells2);
                  return helper_.py_2_int(ints);
              }

              return TwoCenterIntegral::calculate_multi_(shells1, shells2);
        }
};

} // close namespace pulsar

#endif
