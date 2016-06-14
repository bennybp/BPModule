/*! \file
 *
 * \brief One-electron integral implementation (header)
 * \author Ben Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_MODULEBASE__ONEELECTRONINTEGRAL_HPP_
#define PULSAR_GUARD_MODULEBASE__ONEELECTRONINTEGRAL_HPP_

#include "pulsar/modulebase/ModuleBase.hpp"
#include "pulsar/system/BasisSetShell.hpp"
#include "pulsar/system/SphericalTransform.hpp"

namespace pulsar{
namespace modulebase {

/*! \brief One-electron integral implementation
 *
 */
class OneElectronIntegral : public ModuleBase
{
    public:
        typedef OneElectronIntegral BaseType;

        OneElectronIntegral(ID_t id)
            : ModuleBase(id, "OneElectronIntegral")
        { }


        /*! \brief Initialize the integral computation
         * 
         * \param [in] deriv Derivative to calculate
         * \param [in] wfn Wavefunction to use to calculate integrals
         * \param [in] bs1 Basis set tag to use on the first center
         * \param [in] bs2 Basis set tag to use on the second center
         */
        void Initialize(unsigned int deriv,
                        const datastore::Wavefunction & wfn,
                        const system::BasisSet & bs1,
                        const system::BasisSet & bs2)
        {
            return ModuleBase::CallFunction(&OneElectronIntegral::Initialize_, deriv, wfn, bs1, bs2);
        }



        /*! Return the number of components calculated by this module
         * 
         * For example, something that calculates x,y,z component would return 3
         */
        unsigned int NComponents(void) const 
        {
            return ModuleBase::CallFunction(&OneElectronIntegral::NComponents_);
        }


        /*! \brief Calculate an integral
         *
         * \param [in] shell1 Shell index on the first center
         * \param [in] shell2 Shell index on the second center
         * \param [in] outbuffer Where to place the completed integrals
         * \param [in] bufsize Size of \p outbuffer (as the number of doubles)
         * \return Number of integrals calculated
         */
        uint64_t Calculate(uint64_t shell1, uint64_t shell2,
                           double * outbuffer, size_t bufsize)
        {
            return ModuleBase::FastCallFunction(&OneElectronIntegral::Calculate_,
                                                shell1, shell2, outbuffer, bufsize);
        }


        /*! \brief Calculate an integral (for use from python)
         *
         * \param [in] shell1 Shell index on the first center
         * \param [in] shell2 Shell index on the second center
         * \param [in] outbuffer Where to place the completed integrals
         * \return Number of integrals calculated
         */
        uint64_t CalculatePy(uint64_t shell1, uint64_t shell2,
                             pybind11::buffer outbuffer)
        {
            auto ptrinfo = PythonBufferToPtr<double>(outbuffer, 1);

            return ModuleBase::FastCallFunction(&OneElectronIntegral::Calculate_,
                                                shell1, shell2,
                                                ptrinfo.first, ptrinfo.second[0]);
        }

        /*! \brief Calculate multiple integrals
         *
         * \param [in] shells1 Shell indices on the first center
         * \param [in] shells2 Shell indices on the second center
         * \param [in] outbuffer Where to place the completed integrals
         * \param [in] bufsize Size of \p outbuffer (as the number of doubles)
         * \return Number of integrals calculated
         */
        uint64_t CalculateMulti(const std::vector<uint64_t> & shells1,
                                const std::vector<uint64_t> & shells2,
                                double * outbuffer, size_t bufsize)
        {
            return ModuleBase::FastCallFunction(&OneElectronIntegral::CalculateMulti_,
                                                shells1, shells2, outbuffer, bufsize);
        }


