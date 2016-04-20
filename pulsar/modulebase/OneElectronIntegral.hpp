/*! \file
 *
 * \brief Two-electron integral implementation (header)
 * \author Ben Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_MODULEBASE__ONEELECTRONINTEGRAL_HPP_
#define PULSAR_GUARD_MODULEBASE__ONEELECTRONINTEGRAL_HPP_

#include "pulsar/modulebase/ModuleBase.hpp"


namespace pulsar{
namespace modulebase {

/*! \brief One-electron integral builder
 *
 */
class OneElectronIntegral : public ModuleBase
{
    public:
        typedef OneElectronIntegral BaseType;

        OneElectronIntegral(ID_t id)
            : ModuleBase(id, "OneElectronIntegral")
        { }


        /*! \brief Set the basis sets for the integrals
         * 
         * \param [in] bs1 Basis set on the first center
         * \param [in] bs2 Basis set on the second center
         */
        void SetBases(std::shared_ptr<const system::BasisSet> & bs1,
                      std::shared_ptr<const system::BasisSet> & bs2)
        {
            return ModuleBase::CallFunction(&OneElectronIntegral::SetBases_, bs1, bs2);
        }


        /*! \brief Calculate an integral
         *
         * \param [in] deriv Derivative to calculate
         * \param [in] shell1 Shell index on the first center
         * \param [in] shell2 Shell index on the second center
         * \return Number of integrals calculated
         */
        long Calculate(int deriv, int shell1, int shell2)
        {
            return ModuleBase::CallFunction(&OneElectronIntegral::Calculate_, deriv, 
                                                                                  shell1,
                                                                                  shell2);
        }

        /*! \brief Obtain the buffer to the stored integrals */
        const double * GetBuf(void)
        {
            return ModuleBase::CallFunction(&OneElectronIntegral::GetBuf_);
        }


        /*! \brief Obtain how many integrals were last calculated */
        long GetIntegralCount(void)
        {
            return ModuleBase::CallFunction(&OneElectronIntegral::GetIntegralCount_);
        }


        /*! \brief Obtain a copy of the buffer of stored integrals (for python) */
        pybind11::object GetBufPy(void)
        {
            return python::ConvertToPy(GetBuf(), GetIntegralCount());  
        }



        /////////////////////////////////////////
        // To be implemented by derived classes
        /////////////////////////////////////////
        //! \copydoc SetBases
        virtual void SetBases_(std::shared_ptr<const system::BasisSet> & bs1,
                               std::shared_ptr<const system::BasisSet> & bs2) = 0;


        //! \copydoc Calculate
        virtual long Calculate_(int deriv, int shell1, int shell2) = 0;


        //! \copydoc GetBuf
        virtual const double * GetBuf_(void) = 0;


        //! \copydoc GetIntegralCount
        virtual long GetIntegralCount_(void) = 0;
        
};


class OneElectronIntegral_Py : public OneElectronIntegral
{
    public:
        using OneElectronIntegral::OneElectronIntegral;

        MODULEBASE_FORWARD_PROTECTED_TO_PY
    
        virtual void SetBases_(std::shared_ptr<const system::BasisSet> & bs1,
                               std::shared_ptr<const system::BasisSet> & bs2)

        {
            return CallPyOverride<void>("SetBases_", bs1, bs2);
        }


        virtual long Calculate_(int deriv, int shell1, int shell2)
        {
            return CallPyOverride<long>("Calculate_", deriv, shell1, shell2);
        }


        virtual const double * GetBuf_(void)
        {
            return CallPyOverride<double *>("GetBuf_");
        }


        virtual long GetIntegralCount_(void)
        {
            return CallPyOverride<long>("GetIntegralCount_");
        }

};

} // close namespace modulebase
} // close namespace pulsar

#endif
