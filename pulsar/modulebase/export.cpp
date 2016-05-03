/*! \file
 *
 * \brief Python exports for module base classes
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "pulsar/modulebase/All.hpp"
#include "pulsar/datastore/Wavefunction.hpp"
#include "pulsar/modulemanager/ModuleManager.hpp"


using pulsar::modulemanager::ModuleManager;
using pulsar::modulemanager::ModuleInfo;
using pulsar::datastore::OptionMap;



namespace pulsar{
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
         .def("Options", &ModuleBase::Options, pybind11::return_value_policy::copy)
         .def("InitialWfn", static_cast<datastore::Wavefunction &(ModuleBase::*)(void)>(&ModuleBase::InitialWfn), pybind11::return_value_policy::reference_internal) 
         .def("FinalWfn", static_cast<datastore::Wavefunction &(ModuleBase::*)(void)>(&ModuleBase::FinalWfn), pybind11::return_value_policy::reference_internal) 
         .def("SetInitialWfn",&ModuleBase::SetInitialWfn)
         .def("SetFinalWfn",&ModuleBase::SetFinalWfn)
         .def("CreateChildModule", &ModuleBase::CreateChildModulePy)
         .def("CreateChildModuleFromOption", &ModuleBase::CreateChildModuleFromOptionPy)
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
    // System integrals
    /////////////////////////
    pybind11::class_<SystemIntegral_Py> sysint(m, "SystemIntegral", mbase);
    sysint.alias<SystemIntegral>()
          .def(pybind11::init<ID_t>())
          .def_readonly("out", &SystemIntegral_Py::out, pybind11::return_value_policy::reference_internal) 
          .def("Cache", &SystemIntegral_Py::Cache, pybind11::return_value_policy::reference_internal)
          .def("MManager", &SystemIntegral_Py::MManager, pybind11::return_value_policy::reference_internal)
          .def("Calculate", &SystemIntegral::CalculatePy)
          ;

    /////////////////////////
    // One electron integral implementation
    /////////////////////////
    pybind11::class_<OneElectronIntegral_Py> oneel(m, "OneElectronIntegral", mbase);
    oneel.alias<OneElectronIntegral>()
            .def(pybind11::init<ID_t>())
            .def_readonly("out", &OneElectronIntegral_Py::out, pybind11::return_value_policy::reference_internal) 
            .def("Cache", &OneElectronIntegral_Py::Cache, pybind11::return_value_policy::reference_internal)
            .def("MManager", &OneElectronIntegral_Py::MManager, pybind11::return_value_policy::reference_internal)
            .def("SetBases", &OneElectronIntegral::SetBases)
            .def("Calculate", &OneElectronIntegral::CalculatePy)
            .def("CalculateMulti", &OneElectronIntegral::CalculateMultiPy)
            ;

    /////////////////////////
    // Two electron integral implementation
    /////////////////////////
    pybind11::class_<TwoElectronIntegral_Py> twoel(m, "TwoElectronIntegral", mbase);
    twoel.alias<TwoElectronIntegral>()
            .def(pybind11::init<ID_t>())
            .def_readonly("out", &TwoElectronIntegral_Py::out, pybind11::return_value_policy::reference_internal) 
            .def("Cache", &TwoElectronIntegral_Py::Cache, pybind11::return_value_policy::reference_internal)
            .def("MManager", &TwoElectronIntegral_Py::MManager, pybind11::return_value_policy::reference_internal)
            .def("SetBases", &TwoElectronIntegral::SetBases)
            .def("Calculate", &TwoElectronIntegral::CalculatePy)
            .def("CalculateMulti", &TwoElectronIntegral::CalculateMultiPy)
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
} // close namespace pulsar

