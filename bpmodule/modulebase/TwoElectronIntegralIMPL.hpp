/*! \file
 *
 * \brief Two-electron integral implementation (header)
 * \author Ben Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_MODULEBASE__TWOELECTRONINTEGRALIMPL_HPP_
#define BPMODULE_GUARD_MODULEBASE__TWOELECTRONINTEGRALIMPL_HPP_

#include "bpmodule/modulebase/ModuleBase.hpp"


namespace bpmodule {
namespace modulebase {

/*! \brief Two-electron integral implementation
 *
 */
class TwoElectronIntegralIMPL : public ModuleBase
{
    public:
        typedef TwoElectronIntegralIMPL BaseType;

        TwoElectronIntegralIMPL(unsigned long id)
            : ModuleBase(id, "TwoElectronIntegralIMPL")
        { }


        /*! \brief Set the basis sets for the integrals
         * 
         * \param [in] ncenter The number of centers for the integrals (ie, 3-center, 2-center)
         * \param [in] bs1 Basis set on the first center
         * \param [in] bs2 Basis set on the second center
         * \param [in] bs3 Basis set on the third center
         * \param [in] bs4 Basis set on the fourth center
         */
        void SetBases(int ncenter,
                      std::shared_ptr<const system::BasisSet> & bs1,
                      std::shared_ptr<const system::BasisSet> & bs2,
                      std::shared_ptr<const system::BasisSet> & bs3,
                      std::shared_ptr<const system::BasisSet> & bs4)
        {
            return ModuleBase::CallFunction(&TwoElectronIntegralIMPL::SetBases_, ncenter, bs1, bs2, bs3, bs4);
        }


        /*! \brief Calculate an integral
         *
         * \param [in] deriv Derivative to calculate
         * \param [in] shell1 Shell index on the first center
         * \param [in] shell2 Shell index on the second center
         * \param [in] shell3 Shell index on the third center
         * \param [in] shell4 Shell index on the fourth center
         * \return Number of integrals calculated
         */
        long Calculate(int deriv, int shell1, int shell2, int shell3, int shell4)
        {
            return ModuleBase::FastCallFunction(&TwoElectronIntegralIMPL::Calculate_, deriv, 
                                                                                      shell1,
                                                                                      shell2,
                                                                                      shell3,
                                                                                      shell4);
        }


        /*! \brief Obtain the buffer to the stored integrals */
        const double * GetBuf(void)
        {
            return ModuleBase::FastCallFunction(&TwoElectronIntegralIMPL::GetBuf_);
        }


        /*! \brief Obtain how many integrals were last calculated */
        long GetIntegralCount(void)
        {
            return ModuleBase::FastCallFunction(&TwoElectronIntegralIMPL::GetIntegralCount_);
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
        virtual void SetBases_(int ncenter,
                               std::shared_ptr<const system::BasisSet> & bs1,
                               std::shared_ptr<const system::BasisSet> & bs2,
                               std::shared_ptr<const system::BasisSet> & bs3,
                               std::shared_ptr<const system::BasisSet> & bs4) = 0;


        //! \copydoc Calculate
        virtual long Calculate_(int deriv, int shell1, int shell2, int shell3, int shell4) = 0;

        //! \copydoc GetBuf
        virtual const double * GetBuf_(void) = 0;

        //! \copydoc GetIntegralCount
        virtual long GetIntegralCount_(void) = 0;
        
};


class TwoElectronIntegralIMPL_Py : public TwoElectronIntegralIMPL
{
    public:
        using TwoElectronIntegralIMPL::TwoElectronIntegralIMPL;

        MODULEBASE_FORWARD_PROTECTED_TO_PY
    
        virtual void SetBases_(int ncenter,
                               std::shared_ptr<const system::BasisSet> & bs1,
                               std::shared_ptr<const system::BasisSet> & bs2,
                               std::shared_ptr<const system::BasisSet> & bs3,
                               std::shared_ptr<const system::BasisSet> & bs4)

        {
            return CallPyOverride<void>("SetBases_", ncenter, bs1, bs2, bs3, bs4);
        }


        virtual long Calculate_(int deriv, int shell1, int shell2, int shell3, int shell4)
        {
            return CallPyOverride<long>("Calculate_", deriv, shell1, shell2, shell3, shell4);
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
} // close namespace bpmodule

#endif
