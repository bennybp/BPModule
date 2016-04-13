/*! \file
 *
 * \brief Python exports for module base classes
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/modulebase/All.hpp"
#include "bpmodule/datastore/Wavefunction.hpp"
#include "bpmodule/modulemanager/ModuleManager.hpp"


using bpmodule::modulemanager::ModuleManager;
using bpmodule::modulemanager::ModuleInfo;
using bpmodule::datastore::OptionMap;



namespace bpmodule {
namespace modulebase {
namespace export_python {



PYBIND11_PLUGIN(modulebase)
{
    pybind11::module m("modulebase", "Base classes for all modules");

    ///////////////////////
    // Module Base classes
    ///////////////////////
    // CallFunc doesn't need to be exported 
    pybind11::class_<ModuleBase> mbase(m, "ModuleBase");
    mbase.def("ID", &ModuleBase::ID)
         .def("Key", &ModuleBase::Key)
         .def("Name", &ModuleBase::Name)
         .def("Version", &ModuleBase::Version)
         .def("Print", &ModuleBase::Print)
         .def("Options", static_cast<OptionMap &(ModuleBase::*)(void)>(&ModuleBase::Options), pybind11::return_value_policy::reference_internal)
         .def("InitialWfn", static_cast<datastore::Wavefunction &(ModuleBase::*)(void)>(&ModuleBase::InitialWfn), pybind11::return_value_policy::reference_internal) 
         .def("FinalWfn", static_cast<datastore::Wavefunction &(ModuleBase::*)(void)>(&ModuleBase::FinalWfn), pybind11::return_value_policy::reference_internal) 
         .def("SetInitialWfn",&ModuleBase::SetInitialWfn)
         .def("SetFinalWfn",&ModuleBase::SetFinalWfn)
         .def("CreateChildModule", &ModuleBase::CreateChildModulePy)
         .def("EnableDebug", &ModuleBase::EnableDebug)
         .def("DebugEnabled", &ModuleBase::DebugEnabled)
         ;


    /////////////////////////
    // Test class
    /////////////////////////
    pybind11::class_<Test_Base_Py> testbase(m, "Test_Base", mbase);
    testbase.alias<Test_Base>()
            .def(pybind11::init<ID_t>())
            .def_readonly("out", &Test_Base_Py::out, pybind11::return_value_policy::reference_internal) 
            .def("Cache", &Test_Base_Py::Cache, pybind11::return_value_policy::reference_internal)
            .def("MManager", &Test_Base_Py::MManager, pybind11::return_value_policy::reference_internal)
            .def("RunTest", &Test_Base::RunTest)
            .def("CallRunTest", &Test_Base::CallRunTest)
            .def("CallRunTest2", &Test_Base::CallRunTest2)
            .def("TestThrow", &Test_Base::TestThrow)
            .def("CallThrow", &Test_Base::CallThrow)
            ;


    /////////////////////////
    // System Fragmenters
    /////////////////////////
    pybind11::class_<SystemFragmenter_Py> sysfrag(m, "SystemFragmenter", mbase);
    sysfrag.alias<SystemFragmenter>()
            .def(pybind11::init<ID_t>())
            .def_readonly("out", &SystemFragmenter_Py::out, pybind11::return_value_policy::reference_internal) 
            .def("Cache", &SystemFragmenter_Py::Cache, pybind11::return_value_policy::reference_internal)
            .def("MManager", &SystemFragmenter_Py::MManager, pybind11::return_value_policy::reference_internal)
            .def("Fragmentize", &SystemFragmenter::Fragmentize)
    ;


    /////////////////////////
    // One electron integral implementation
    /////////////////////////
    //! \todo Don't know about from Calculate to python
    pybind11::class_<OneElectronIntegralIMPL_Py> oneelimpl(m, "OneElectronIntegralIMPL", mbase);
    oneelimpl.alias<OneElectronIntegralIMPL>()
            .def(pybind11::init<ID_t>())
            .def_readonly("out", &OneElectronIntegralIMPL_Py::out, pybind11::return_value_policy::reference_internal) 
            .def("Cache", &OneElectronIntegralIMPL_Py::Cache, pybind11::return_value_policy::reference_internal)
            .def("MManager", &OneElectronIntegralIMPL_Py::MManager, pybind11::return_value_policy::reference_internal)
            .def("SetBases", &OneElectronIntegralIMPL::SetBases)
            .def("Calculate", &OneElectronIntegralIMPL::Calculate)
            .def("GetIntegralCount", &OneElectronIntegralIMPL::GetIntegralCount)
            .def("GetBuf", &OneElectronIntegralIMPL::GetBufPy)
            ;

    /////////////////////////
    // Two electron integral implementation
    /////////////////////////
    pybind11::class_<TwoElectronIntegralIMPL_Py> twoelimpl(m, "TwoElectronIntegralIMPL", mbase);
    twoelimpl.alias<TwoElectronIntegralIMPL>()
            .def(pybind11::init<ID_t>())
            .def_readonly("out", &TwoElectronIntegralIMPL_Py::out, pybind11::return_value_policy::reference_internal) 
            .def("Cache", &TwoElectronIntegralIMPL_Py::Cache, pybind11::return_value_policy::reference_internal)
            .def("MManager", &TwoElectronIntegralIMPL_Py::MManager, pybind11::return_value_policy::reference_internal)
            .def("SetBases", &TwoElectronIntegralIMPL::SetBases)
            .def("Calculate", &TwoElectronIntegralIMPL::Calculate)
            .def("GetIntegralCount", &TwoElectronIntegralIMPL::GetIntegralCount)
            .def("GetBuf", &TwoElectronIntegralIMPL::GetBufPy)
            ;

    ///////////////////////
    // Method base class
    ///////////////////////
    pybind11::class_<EnergyMethod_Py> energymethod(m, "EnergyMethod", mbase);
    energymethod.alias<EnergyMethod>()
            .def(pybind11::init<ID_t>())
            .def_readonly("out", &EnergyMethod_Py::out, pybind11::return_value_policy::reference_internal) 
            .def("Cache", &EnergyMethod_Py::Cache, pybind11::return_value_policy::reference_internal)
            .def("MManager", &EnergyMethod_Py::MManager, pybind11::return_value_policy::reference_internal)
            .def("Deriv", &EnergyMethod::Deriv)
            .def("Energy",&EnergyMethod::Energy)
            .def("Gradient",&EnergyMethod::Gradient)
            .def("Hessian",&EnergyMethod::Hessian)
            ;
    return m.ptr();
}
} // close namespace export_python
} // close namespace modulebase
} // close namespace bpmodule

