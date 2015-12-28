/*! \file
 *
 * \brief One-electron integral implementation (header)
 * \author Ben Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_MODULEBASE__ONEELECTRONINTEGRAL_HPP_
#define BPMODULE_GUARD_MODULEBASE__ONEELECTRONINTEGRAL_HPP_

#include "bpmodule/modulebase/ModuleBase.hpp"


namespace bpmodule {
namespace modulebase {

/*! \brief One-electron integral builder
 *
 */
class OneElectronIntegral : public ModuleBase
{
    public:
        OneElectronIntegral(unsigned long id)
            : ModuleBase(id)
        { }

        OneElectronIntegral(pybind11::object self, unsigned long id)
            : ModuleBase(self, id)
        { }


        /*! \brief Set the basis sets for the integrals
         * 
         * \param [in] ncenter The number of centers for the integrals (ie, 3-center, 2-center)
         */
        void SetBases(const datastore::UIDPointer<basisset::BasisSet> & bs1,
                      const datastore::UIDPointer<basisset::BasisSet> & bs2)
        {
            return ModuleBase::CallFunction(&OneElectronIntegral::SetBases_, bs1, bs2);
        }


        long Calculate(int deriv, int shell1, int shell2)
        {
            return ModuleBase::CallFunction(&OneElectronIntegral::Calculate_, deriv, 
                                                                                  shell1,
                                                                                  shell2);
        }

        const double * GetBuf(void)
        {
            return ModuleBase::CallFunction(&OneElectronIntegral::GetBuf_);
        }


        long GetIntegralCount(void)
        {
            return ModuleBase::CallFunction(&OneElectronIntegral::GetIntegralCount_);
        }


        pybind11::object GetBufPy(void)
        {
            return python::ConvertToPy(GetBuf(), GetIntegralCount());  
        }



        /////////////////////////////////////////
        // To be implemented by derived classes
        /////////////////////////////////////////
        //! \copydoc SetBases
        virtual void SetBases_(const datastore::UIDPointer<basisset::BasisSet> & bs1,
                               const datastore::UIDPointer<basisset::BasisSet> & bs2)
        {
            ModuleBase::CallPyMethod<void>("SetBases_", bs1, bs2);
        }


        //! \copydoc Calculate
        virtual long Calculate_(int deriv, int shell1, int shell2)
        {
            return ModuleBase::CallPyMethod<long>("Calculate_", deriv, shell1, shell2);
        }
        
        virtual const double * GetBuf_(void)
        {
            return ModuleBase::CallPyMethod<const double *>("GetBuf_");
        }


        virtual long GetIntegralCount_(void)
        {
            return ModuleBase::CallPyMethod<long>("GetIntegralCount_");
        }


};

} // close namespace modulebase
} // close namespace bpmodule

#endif
