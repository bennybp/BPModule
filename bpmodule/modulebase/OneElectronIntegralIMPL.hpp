/*! \file
 *
 * \brief Two-electron integral implementation (header)
 * \author Ben Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_MODULEBASE__ONEELECTRONINTEGRALIMPL_HPP_
#define BPMODULE_GUARD_MODULEBASE__ONEELECTRONINTEGRALIMPL_HPP_

#include "bpmodule/modulebase/ModuleBase.hpp"
#include "bpmodule/basisset/BasisSet.hpp"


namespace bpmodule {
namespace modulebase {

/*! \brief Two-electron integral implementation
 *
 */
class OneElectronIntegralIMPL : public ModuleBase
{
    public:
        typedef OneElectronIntegralIMPL BaseType;

        OneElectronIntegralIMPL(unsigned long id)
            : ModuleBase(id, "OneElectronIntegralIMPL")
        { }


        /*! \brief Set the basis sets for the integrals
         * 
         * \param [in] ncenter The number of centers for the integrals (ie, 3-center, 2-center)
         */
        void SetBases(const datastore::UIDPointer<basisset::BasisSet> & bs1,
                      const datastore::UIDPointer<basisset::BasisSet> & bs2)
        {
            return ModuleBase::CallFunction(&OneElectronIntegralIMPL::SetBases_, bs1, bs2);
        }


        long Calculate(int deriv, int shell1, int shell2)
        {
            return ModuleBase::CallFunction(&OneElectronIntegralIMPL::Calculate_, deriv, 
                                                                                  shell1,
                                                                                  shell2);
        }


        const double * GetBuf(void)
        {
            return ModuleBase::CallFunction(&OneElectronIntegralIMPL::GetBuf_);
        }


        long GetIntegralCount(void)
        {
            return ModuleBase::CallFunction(&OneElectronIntegralIMPL::GetIntegralCount_);
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
                               const datastore::UIDPointer<basisset::BasisSet> & bs2) = 0;


        //! \copydoc Calculate
        virtual long Calculate_(int deriv, int shell1, int shell2) = 0;


        virtual const double * GetBuf_(void) = 0;


        virtual long GetIntegralCount_(void) = 0;
        
};


class OneElectronIntegralIMPL_Py : public OneElectronIntegralIMPL
{
    public:
        using OneElectronIntegralIMPL::OneElectronIntegralIMPL;

        MODULEBASE_FORWARD_PROTECTED_TO_PY
    
        virtual void SetBases_(const datastore::UIDPointer<basisset::BasisSet> & bs1,
                               const datastore::UIDPointer<basisset::BasisSet> & bs2)

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
} // close namespace bpmodule

#endif
