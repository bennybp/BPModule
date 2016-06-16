/*! \file
 *
 * \brief Integrals that deal only with System (header)
 * \author Ben Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MODULEBASE__SYSTEMINTEGRAL_HPP_
#define PULSAR_GUARD_MODULEBASE__SYSTEMINTEGRAL_HPP_

#include "pulsar/modulebase/ModuleBase.hpp"


namespace pulsar{
namespace modulebase {

/*! \brief One-electron integral implementation
 *
 */
class SystemIntegral : public ModuleBase
{
    public:
        typedef SystemIntegral BaseType;

        SystemIntegral(ID_t id)
            : ModuleBase(id, "SystemIntegral")
        { }


        /*! \brief Initialize the integral computation
         *
         * \param [in] deriv Derivative to calculate
         * \param [in] sys System for which we are calculating the integrals
         */
        void Initialize(unsigned int deriv, const system::System & sys)
        {
            return ModuleBase::FastCallFunction(&SystemIntegral::Initialize_,
                                                deriv, sys);
        }


        /*! \brief Calculate an integral
         *
         * \param [in] deriv Derivative to calculate
         * \param [in] outbuffer Where to place the completed integrals
         * \param [in] bufsize Size of \p outbuffer (as the number of doubles)
         * \return Number of integrals calculated
         */
        uint64_t Calculate(double * outbuffer, size_t bufsize)
        {
            return ModuleBase::FastCallFunction(&SystemIntegral::Calculate_,
                                                outbuffer, bufsize);
        }


        /*! \brief Calculate an integral (for use from python)
         *
         * \param [in] deriv Derivative to calculate
         * \param [in] shell1 Shell index on the first center
         * \param [in] shell2 Shell index on the second center
         * \param [in] outbuffer Where to place the completed integrals
         * \return Number of integrals calculated
         */
        uint64_t CalculatePy(pybind11::buffer outbuffer)
        {
            auto ptrinfo = PythonBufferToPtr<double>(outbuffer, 1);

            return ModuleBase::FastCallFunction(&SystemIntegral::Calculate_,
                                                ptrinfo.first, ptrinfo.second[0]);
        }


        /////////////////////////////////////////
        // To be implemented by derived classes
        /////////////////////////////////////////

        //! \copydoc Initialize
        virtual void Initialize_(unsigned int deriv, const system::System & sys) = 0;

        //! \copydoc Calculate
        virtual uint64_t Calculate_(double * outbuffer, size_t bufsize) = 0;
};


class SystemIntegral_Py : public SystemIntegral
{
    public:
        using SystemIntegral::SystemIntegral;

        MODULEBASE_FORWARD_PROTECTED_TO_PY

        virtual void Initialize_(unsigned int deriv, const system::System & sys)
        {
            return CallPyOverride<void>("Initialize_", deriv, sys);
        }

        virtual uint64_t Calculate_(double * outbuffer, size_t bufsize)
        {
            //! \todo untested, won't work
            pybind11::buffer_info buf(outbuffer,
                                      sizeof(double),
                                      pybind11::format_descriptor<double>::value,
                                      1, { bufsize },
                                      { sizeof(double) });

            return CallPyOverride<uint64_t>("Calculate_", buf, bufsize);
        }
};

} // close namespace modulebase
} // close namespace pulsar

#endif
