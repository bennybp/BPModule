/*! \file
 *
 * \brief One-electron integral implementation (source)
 * \author Ben Pritchard (ben@bennyp.org)
 */ 

#include "bpmodule/modulebase/OneElectronIntegralIMPL.hpp"

namespace bpmodule {
namespace modulebase {

OneElectronIntegralIMPL::OneElectronIntegralIMPL(unsigned long id)
    : ModuleBase(id)
{ }

OneElectronIntegralIMPL::OneElectronIntegralIMPL(PyObject * self, unsigned long id)
    : ModuleBase(self, id)
{ }


void OneElectronIntegralIMPL::SetBases(datastore::UIDPointer<basisset::BasisSet> bs1,
                                       datastore::UIDPointer<basisset::BasisSet> bs2)
{
    ModuleBase::CallPyMethod<void>("SetBases", bs1, bs2);
}


////////////////////////////
// Private functions
////////////////////////////
boost::python::object OneElectronIntegralIMPL::MoveToPyObject_(std::function<void(modulebase::ModuleBase *)> deleter)
{
    return ModuleBase::MoveToPyObjectHelper_<OneElectronIntegralIMPL>(deleter, this);
}



} // close namespace modulebase
} // close namespace bpmodule
