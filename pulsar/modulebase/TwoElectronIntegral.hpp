/*! \file
 *
 * \brief Two-electron integral implementation (header)
 * \author Ben Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MODULEBASE__TWOELECTRONINTEGRAL_HPP_
#define PULSAR_GUARD_MODULEBASE__TWOELECTRONINTEGRAL_HPP_

#include "pulsar/modulebase/ModuleBase.hpp"
#include "pulsar/system/BasisSet.hpp"


namespace pulsar{
namespace modulebase {

/*! \brief Two-electron integral implementation
 *
 */
class TwoElectronIntegral : public ModuleBase
{
    public:
        typedef TwoElectronIntegral BaseType;

        TwoElectronIntegral(ID_t id)
            : ModuleBase(id, "TwoElectronIntegral"), initialized_(false)
        { }


        /*! \brief initialize the integral computation
         *
         * \param [in] deriv Derivative to calculate
         * \param [in] wfn Wavefunction to use to calculate integrals
         * \param [in] bs1 Basis set tag to use for the first center
         * \param [in] bs2 Basis set tag to use for the second center
         * \param [in] bs3 Basis set tag to use for the third center
         * \param [in] bs4 Basis set tag to use for the fourth center
         */
        void initialize(unsigned int deriv,
                        const datastore::Wavefunction & wfn,
                        const system::BasisSet & bs1,
                        const system::BasisSet & bs2,
                        const system::BasisSet & bs3,
                        const system::BasisSet & bs4)
        {
            ModuleBase::fast_call_function(&TwoElectronIntegral::uninitialized_or_throw_);
            ModuleBase::call_function(&TwoElectronIntegral::initialize_, deriv, wfn, bs1, bs2, bs3, bs4);
            initialized_ = true;
        }


        /*! Return the number of components calculated by this module
         *
         * For example, something that calculates x,y,z component would return 3
         */
        unsigned int n_components(void) const
        {
            return ModuleBase::call_function(&TwoElectronIntegral::n_components_);
        }


        /*! \brief calculate an integral
         *
         * \param [in] shell1 Shell index on the first center
         * \param [in] shell2 Shell index on the second center
         * \param [in] shell3 Shell index on the third center
         * \param [in] shell4 Shell index on the fourth center
         * \param [in] outbuffer Where to place the completed integrals
         * \param [in] bufsize Size of \p outbuffer (as the number of doubles)
         * \return Number of integrals calculated
         */
        uint64_t calculate(uint64_t shell1, uint64_t shell2,
                           uint64_t shell3, uint64_t shell4,
                           double * outbuffer, size_t bufsize)
        {
            ModuleBase::fast_call_function(&TwoElectronIntegral::initialized_or_throw_);
            return ModuleBase::fast_call_function(&TwoElectronIntegral::calculate_,
                                                shell1, shell2, shell3, shell4,
                                                outbuffer, bufsize);
        }

        /*! \brief calculate an integral (for use from python)
         *
         * \param [in] shell1 Shell index on the first center
         * \param [in] shell2 Shell index on the second center
         * \param [in] shell3 Shell index on the third center
         * \param [in] shell4 Shell index on the fourth center
         * \param [in] outbuffer Where to place the completed integrals
         * \return Number of integrals calculated
         */
        uint64_t calculate_py(uint64_t shell1, uint64_t shell2,
                              uint64_t shell3, uint64_t shell4,
                              pybind11::buffer outbuffer)
        {
            auto ptrinfo = python_buffer_to_ptr<double>(outbuffer, 1);

            return ModuleBase::fast_call_function(&TwoElectronIntegral::calculate_,
                                                shell1, shell2, shell3, shell4,
                                                ptrinfo.first, ptrinfo.second[0]);
        }


        /*! \brief calculate multiple integrals
         *
         * \param [in] shells1 Shell indicies on the first center
         * \param [in] shells2 Shell indicies on the second center
         * \param [in] shells3 Shell indicies on the third center
         * \param [in] shells4 Shell indicies on the fourth center
         * \param [in] outbuffer Where to place the completed integrals
         * \param [in] bufsize Size of \p outbuffer (as the number of doubles)
         * \return Number of integrals calculated
         */
        uint64_t calculate_multi(const std::vector<uint64_t> & shells1,
                                 const std::vector<uint64_t> & shells2,
                                 const std::vector<uint64_t> & shells3,
                                 const std::vector<uint64_t> & shells4,
                                 double * outbuffer, size_t bufsize)
        {
            ModuleBase::fast_call_function(&TwoElectronIntegral::initialized_or_throw_);
            return ModuleBase::fast_call_function(&TwoElectronIntegral::calculate_multi_,
                                                  shells1, shells2, shells3, shells4,
                                                  outbuffer, bufsize);
        }


