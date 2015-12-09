/*! \file
 *
 * \brief Two-electron integral implementation (source)
 * \author Ben Pritchard (ben@bennyp.org)
 */ 

#include "bpmodule/modulebase/TwoElectronIntegralIMPL.hpp"

namespace bpmodule {
namespace modulebase {

TwoElectronIntegralIMPL::TwoElectronIntegralIMPL(unsigned long id)
    : ModuleBase(id)
{ }

TwoElectronIntegralIMPL::TwoElectronIntegralIMPL(PyObject * self, unsigned long id)
    : ModuleBase(self, id)
{ }


void TwoElectronIntegralIMPL::SetBases(int ncenter,
                                       datastore::UIDPointer<basisset::BasisSet> bs1,
                                       datastore::UIDPointer<basisset::BasisSet> bs2,
                                       datastore::UIDPointer<basisset::BasisSet> bs3,
                                       datastore::UIDPointer<basisset::BasisSet> bs4)
{
    ModuleBase::CallPyMethod<void>("SetBases", ncenter, bs1, bs2, bs3, bs4);
}


////////////////////////////
// Private functions
////////////////////////////
boost::python::object TwoElectronIntegralIMPL::MoveToPyObject_(std::function<void(modulebase::ModuleBase *)> deleter)
{
    return ModuleBase::MoveToPyObjectHelper_<TwoElectronIntegralIMPL>(deleter, this);
}



} // close namespace modulebase
} // close namespace bpmodule