        /*! \brief Calculate multiple integrals (for use from python)
         *
         * \param [in] shells1 Shell indicies on the first center
         * \param [in] shells2 Shell indicies on the second center
         * \param [in] outbuffer Where to place the completed integrals
         * \return Number of integrals calculated
         */
        uint64_t CalculateMultiPy(const std::vector<uint64_t> & shells1,
                                  const std::vector<uint64_t> & shells2,
                                  pybind11::buffer outbuffer)
        {
            auto ptrinfo = PythonBufferToPtr<double>(outbuffer, 1);

            return ModuleBase::FastCallFunction(&OneElectronIntegral::CalculateMulti_,
                                                shells1, shells2,
                                                ptrinfo.first, ptrinfo.second[0]);
        }



        /////////////////////////////////////////
        // To be implemented by derived classes
        /////////////////////////////////////////
        //! \copydoc Initialize
        virtual void Initialize_(unsigned int deriv,
                                 const datastore::Wavefunction & wfn,
                                 const system::BasisSet & bs1,
                                 const system::BasisSet & bs2) = 0;


        //! \copydoc NComponents
        virtual unsigned int NComponents_(void) const 
        {
            return 1;
        }

        //! \copydoc Calculate
        virtual uint64_t Calculate_(uint64_t shell1, uint64_t shell2,
                                    double * outbuffer, size_t bufsize) = 0;

        //! \copydoc CalculateMulti
        virtual uint64_t CalculateMulti_(const std::vector<uint64_t> & shells1, 
                                         const std::vector<uint64_t> & shells2,
                                         double * outbuffer, size_t bufsize)
        {
            //////////////////////////////////////////////////////////
            // default implementation - just loop over and do them all
            //////////////////////////////////////////////////////////

            uint64_t ntotal = 0;

            for(uint64_t s1 : shells1)
            for(uint64_t s2 : shells2)
            {
                uint64_t nbatch = Calculate_(s1, s2, outbuffer, bufsize);
                ntotal += nbatch;
                outbuffer += nbatch;

                // be safe with unsigned types
                bufsize = ( (nbatch >= bufsize) ? 0 : (bufsize - nbatch) );
            }

            return ntotal;
        }
};


class OneElectronIntegral_Py : public OneElectronIntegral
{
    public:
        using OneElectronIntegral::OneElectronIntegral;

        MODULEBASE_FORWARD_PROTECTED_TO_PY
    
        virtual void Initialize_(unsigned int deriv,
                                 const datastore::Wavefunction & wfn,
                                 const system::BasisSet & bs1,
                                 const system::BasisSet & bs2)

        {
            return CallPyOverride<void>("Initialize_", deriv, wfn, bs1, bs2);
        }


        virtual unsigned int NComponents_(void) const 
        {
            if(HasPyOverride("NComponents_"))
                return CallPyOverride<unsigned int>("NComponents_");
            else
                return OneElectronIntegral::NComponents_();
        }


        virtual uint64_t Calculate_(uint64_t shell1, uint64_t shell2,
                                    double * outbuffer, size_t bufsize)
        {
            //! \todo untested

            pybind11::buffer_info buf(outbuffer,
                                      sizeof(double),
                                      pybind11::format_descriptor<double>::value,
                                      1, { bufsize },
                                      { sizeof(double) });

            return CallPyOverride<uint64_t>("Calculate_", shell1, shell2, buf, bufsize);
        }


        virtual uint64_t CalculateMulti_(const std::vector<uint64_t> & shells1, 
                                         const std::vector<uint64_t> & shells2,
                                         double * outbuffer, size_t bufsize) 
        {
            if(HasPyOverride("CalculateMulti_"))
            {
                //! \todo untested

                pybind11::buffer_info pybuf(outbuffer,
                                            sizeof(double),
                                            pybind11::format_descriptor<double>::value,
                                            1, { bufsize },
                                            { sizeof(double) });

                return CallPyOverride<uint64_t>("CalculateMulti_",
                                                shells1, shells2, pybuf, bufsize);
            }
            else
                return OneElectronIntegral::CalculateMulti_(shells1, shells2,
                                                            outbuffer, bufsize);
        }
};

} // close namespace modulebase
} // close namespace pulsar

#endif
