/*! \file
 *
 * \brief Python exports for module base classes
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/modulebase/All.hpp"
#include "bpmodule/datastore/Wavefunction.hpp"
#include "bpmodule/modulelocator/ModuleManager.hpp"


using bpmodule::modulelocator::ModuleManager;
using bpmodule::modulelocator::ModuleInfo;
using bpmodule::datastore::OptionMap;



namespace bpmodule {
namespace modulebase {
namespace export_python {


template<typename T>
using ModuleBasePtr = std::shared_ptr<T>;


PYBIND11_PLUGIN(modulebase)
{
    pybind11::module m("modulebase", "Base classes for all modules");

    ///////////////////////
    // Module Base classes
    ///////////////////////
    // CallFunc doesn't need to be exported 
    pybind11::class_<ModuleBase, ModuleBasePtr<ModuleBase>> mbase(m, "ModuleBase");
    mbase.def("ID", &ModuleBase::ID)
         .def("Key", &ModuleBase::Key)
         .def("Name", &ModuleBase::Name)
         .def("Version", &ModuleBase::Version)
         .def("Print", &ModuleBase::Print)
         .def("Options", static_cast<OptionMap &(ModuleBase::*)(void)>(&ModuleBase::Options), pybind11::return_value_policy::reference_internal)
         .def("Wfn", static_cast<datastore::Wavefunction &(ModuleBase::*)(void)>(&ModuleBase::Wfn), pybind11::return_value_policy::reference_internal) 
         .def("CreateChildModule", &ModuleBase::CreateChildModulePy)
         //.def("MManager", static_cast<ModuleManager &(ModuleBase::*)(void) const>(&ModuleBase::MManager), pybind11::return_value_policy::reference_internal) 
         ;


    /////////////////////////
    // Test class
    /////////////////////////
    pybind11::class_<Test_Base_Py, ModuleBasePtr<Test_Base>> testbase(m, "Test_Base", mbase);
    testbase.alias<Test_Base>()
            .def(pybind11::init<unsigned long>())
            .def("Cache", &Test_Base_Py::Cache, pybind11::return_value_policy::reference_internal)
            .def("RunTest", &Test_Base::RunTest)
            .def("CallRunTest", &Test_Base::CallRunTest)
            .def("TestThrow", &Test_Base::TestThrow)
            .def("CallThrow", &Test_Base::CallThrow)
            ;


    /////////////////////////
    // One electron integral implementation
    /////////////////////////
    //! \todo Don't know about from Calculate to python
    pybind11::class_<OneElectronIntegralIMPL_Py, ModuleBasePtr<OneElectronIntegralIMPL>> oneelimpl(m, "OneElectronIntegralIMPL", mbase);
    oneelimpl.alias<OneElectronIntegralIMPL>()
            .def(pybind11::init<unsigned long>())
            .def("Cache", &OneElectronIntegralIMPL_Py::Cache, pybind11::return_value_policy::reference_internal)
            .def("SetBases", &OneElectronIntegralIMPL::SetBases)
            .def("Calculate", &OneElectronIntegralIMPL::Calculate)
            .def("GetIntegralCount", &OneElectronIntegralIMPL::GetIntegralCount)
            .def("GetBuf", &OneElectronIntegralIMPL::GetBufPy)
            ;

/*
    /////////////////////////
    // One electron integral builder
    /////////////////////////
    register_ptr_to_python<boost::shared_ptr<OneElectronIntegral>>();

    //! \todo Don't know about from Calculate to python
    class_<OneElectronIntegral, bases<ModuleBase>, boost::noncopyable>("OneElectronIntegral", init<PyObject *, unsigned long>())
    .def("SetBases", &OneElectronIntegral::SetBases)
    .def("Calculate", &OneElectronIntegral::Calculate)
    .def("GetIntegralCount", &OneElectronIntegral::GetIntegralCount)
    .def("GetBuf", &OneElectronIntegral::GetBufPy)
    ;
*/

    /////////////////////////
    // Two electron integral implementation
    /////////////////////////
    pybind11::class_<TwoElectronIntegralIMPL_Py, ModuleBasePtr<TwoElectronIntegralIMPL>> twoelimpl(m, "TwoElectronIntegralIMPL", mbase);
    twoelimpl.alias<TwoElectronIntegralIMPL>()
            .def(pybind11::init<unsigned long>())
            .def("Cache", &TwoElectronIntegralIMPL_Py::Cache, pybind11::return_value_policy::reference_internal)
            .def("SetBases", &TwoElectronIntegralIMPL::SetBases)
            .def("Calculate", &TwoElectronIntegralIMPL::Calculate)
            .def("GetIntegralCount", &TwoElectronIntegralIMPL::GetIntegralCount)
            .def("GetBuf", &TwoElectronIntegralIMPL::GetBufPy)
            ;

/*
    /////////////////////////
    // Two electron integral builder
    /////////////////////////
    register_ptr_to_python<boost::shared_ptr<TwoElectronIntegral>>();

    class_<TwoElectronIntegral, bases<ModuleBase>, boost::noncopyable>("TwoElectronIntegral", init<PyObject *, unsigned long>())
    .def("SetBases", &TwoElectronIntegral::SetBases)
    .def("Calculate", &TwoElectronIntegral::Calculate)
    .def("GetIntegralCount", &TwoElectronIntegral::GetIntegralCount)
    .def("GetBuf", &TwoElectronIntegral::GetBufPy)
    ;
*/

    return m.ptr();
}


} // close namespace export_python
} // close namespace modulebase
} // close namespace bpmodule

