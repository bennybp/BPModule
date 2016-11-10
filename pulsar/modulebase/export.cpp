/*! \file
 *
 * \brief Python exports for module base classes
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include <pybind11/operators.h>
#include "pulsar/modulebase/All.hpp"
#include "pulsar/datastore/Wavefunction.hpp"
#include "pulsar/modulemanager/ModuleManager.hpp"

namespace pulsar{

void export_modulebase(pybind11::module & m)
{
    ///////////////////////
    // Module Base classes
    ///////////////////////
    // CallFunc doesn't need to be exported
    pybind11::class_<ModuleBase> mbase(m, "ModuleBase");
    mbase.def("id", &ModuleBase::id)
         .def("key", &ModuleBase::key)
         .def("name", &ModuleBase::name)
         .def("version", &ModuleBase::version)
         .def("print", &ModuleBase::print)
         .def("options", static_cast<OptionMap & (ModuleBase::*)(void)>(&ModuleBase::options), pybind11::return_value_policy::reference_internal)
         .def("create_child", &ModuleBase::create_child_py)
         .def("create_child_from_option", &ModuleBase::create_child_from_option_py)
         .def("enable_debug", &ModuleBase::enable_debug)
         .def("debug_enabled", &ModuleBase::debug_enabled)
         ;


    /////////////////////////
    // Test class
    /////////////////////////
    pybind11::class_<Test_Base, std::unique_ptr<Test_Base>, Test_Base_Py> testbase(m, "Test_Base", mbase);
    testbase.def(pybind11::init<ID_t>())
            .def_readonly("out", &Test_Base_Py::out, pybind11::return_value_policy::reference_internal)
            .def("cache", &Test_Base_Py::cache, pybind11::return_value_policy::reference_internal)
            .def("module_manager", &Test_Base_Py::module_manager, pybind11::return_value_policy::reference_internal)
            .def("run_test", &Test_Base::run_test)
            .def("call_run_test", &Test_Base::call_run_test)
            .def("call_run_test2", &Test_Base::call_run_test2)
            .def("test_throw", &Test_Base::test_throw)
            .def("call_throw", &Test_Base::call_throw)
            .def("add_to_cache", &Test_Base::add_to_cache)
            .def("get_from_cache", &Test_Base::get_from_cache)
            ;


    /////////////////////////
    // System Fragmenters
    /////////////////////////
    pybind11::class_<NMerInfo> (m,"NMerInfo")
        .def(pybind11::init<>())
        .def(pybind11::init<const NMerInfo&>())
        .def_readwrite("sn",&NMerInfo::sn)
        .def_readwrite("nmer",&NMerInfo::nmer)
        .def_readwrite("weight",&NMerInfo::weight)
        .def(pybind11::self==pybind11::self)
        .def(pybind11::self!=pybind11::self)
        ;

    pybind11::class_<SystemFragmenter, std::unique_ptr<SystemFragmenter>, SystemFragmenter_Py> sysfrag(m, "SystemFragmenter", mbase);
    sysfrag.def(pybind11::init<ID_t>())
           .def_readonly("out", &SystemFragmenter_Py::out, pybind11::return_value_policy::reference_internal)
           .def("cache", &SystemFragmenter_Py::cache, pybind11::return_value_policy::reference_internal)
           .def("module_manager", &SystemFragmenter_Py::module_manager, pybind11::return_value_policy::reference_internal)
           .def("fragmentize", &SystemFragmenter_Py::fragmentize_py)
    ;


    /////////////////////////
    // System integrals
    /////////////////////////
    pybind11::class_<SystemIntegral, std::unique_ptr<SystemIntegral>, SystemIntegral_Py> sysint(m, "SystemIntegral", mbase);
    sysint.def(pybind11::init<ID_t>())
          .def_readonly("out", &SystemIntegral_Py::out, pybind11::return_value_policy::reference_internal)
          .def("cache", &SystemIntegral_Py::cache, pybind11::return_value_policy::reference_internal)
          .def("module_manager", &SystemIntegral_Py::module_manager, pybind11::return_value_policy::reference_internal)
          .def("initialize", &SystemIntegral::initialize)
          .def("calculate", &SystemIntegral::calculate_py)
          ;

    /////////////////////////
    // One electron integral implementation
    /////////////////////////
    pybind11::class_<OneElectronIntegral, std::unique_ptr<OneElectronIntegral>, OneElectronIntegral_Py> oneel(m, "OneElectronIntegral", mbase);
    oneel.def(pybind11::init<ID_t>())
         .def_readonly("out", &OneElectronIntegral_Py::out, pybind11::return_value_policy::reference_internal)
         .def("cache", &OneElectronIntegral_Py::cache, pybind11::return_value_policy::reference_internal)
         .def("module_manager", &OneElectronIntegral_Py::module_manager, pybind11::return_value_policy::reference_internal)
         .def("initialize", &OneElectronIntegral::initialize)
         .def("n_components", &OneElectronIntegral::n_components)
         .def("calculate", &OneElectronIntegral::calculate_py)
         .def("calculate_multi", &OneElectronIntegral::calculate_multi_py)
         ;

    ///////////////////////////
    // One electron cacher
    ///////////////////////////
    pybind11::class_<OneElectronMatrix, std::unique_ptr<OneElectronMatrix>, OneElectronMatrix_Py> oneelcache(m, "OneElectronMatrix", mbase);
    oneelcache.def(pybind11::init<ID_t>())
              .def_readonly("out", &OneElectronMatrix_Py::out, pybind11::return_value_policy::reference_internal)
              .def("cache", &OneElectronMatrix_Py::cache, pybind11::return_value_policy::reference_internal)
              .def("module_manager", &OneElectronMatrix_Py::module_manager, pybind11::return_value_policy::reference_internal)
              .def("calculate", &OneElectronMatrix::calculate)
         ;

    /////////////////////////
    // Two electron integral implementation
    /////////////////////////
    pybind11::class_<TwoElectronIntegral, std::unique_ptr<TwoElectronIntegral>, TwoElectronIntegral_Py> twoel(m, "TwoElectronIntegral", mbase);
    twoel.def(pybind11::init<ID_t>())
         .def_readonly("out", &TwoElectronIntegral_Py::out, pybind11::return_value_policy::reference_internal)
         .def("cache", &TwoElectronIntegral_Py::cache, pybind11::return_value_policy::reference_internal)
         .def("module_manager", &TwoElectronIntegral_Py::module_manager, pybind11::return_value_policy::reference_internal)
         .def("initialize", &TwoElectronIntegral::initialize)
         .def("n_components", &TwoElectronIntegral::n_components)
         .def("calculate", &TwoElectronIntegral::calculate_py)
         .def("calculate_multi", &TwoElectronIntegral::calculate_multi_py)
         ;

    ///////////////////////
    // SCF Iterator
    ///////////////////////
    pybind11::class_<SCFIterator, std::unique_ptr<SCFIterator>, SCFIterator_Py> scfiter(m, "SCFIterator", mbase);
    scfiter.def(pybind11::init<ID_t>())
           .def_readonly("out", &SCFIterator_Py::out, pybind11::return_value_policy::reference_internal)
           .def("cache", &SCFIterator_Py::cache, pybind11::return_value_policy::reference_internal)
           .def("module_manager", &SCFIterator_Py::module_manager, pybind11::return_value_policy::reference_internal)
           .def("next", &SCFIterator::next)
            ;

    ///////////////////////
    // Fock Builder
    ///////////////////////
    pybind11::class_<FockBuilder, std::unique_ptr<FockBuilder>, FockBuilder_Py> fockb(m, "FockBuilder", mbase);
    fockb.def(pybind11::init<ID_t>())
         .def_readonly("out", &FockBuilder_Py::out, pybind11::return_value_policy::reference_internal)
         .def("cache", &FockBuilder_Py::cache, pybind11::return_value_policy::reference_internal)
         .def("module_manager", &FockBuilder_Py::module_manager, pybind11::return_value_policy::reference_internal)
         .def("initialize", &FockBuilder::initialize)
         .def("calculate", &FockBuilder::calculate)
         ;

    ///////////////////////
    // Property Calculator
    ///////////////////////
    pybind11::class_<PropertyCalculator, std::unique_ptr<PropertyCalculator>, PropertyCalculator_Py> propcalc(m, "PropertyCalculator", mbase);
    propcalc.def(pybind11::init<ID_t>())
            .def_readonly("out", &PropertyCalculator_Py::out, pybind11::return_value_policy::reference_internal)
            .def("cache", &PropertyCalculator_Py::cache, pybind11::return_value_policy::reference_internal)
            .def("module_manager", &PropertyCalculator_Py::module_manager, pybind11::return_value_policy::reference_internal)
            .def("calculate", &PropertyCalculator::calculate)
           ;

    ///////////////////////
    // Method base class
    ///////////////////////
    pybind11::class_<EnergyMethod, std::unique_ptr<EnergyMethod>, EnergyMethod_Py> energymethod(m, "EnergyMethod", mbase);
    energymethod.def(pybind11::init<ID_t>())
                .def_readonly("out", &EnergyMethod_Py::out, pybind11::return_value_policy::reference_internal)
                .def("cache", &EnergyMethod_Py::cache, pybind11::return_value_policy::reference_internal)
                .def("module_manager", &EnergyMethod_Py::module_manager, pybind11::return_value_policy::reference_internal)
                .def("deriv", &EnergyMethod::deriv)
                .def("energy",&EnergyMethod::energy)
                .def("gradient",&EnergyMethod::gradient)
                .def("hessian",&EnergyMethod::hessian)
                ;
}

} // close namespace pulsar

