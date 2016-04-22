/*! \file
 *
 * \brief Two-electron integral implementation (header)
 * \author Ben Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_MODULEBASE__ONEELECTRONINTEGRALIMPL_HPP_
#define PULSAR_GUARD_MODULEBASE__ONEELECTRONINTEGRALIMPL_HPP_

#include "pulsar/modulebase/ModuleBase.hpp"


namespace pulsar{
namespace modulebase {

/*! \brief One-electron integral implementation
 *
 */
class OneElectronIntegralIMPL : public ModuleBase
{
    public:
        typedef OneElectronIntegralIMPL BaseType;

        OneElectronIntegralIMPL(ID_t id)
            : ModuleBase(id, "OneElectronIntegralIMPL")
        { }


        /*! \brief Set the basis sets for the integrals
         * 
         * \param [in] bs1 Basis set tag to use on the first center
         * \param [in] bs1 Basis set tag to use on the second center
         */
        void SetBases(const std::string & bs1, const std::string & bs2)
        {
            return ModuleBase::CallFunction(&OneElectronIntegralIMPL::SetBases_, bs1, bs2);
        }


        /*! \brief Calculate an integral
         *
         * \param [in] deriv Derivative to calculate
         * \param [in] shell1 Shell index on the first center
         * \param [in] shell2 Shell index on the second center
         * \return Number of integrals calculated
         */
        uint64_t Calculate(uint64_t deriv, uint64_t shell1, uint64_t shell2,
                           double * outbuffer, size_t bufsize)
        {
            return ModuleBase::FastCallFunction(&OneElectronIntegralIMPL::Calculate_,
                                                deriv, shell1, shell2, outbuffer, bufsize);
        }

        /*! \brief Calculate multiple integrals
         *
         * \param [in] deriv Derivative to calculate
         * \param [in] shells1 Shell indices on the first center
         * \param [in] shells2 Shell indices on the second center
         * \return Number of integrals calculated
         */
        virtual uint64_t CalculateMulti(uint64_t deriv,
                                        const std::vector<uint64_t> & shells1,
                                        const std::vector<uint64_t> & shells2,
                                        double * outbuffer, size_t bufsize)
        {
            return ModuleBase::FastCallFunction(&OneElectronIntegralIMPL::CalculateMulti_,
                                                deriv, shells1, shells2, outbuffer, bufsize);
        }

        /////////////////////////////////////////
        // To be implemented by derived classes
        /////////////////////////////////////////
        //! \copydoc SetBases
        virtual void SetBases_(const std::string & bs1, const std::string & bs2) = 0;


        //! \copydoc Calculate
        virtual uint64_t Calculate_(uint64_t deriv, uint64_t shell1, uint64_t shell2,
                                    double * outbuffer, size_t bufsize) = 0;

        //! \copydoc CalculateMulti
        virtual uint64_t CalculateMulti_(uint64_t deriv,
                                         const std::vector<uint64_t> & shells1, 
                                         const std::vector<uint64_t> & shells2,
                                         double * outbuffer, size_t bufsize) = 0;
};


class OneElectronIntegralIMPL_Py : public OneElectronIntegralIMPL
{
    public:
        using OneElectronIntegralIMPL::OneElectronIntegralIMPL;

        MODULEBASE_FORWARD_PROTECTED_TO_PY
    
        virtual void SetBases_(const std::string & bs1, const std::string & bs2)

        {
            return CallPyOverride<void>("SetBases_", bs1, bs2);
        }


        virtual uint64_t Calculate_(uint64_t deriv, uint64_t shell1, uint64_t shell2,
                                    double * outbuffer, size_t bufsize)
        {
            pybind11::buffer_info buf(outbuffer,
                                      sizeof(double),
                                      pybind11::format_descriptor<double>::value(),
                                      1, { bufsize },
                                      { sizeof(double) });

            return CallPyOverride<uint64_t>("Calculate_", deriv, shell1, buf, bufsize);
        }


        virtual uint64_t CalculateMulti_(uint64_t deriv,
                                         const std::vector<uint64_t> & shells1, 
                                         const std::vector<uint64_t> & shells2,
                                         double * outbuffer, size_t bufsize) 
        {
            pybind11::buffer_info buf(outbuffer,
                                      sizeof(double),
                                      pybind11::format_descriptor<double>::value(),
                                      1, { bufsize },
                                      { sizeof(double) });

            return CallPyOverride<uint64_t>("CalculateMulti_", deriv, shells1, shells2, buf, bufsize);
        }
};

} // close namespace modulebase
} // close namespace pulsar

#endif
