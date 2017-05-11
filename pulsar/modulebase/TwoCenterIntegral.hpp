/*! \file
 *
 * \brief One-electron integral implementation (header)
 * \author Ben Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MODULEBASE__ONEELECTRONINTEGRAL_HPP_
#define PULSAR_GUARD_MODULEBASE__ONEELECTRONINTEGRAL_HPP_

#include "pulsar/modulebase/ModuleBase.hpp"
#include "pulsar/system/BasisSet.hpp"

namespace pulsar{

/*! \brief One-electron integral implementation
 */
class TwoCenterIntegral : public ModuleBase
{
    public:
        typedef TwoCenterIntegral BaseType;

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
            initialized_ = true; 
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


        /*! \brief calculate an integral (for use from python)
         *
         * \param [in] shell1 Shell index on the first center
         * \param [in] shell2 Shell index on the second center
         * \return the completed integrals in a python buffer
         *
         * \warning This segfaults if I go above 10,000
         */
        pybind11::memoryview calculate_py(uint64_t shell1, uint64_t shell2)
        {
            const double* ints = ModuleBase::call_function(&TwoCenterIntegral::calculate_,
                                                                shell1, shell2);
            return memoryview_cast(ints,10000);
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


        /*! \brief calculate multiple integrals (for use from python)
         *
         * \param [in] shells1 Shell indicies on the first center
         * \param [in] shells2 Shell indicies on the second center
         * \param [in] outbuffer Where to place the completed integrals
         * \return Number of integrals calculated
         */
        uint64_t calculate_multi_py(const std::vector<uint64_t> & shells1,
                                    const std::vector<uint64_t> & shells2)
        {
//            auto ptrinfo = python_buffer_to_ptr<double>(outbuffer, 1);

//            return ModuleBase::call_function(&OneElectronIntegral::calculate_multi_,
//                                                  shells1, shells2,
//                                                  ptrinfo.first, ptrinfo.second[0]);
        }



        /////////////////////////////////////////
        // To be implemented by derived classes
        /////////////////////////////////////////
        //! \copydoc initialize
        virtual void initialize_(unsigned int deriv,
                                 const Wavefunction & wfn,
                                 const BasisSet & bs1,
                                 const BasisSet & bs2) = 0;


        //! \copydoc n_components
        virtual unsigned int n_components_(void) const
        {
            return 1;
        }

        //! \copydoc calculate
        virtual const double* calculate_(uint64_t shell1, uint64_t shell2) = 0;

        //! \copydoc calculate_multi
        virtual const double* calculate_multi_(const std::vector<uint64_t> & shells1,
                                          const std::vector<uint64_t> & shells2)
        {
//            //////////////////////////////////////////////////////////
//            // default implementation - just loop over and do them all
//            //////////////////////////////////////////////////////////

//            uint64_t ntotal = 0;

//            for(uint64_t s1 : shells1)
//            for(uint64_t s2 : shells2)
//            {
//                uint64_t nbatch = calculate_(s1, s2, outbuffer, bufsize);
//                ntotal += nbatch;
//                outbuffer += nbatch;

//                // be safe with unsigned types
//                bufsize = ( (nbatch >= bufsize) ? 0 : (bufsize - nbatch) );
//            }

//            return ntotal;
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


        virtual unsigned int n_components_(void) const
        {
            if(has_py_override(this, "n_components_"))
                return call_py_override<unsigned int>(this, "n_components_");
            else
                return TwoCenterIntegral::n_components_();
        }


        virtual const double* calculate_(uint64_t shell1, uint64_t shell2)
        {
            pybind11::buffer_info info =
                call_py_override<pybind11::buffer>(this,"calculate_",shell1,shell2).request();

            return static_cast<const double*>(info.ptr);
        }


        virtual const double* calculate_multi_(const std::vector<uint64_t> & shells1,
                                          const std::vector<uint64_t> & shells2)
        {
//            if(has_py_override(this, "calculate_multi_"))
//            {
//                //! \todo untested

//                pybind11::buffer_info pybuf(outbuffer,
//                                            sizeof(double),
//                                            pybind11::format_descriptor<double>::format(),
//                                            1, { bufsize },
//                                            { sizeof(double) });

//                return call_py_override<uint64_t>(this, "calculate_multi_",
//                                                shells1, shells2, pybuf, bufsize);
//            }
//            else
//                return OneElectronIntegral::calculate_multi_(shells1, shells2,
//                                                             outbuffer, bufsize);
        }
};

} // close namespace pulsar

#endif
