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



void export_pybind11(pybind11::module & mtop)
{
    pybind11::module m = mtop.def_submodule("modulebase", "Base classes for all modules");

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
         .def("Options", static_cast<OptionMap & (ModuleBase::*)(void)>(&ModuleBase::Options), pybind11::return_value_policy::reference_internal)
         .def("CreateChild", &ModuleBase::CreateChildPy)
         .def("CreateChildFromOption", &ModuleBase::CreateChildFromOptionPy)
         .def("EnableDebug", &ModuleBase::EnableDebug)
         .def("DebugEnabled", &ModuleBase::DebugEnabled)
         ;


    /////////////////////////
    // Test class
    /////////////////////////
    pybind11::class_<Test_Base, std::unique_ptr<Test_Base>, Test_Base_Py> testbase(m, "Test_Base", mbase);
    testbase.def(pybind11::init<ID_t>())
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
    pybind11::class_<NMerInfo> (m,"NMerInfo")
        .def(pybind11::init<>())
        .def_readwrite("SN",&NMerInfo::SN)
        .def_readwrite("NMer",&NMerInfo::NMer)
        .def_readwrite("Weight",&NMerInfo::Weight)
        ;

    pybind11::class_<SystemFragmenter, std::unique_ptr<SystemFragmenter>, SystemFragmenter_Py> sysfrag(m, "SystemFragmenter", mbase);
    sysfrag.def(pybind11::init<ID_t>())
           .def_readonly("out", &SystemFragmenter_Py::out, pybind11::return_value_policy::reference_internal)
           .def("Cache", &SystemFragmenter_Py::Cache, pybind11::return_value_policy::reference_internal)
           .def("MManager", &SystemFragmenter_Py::MManager, pybind11::return_value_policy::reference_internal)
           .def("Fragmentize", &SystemFragmenter_Py::Py_Fragmentize)
    ;


    /////////////////////////
    // System integrals
    /////////////////////////
    pybind11::class_<SystemIntegral, std::unique_ptr<SystemIntegral>, SystemIntegral_Py> sysint(m, "SystemIntegral", mbase);
    sysint.def(pybind11::init<ID_t>())
          .def_readonly("out", &SystemIntegral_Py::out, pybind11::return_value_policy::reference_internal)
          .def("Cache", &SystemIntegral_Py::Cache, pybind11::return_value_policy::reference_internal)
          .def("MManager", &SystemIntegral_Py::MManager, pybind11::return_value_policy::reference_internal)
          .def("Initialize", &SystemIntegral::Initialize)
          .def("Calculate", &SystemIntegral::CalculatePy)
          ;

    /////////////////////////
    // One electron integral implementation
    /////////////////////////
    pybind11::class_<OneElectronIntegral, std::unique_ptr<OneElectronIntegral>, OneElectronIntegral_Py> oneel(m, "OneElectronIntegral", mbase);
    oneel.def(pybind11::init<ID_t>())
         .def_readonly("out", &OneElectronIntegral_Py::out, pybind11::return_value_policy::reference_internal)
         .def("Cache", &OneElectronIntegral_Py::Cache, pybind11::return_value_policy::reference_internal)
         .def("MManager", &OneElectronIntegral_Py::MManager, pybind11::return_value_policy::reference_internal)
         .def("Initialize", &OneElectronIntegral::Initialize)
         .def("NComponents", &OneElectronIntegral::NComponents)
         .def("Calculate", &OneElectronIntegral::CalculatePy)
         .def("CalculateMulti", &OneElectronIntegral::CalculateMultiPy)
         ;

    ///////////////////////////
    // One electron cacher
    ///////////////////////////
    pybind11::class_<OneElectronCacher, std::unique_ptr<OneElectronCacher>, OneElectronCacher_Py> oneelcache(m, "OneElectronCacher", mbase);
    oneelcache.def(pybind11::init<ID_t>())
              .def_readonly("out", &OneElectronCacher_Py::out, pybind11::return_value_policy::reference_internal)
              .def("Cache", &OneElectronCacher_Py::Cache, pybind11::return_value_policy::reference_internal)
              .def("MManager", &OneElectronCacher_Py::MManager, pybind11::return_value_policy::reference_internal)
              .def("Calculate", &OneElectronCacher::Calculate)
         ;

    /////////////////////////
    // Two electron integral implementation
    /////////////////////////
    pybind11::class_<TwoElectronIntegral, std::unique_ptr<TwoElectronIntegral>, TwoElectronIntegral_Py> twoel(m, "TwoElectronIntegral", mbase);
    twoel.def(pybind11::init<ID_t>())
         .def_readonly("out", &TwoElectronIntegral_Py::out, pybind11::return_value_policy::reference_internal)
         .def("Cache", &TwoElectronIntegral_Py::Cache, pybind11::return_value_policy::reference_internal)
         .def("MManager", &TwoElectronIntegral_Py::MManager, pybind11::return_value_policy::reference_internal)
         .def("Initialize", &TwoElectronIntegral::Initialize)
         .def("NComponents", &TwoElectronIntegral::NComponents)
         .def("Calculate", &TwoElectronIntegral::CalculatePy)
         .def("CalculateMulti", &TwoElectronIntegral::CalculateMultiPy)
         ;

    ///////////////////////
    // SCF Iterator
    ///////////////////////
    pybind11::class_<SCFIterator, std::unique_ptr<SCFIterator>, SCFIterator_Py> scfiter(m, "SCFIterator", mbase);
    scfiter.def(pybind11::init<ID_t>())
           .def_readonly("out", &SCFIterator_Py::out, pybind11::return_value_policy::reference_internal)
           .def("Cache", &SCFIterator_Py::Cache, pybind11::return_value_policy::reference_internal)
           .def("MManager", &SCFIterator_Py::MManager, pybind11::return_value_policy::reference_internal)
           .def("Next", &SCFIterator::Next)
            ;

    ///////////////////////
    // Fock Builder
    ///////////////////////
    pybind11::class_<FockBuilder, std::unique_ptr<FockBuilder>, FockBuilder_Py> fockb(m, "FockBuilder", mbase);
    fockb.def(pybind11::init<ID_t>())
         .def_readonly("out", &FockBuilder_Py::out, pybind11::return_value_policy::reference_internal)
         .def("Cache", &FockBuilder_Py::Cache, pybind11::return_value_policy::reference_internal)
         .def("MManager", &FockBuilder_Py::MManager, pybind11::return_value_policy::reference_internal)
         .def("Initialize", &FockBuilder::Initialize)
         .def("Calculate", &FockBuilder::Calculate)
         ;

    ///////////////////////
    // Property Calculator
    ///////////////////////
    pybind11::class_<PropertyCalculator, std::unique_ptr<PropertyCalculator>, PropertyCalculator_Py> propcalc(m, "PropertyCalculator", mbase);
    propcalc.def(pybind11::init<ID_t>())
            .def_readonly("out", &PropertyCalculator_Py::out, pybind11::return_value_policy::reference_internal)
            .def("Cache", &PropertyCalculator_Py::Cache, pybind11::return_value_policy::reference_internal)
            .def("MManager", &PropertyCalculator_Py::MManager, pybind11::return_value_policy::reference_internal)
            .def("Calculate", &PropertyCalculator::Calculate)
           ;

    ///////////////////////
    // Method base class
    ///////////////////////
    pybind11::class_<EnergyMethod, std::unique_ptr<EnergyMethod>, EnergyMethod_Py> energymethod(m, "EnergyMethod", mbase);
    energymethod.def(pybind11::init<ID_t>())
                .def_readonly("out", &EnergyMethod_Py::out, pybind11::return_value_policy::reference_internal)
                .def("Cache", &EnergyMethod_Py::Cache, pybind11::return_value_policy::reference_internal)
                .def("MManager", &EnergyMethod_Py::MManager, pybind11::return_value_policy::reference_internal)
                .def("Deriv", &EnergyMethod::Deriv)
                .def("Energy",&EnergyMethod::Energy)
                .def("Gradient",&EnergyMethod::Gradient)
                .def("Hessian",&EnergyMethod::Hessian)
                ;
}
} // close namespace export_python
} // close namespace modulebase
} // close namespace pulsar

