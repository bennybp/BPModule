/*! \file
 *
 * \brief Two-electron integral implementation (header)
 * \author Ben Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_TWOELECTRONINTEGRALIMPL_HPP_
#define _GUARD_TWOELECTRONINTEGRALIMPL_HPP_

#include "bpmodule/modulebase/ModuleBase.hpp"


namespace bpmodule {
namespace modulebase {

/*! \brief Two-electron integral implementation new module
 *
 */
class TwoElectronIntegralIMPL : public ModuleBase
{
    public:
        TwoElectronIntegralIMPL(unsigned long id)
            : ModuleBase(id)
        { }

        TwoElectronIntegralIMPL(PyObject * self, unsigned long id)
            : ModuleBase(self, id)
        { }


        /*! \brief Set the basis sets for the integrals
         * 
         * \param [in] ncenter The number of centers for the integrals (ie, 3-center, 2-center)
         */
        void SetBases(int ncenter,
                      const datastore::UIDPointer<basisset::BasisSet> & bs1,
                      const datastore::UIDPointer<basisset::BasisSet> & bs2,
                      const datastore::UIDPointer<basisset::BasisSet> & bs3,
                      const datastore::UIDPointer<basisset::BasisSet> & bs4)
        {
            return ModuleBase::CallFunction(&TwoElectronIntegralIMPL::SetBases_, ncenter, bs1, bs2, bs3, bs4);
        }


        //! \todo don't know about passing outbuf to python
        long Calculate(int deriv, int shell1, int shell2, int shell3, int shell4, double * outbuf)
        {
            return ModuleBase::CallFunction(&TwoElectronIntegralIMPL::Calculate_, deriv, 
                                                                                  shell1,
                                                                                  shell2,
                                                                                  shell3,
                                                                                  shell4,
                                                                                  outbuf);
        }


        //! \todo mostly for testing?
        boost::python::object CalculatePy(int deriv, int shell1, int shell2, int shell3, int shell4)
        {
            static std::vector<double> out(30000); // 1st derivative of ffff?
            long r = ModuleBase::CallFunction(&TwoElectronIntegralIMPL::Calculate_, deriv, 
                                                                                    shell1,
                                                                                    shell2,
                                                                                    shell3,
                                                                                    shell4,
                                                                                    out.data());

            return python_helper::ConvertToPy(std::vector<double>(out.begin(), out.begin()+r));
        }



        /////////////////////////////////////////
        // To be implemented by derived classes
        /////////////////////////////////////////
        //! \copydoc SetBases
        virtual void SetBases_(int ncenter,
                               const datastore::UIDPointer<basisset::BasisSet> & bs1,
                               const datastore::UIDPointer<basisset::BasisSet> & bs2,
                               const datastore::UIDPointer<basisset::BasisSet> & bs3,
                               const datastore::UIDPointer<basisset::BasisSet> & bs4)
        {
            ModuleBase::CallPyMethod<void>("SetBases_", ncenter, bs1, bs2, bs3, bs4);
        }


        //! \copydoc Calculate
        virtual long Calculate_(int deriv, int shell1, int shell2, int shell3, int shell4, double * outbuf)
        {
            return ModuleBase::CallPyMethod<long>("Calculate_", deriv, shell1, shell2, shell3, shell4, outbuf);
        }
        

    private:
        virtual boost::python::object MoveToPyObject_(std::function<void(modulebase::ModuleBase *)> deleter)
        {
            return ModuleBase::MoveToPyObjectHelper_<TwoElectronIntegralIMPL>(deleter, this);
        }

};

} // close namespace modulebase
} // close namespace bpmodule

#endif