        /*! \brief calculate multiple integrals (for use from python)
         *
         * \param [in] shells1 Shell indicies on the first center
         * \param [in] shells2 Shell indicies on the second center
         * \param [in] shells3 Shell indicies on the third center
         * \param [in] shells4 Shell indicies on the fourth center
         * \param [in] outbuffer Where to place the completed integrals
         * \return Number of integrals calculated
         */
        uint64_t calculate_multi_py(const std::vector<uint64_t> & shells1,
                                    const std::vector<uint64_t> & shells2,
                                    const std::vector<uint64_t> & shells3,
                                    const std::vector<uint64_t> & shells4,
                                    pybind11::buffer outbuffer)
        {
            auto ptrinfo = python_buffer_to_ptr<double>(outbuffer, 1);

            return ModuleBase::fast_call_function(&TwoElectronIntegral::calculate_multi_,
                                                shells1, shells2, shells3, shells4,
                                                ptrinfo.first, ptrinfo.second[0]);
        }


        /////////////////////////////////////////
        // To be implemented by derived classes
        /////////////////////////////////////////
        //! \copydoc initialize
        virtual void initialize_(unsigned int deriv,
                                 const datastore::Wavefunction & wfn,
                                 const system::BasisSet & bs1,
                                 const system::BasisSet & bs2,
                                 const system::BasisSet & bs3,
                                 const system::BasisSet & bs4) = 0;


        //! \copydoc n_components
        virtual unsigned int n_components_(void) const
        {
            return 1;
        }


        //! \copydoc calculate
        virtual uint64_t calculate_(uint64_t shell1, uint64_t shell2,
                                    uint64_t shell3, uint64_t shell4,
                                    double * outbuffer, size_t bufsize) = 0;

        //! \copydoc calculate_multi
        virtual uint64_t calculate_multi_(const std::vector<uint64_t> & shells1,
                                          const std::vector<uint64_t> & shells2,
                                          const std::vector<uint64_t> & shells3,
                                          const std::vector<uint64_t> & shells4,
                                          double * outbuffer, size_t bufsize)
        {
            //////////////////////////////////////////////////////////
            // default implementation - just loop over and do them all
            //////////////////////////////////////////////////////////

            uint64_t ntotal = 0;

            for(uint64_t s1 : shells1)
            for(uint64_t s2 : shells2)
            for(uint64_t s3 : shells3)
            for(uint64_t s4 : shells4)
            {
                uint64_t nbatch = calculate_(s1, s2, s3, s4, outbuffer, bufsize);
                ntotal += nbatch;
                outbuffer += nbatch;

                // be safe with unsigned types
                bufsize = ( (nbatch >= bufsize) ? 0 : (bufsize - nbatch) );
            }

            return ntotal;
        }



    private:
        bool initialized_; //!< Has initialize() been called


        void initialized_or_throw_(void) const
        {
            if(!initialized_)
                throw exception::GeneralException("Module is not yet initialized");
        }

        void uninitialized_or_throw_(void) const
        {
            if(initialized_)
                throw exception::GeneralException("Module has already been initialized");
        }
};


class TwoElectronIntegral_Py : public TwoElectronIntegral
{
    public:
        using TwoElectronIntegral::TwoElectronIntegral;

        MODULEBASE_FORWARD_PROTECTED_TO_PY

        virtual void initialize_(unsigned int deriv,
                                 const datastore::Wavefunction & wfn,
                                 const system::BasisSet & bs1,
                                 const system::BasisSet & bs2,
                                 const system::BasisSet & bs3,
                                 const system::BasisSet & bs4)

        {
            return call_py_override<void>("initialize_", deriv, wfn, bs1, bs2, bs3, bs4);
        }


        virtual unsigned int n_components_(void) const
        {
            if(has_py_override("n_components_"))
                return call_py_override<unsigned int>("n_components_");
            else
                return TwoElectronIntegral::n_components_();
        }


        virtual uint64_t calculate_(uint64_t shell1, uint64_t shell2,
                                    uint64_t shell3, uint64_t shell4,
                                    double * outbuffer, size_t bufsize)
        {
            //! \todo untested

            pybind11::buffer_info buf(outbuffer,
                                      sizeof(double),
                                      pybind11::format_descriptor<double>::value,
                                      1, { bufsize },
                                      { sizeof(double) });

            return call_py_override<uint64_t>("calculate_", shell1, shell2, shell3, shell4,
                                            buf, bufsize);
        }


        virtual uint64_t calculate_multi_(const std::vector<uint64_t> & shells1,
                                          const std::vector<uint64_t> & shells2,
                                          const std::vector<uint64_t> & shells3,
                                          const std::vector<uint64_t> & shells4,
                                          double * outbuffer, size_t bufsize)
        {

            if(has_py_override("calculate_multi_"))
            {
                //! \todo untested

                pybind11::buffer_info pybuf(outbuffer,
                                            sizeof(double),
                                            pybind11::format_descriptor<double>::value,
                                            1, { bufsize },
                                            { sizeof(double) });

                return call_py_override<uint64_t>("calculate_multi_",
                                                shells1, shells2, shells3, shells4, pybuf, bufsize);
            }
            else
                return TwoElectronIntegral::calculate_multi_(shells1, shells2, shells3, shells4,
                                                                outbuffer, bufsize);
        }

};

} // close namespace modulebase
} // close namespace pulsar

#endif
