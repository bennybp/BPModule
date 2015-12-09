/*! \file
 *
 * \brief One-electron integral implementation (header)
 * \author Ben Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_ONEELECTRONINTEGRALIMPL_HPP_
#define _GUARD_ONEELECTRONINTEGRALIMPL_HPP_

#include "bpmodule/modulebase/ModuleBase.hpp"


namespace bpmodule {
namespace modulebase {

/*! \brief One-electron integral implementation new module
 *
 */
class OneElectronIntegralIMPL : public ModuleBase
{
    public:
        OneElectronIntegralIMPL(unsigned long id);

        OneElectronIntegralIMPL(PyObject * self, unsigned long id);

        virtual void SetBases(const datastore::UIDPointer<basisset::BasisSet> & bs1,
                              const datastore::UIDPointer<basisset::BasisSet> & bs2);


        //! \todo don't know about passing outbuf to python
        virtual long Calculate(int shell1, int shell2, double * outbuf);

    private:
        virtual boost::python::object MoveToPyObject_(std::function<void(modulebase::ModuleBase *)> deleter);

};

} // close namespace modulebase
} // close namespace bpmodule

#endif
