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

/*! \brief Integrals that depend only on the system
 */
class SystemIntegral : public ModuleBase
{
    public:
        typedef SystemIntegral BaseType;

        SystemIntegral(ID_t id)
            : ModuleBase(id, "SystemIntegral")
        { }


        /*! \brief Calculate an integral
         *
         * \param [in] deriv Derivative to calculate
         * \return Number of integrals calculated
         */
        uint64_t Calculate(uint64_t deriv)
        {
            return ModuleBase::FastCallFunction(&SystemIntegral::Calculate_,
                                                deriv, shell1, shell2);
        }

        /*! \brief Obtain the buffer to the stored integrals */
        const double * GetBuf(void)
        {
            return ModuleBase::CallFunction(&SystemIntegral::GetBuf_);
        }


        /*! \brief Obtain how many integrals were last calculated */
        uint64_t GetCount(void)
        {
            return ModuleBase::CallFunction(&SystemIntegral::GetCount_);
        }


        /////////////////////////////////////////
        // To be implemented by derived classes
        /////////////////////////////////////////
        //! \copydoc SetBases
        virtual void SetBases_(const std::string & bs1, const std::string & bs2) = 0;


        //! \copydoc Calculate
        virtual uint64_t Calculate_(uint64_t deriv, uint64_t shell1, uint64_t shell2) = 0;

        //! \copydoc CalculateMulti
        virtual uint64_t CalculateMulti_(uint64_t deriv,
                                         const std::vector<uint64_t> & shells1, 
                                         const std::vector<uint64_t> & shells2) = 0;

        //! \copydoc GetBuf
        virtual const double * GetBuf_(void) = 0;


        //! \copydoc GetCount
        virtual uint64_t GetCount_(void) = 0;
        
};


class SystemIntegral_Py : public SystemIntegral
{
    public:
        using SystemIntegral::SystemIntegral;

        MODULEBASE_FORWARD_PROTECTED_TO_PY
    
        virtual void SetBases_(const std::string & bs1, const std::string & bs2)

        {
            return CallPyOverride<void>("SetBases_", bs1, bs2);
        }


        virtual uint64_t Calculate_(uint64_t deriv, uint64_t shell1, uint64_t shell2)
        {
            return CallPyOverride<uint64_t>("Calculate_", deriv, shell1, shell2);
        }

        virtual uint64_t CalculateMulti_(uint64_t deriv,
                                         const std::vector<uint64_t> & shells1, 
                                         const std::vector<uint64_t> & shells2) 
        {
            return CallPyOverride<uint64_t>("CalculateMulti_", deriv, shells1, shells2);
        }

        virtual const double * GetBuf_(void)
        {
            return CallPyOverride<double *>("GetBuf_");
        }


        virtual uint64_t GetCount_(void)
        {
            return CallPyOverride<uint64_t>("GetCount_");
        }

};

} // close namespace modulebase
} // close namespace pulsar

#endif
