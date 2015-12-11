/*! \file
 *
 * \brief One-electron integral implementation (header)
 * \author Ben Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_ONEELECTRONINTEGRAL_HPP_
#define _GUARD_ONEELECTRONINTEGRAL_HPP_

#include "bpmodule/modulebase/ModuleBase.hpp"


namespace bpmodule {
namespace modulebase {

/*! \brief One-electron integral implementation
 *
 */
class OneElectronIntegral : public ModuleBase
{
    public:
        OneElectronIntegral(unsigned long id)
            : ModuleBase(id)
        { }

        OneElectronIntegral(PyObject * self, unsigned long id)
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


        //! \todo don't know about passing outbuf to python
        long Calculate(int deriv, int shell1, int shell2, double * outbuf)
        {
            return ModuleBase::CallFunction(&OneElectronIntegral::Calculate_, deriv, 
                                                                                  shell1,
                                                                                  shell2,
                                                                                  outbuf);
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
        virtual long Calculate_(int deriv, int shell1, int shell2, double * outbuf)
        {
            return ModuleBase::CallPyMethod<long>("Calculate_", deriv, shell1, shell2, outbuf);
        }
        

    private:
        virtual boost::python::object MoveToPyObject_(std::function<void(modulebase::ModuleBase *)> deleter)
        {
            return ModuleBase::MoveToPyObjectHelper_<OneElectronIntegral>(deleter, this);
        }

};

} // close namespace modulebase
} // close namespace bpmodule

#endif
