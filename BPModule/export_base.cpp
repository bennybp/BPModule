// helpers
#include "BPModule/python_helper/Python_stdconvert.hpp"

// Parallelization
#include "BPModule/parallel/Parallel.hpp"

// Various components
#include "BPModule/core/ModuleStore.hpp"
#include "BPModule/core/CModuleLoader.hpp"
#include "BPModule/core/PyModuleLoader.hpp"
#include "BPModule/core/Exception.hpp"
#include "BPModule/modulebase/All_python.hpp"


using namespace boost::python;
namespace bpy = boost::python;



namespace bpmodule {
namespace export_python {


template<typename T>
boost::shared_ptr<T> Wrap_GetScopedModule(ModuleStore * ms, const std::string & key)
{
    T & mod = ms->GetModule<T>(key);
    std::function<void(ModuleBase *)> dfunc = std::bind(static_cast<void(ModuleStore::*)(ModuleBase *)>(&ModuleStore::RemoveModule), ms, std::placeholders::_1);
    return boost::shared_ptr<T>(&mod, dfunc);
}


////////////////////////////
// Main boost python part
////////////////////////////

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ThrowException_overloads, ThrowException, 1, 2)


BOOST_PYTHON_MODULE(bpmodule_base)
{
    // set the translator for exceptions
    register_exception_translator<BPModuleException>(&TranslateException);

    // converting pairs
    to_python_converter<std::pair<int, int>, PairConverter<int, int>>();
    to_python_converter<std::pair<long, long>, PairConverter<long, long>>();
    to_python_converter<std::pair<double, double>, PairConverter<double, double>>();
    to_python_converter<std::pair<std::string, std::string>, PairConverter<std::string, std::string>>();

    // converting vectors
    to_python_converter<std::vector<int>, VectorConverter<int>>();
    to_python_converter<std::vector<long>, VectorConverter<long>>();
    to_python_converter<std::vector<double>, VectorConverter<double>>();
    to_python_converter<std::vector<std::string>, VectorConverter<std::string>>();

    // convert map to dict
    to_python_converter<std::map<std::string, std::string>, MapConverter<std::map<std::string, std::string>>>();
    to_python_converter<std::unordered_map<std::string, std::string>, MapConverter<std::unordered_map<std::string, std::string>>>();

    // setting the output
    def("SetOut_Stdout", &output::SetOut_Stdout);
    def("SetOut_Stderr", &output::SetOut_Stderr);
    def("SetOut_File", &output::SetOut_File);
    def("SetColor", &output::SetColor);
    def("SetDebug", &output::SetDebug);
    def("Valid", &output::Valid);
    def("Flush", &output::Flush);

    // printing to output
    def("Output", Output_Wrap_Output);
    def("Success", Output_Wrap_Success);
    def("Warning", Output_Wrap_Warning);
    def("Error", Output_Wrap_Error);
    def("Changed", Output_Wrap_Changed);
    def("Debug", Output_Wrap_Debug);

    // Parallelization
    def("InitParallel", &parallel::InitParallel);
    def("FinalizeParallel", &parallel::FinalizeParallel);

    // This is only needed because we pass through python
    // No need to do declare all the members, etc 
    class_<ModuleInfo, boost::noncopyable>("ModuleInfo");


    class_<ModuleStore, boost::noncopyable>("ModuleStore")
    .def("Size", &ModuleStore::Size)
    .def("Has", &ModuleStore::Has)
    .def("RemoveModule", static_cast<void(ModuleStore::*)(unsigned long)>(&ModuleStore::RemoveModule))
    .def("SetOptions", Wrap_ModuleStore_SetOptions)
    .def("GetKeys", &ModuleStore::GetKeys)
    .def("KeyInfo", &ModuleStore::KeyInfo)
    .def("GetModule", &ModuleStore::GetModule<ModuleBase>, return_internal_reference<>())
    .def("GetModule_Test", &ModuleStore::GetModule<Test_Base>, return_internal_reference<>())
    .def("GetScopedModule", Wrap_GetScopedModule<ModuleBase>)
    .def("GetScopedModule_Test", Wrap_GetScopedModule<Test_Base>);


    class_<CModuleLoader, boost::noncopyable>("CModuleLoader", init<ModuleStore *>())
    .def("CloseHandles", &CModuleLoader::CloseHandles)
    .def("UnloadAll", &CModuleLoader::UnloadAll)
    .def("LoadSO", Wrap_CModuleLoader_LoadSO);

    class_<PyModuleLoader, boost::noncopyable>("PyModuleLoader", init<ModuleStore *>())
    .def("UnloadAll", &PyModuleLoader::UnloadAll)
    .def("AddPyModule", Wrap_PyModuleLoader_AddPyModule);


    /////////////////////////
    // CalcData
    /////////////////////////
    class_<CalcData>("CalcData", init<>())
    .def(init<const CalcData &>())
    .def("Has", &CalcData::Has)
    .def("GetCopy", &CalcData::GetCopy<bpy::object>)
//    .def("GetRef", &CalcData::GetRef<bpy::object>, return_value_policy<copy_const_reference>()) 
    .def("Set", &CalcData::Set<bpy::object>)
    .def("Erase", &CalcData::Erase)
    .def("SetRef", static_cast<void(CalcData::*)(const CalcData &, const std::string &, const std::string &)>(&CalcData::SetRef))
    .def("SetRef", static_cast<void(CalcData::*)(const CalcData &, const std::string &)>(&CalcData::SetRef));


    ///////////////////////
    // Module Base classes
    ///////////////////////
    // python should never derive from ModuleBase, so a constructor should not be needed
    class_<ModuleBase, boost::noncopyable>("ModuleBase", no_init)
//    .def("MStore", &ModuleBase::MStore, return_internal_reference<>())
    .def("ID", &ModuleBase::ID)
    .def("Key", &ModuleBase::Key, return_value_policy<copy_const_reference>())
    .def("Name", &ModuleBase::Name, return_value_policy<copy_const_reference>())
    .def("Version", &ModuleBase::Version, return_value_policy<copy_const_reference>())
    .def("HasOption", &ModuleBase::HasOption)
    .def("GetOption", &ModuleBase::GetOption<bpy::object>);



    register_ptr_to_python<boost::shared_ptr<Test_Base>>();
    class_<Test_Base_Wrap, bases<ModuleBase>, boost::shared_ptr<Test_Base_Wrap>, boost::noncopyable>("Test_Base", init<unsigned long, ModuleStore &, const ModuleInfo &>())
    .def("MStore", &Test_Base_Wrap::MStore, return_internal_reference<>())
    .def("ThrowException", &Test_Base_Wrap::ThrowException, ThrowException_overloads())
    .def("RunTest", pure_virtual(&Test_Base::RunTest))
    .def("CallRunTest", pure_virtual(&Test_Base::CallRunTest))
    .def("Throw", pure_virtual(&Test_Base::Throw))
    .def("CallThrow", pure_virtual(&Test_Base::CallThrow))
    .def("CalcTest", pure_virtual(&Test_Base::CalcTest), return_value_policy<return_by_value>());

}


} // close namespace export_python
} // close namespace bpmodule

