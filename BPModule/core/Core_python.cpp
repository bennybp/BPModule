#include "BPModule/core/OptionMap.h"
#include "BPModule/core/Output.h"
#include "BPModule/core/ModuleStore.h"

#include "BPModule/core/ModuleBase.h"
#include "BPModule/core/Test_Base.h"

#include <boost/python.hpp>

using namespace boost::python;

namespace bpmodule {

static std::vector<std::string> ConvertStringList(const boost::python::list & list)
{
    int length = boost::python::extract<int>(list.attr("__len__")());
    std::vector<std::string> ret(length);

    for (int i = 0; i < length; i++)
        ret[i] = boost::python::extract<std::string>(list[i]);

    return ret;
}

static boost::python::list ConvertListString(const std::vector<std::string> & v)
{
    list result;
    for(auto & it : v)
        result.append(it);    
    return result;
}

// opmap is passed in because it's easier to do in python
// and you don't have to pass in the type that way
ModuleInfo MakeInfo(const boost::python::dict dictionary, const OptionMap & opmap)
{
    ModuleInfo ret;
    ret.name = boost::python::extract<std::string>(dictionary["name"]);
    ret.soname = boost::python::extract<std::string>(dictionary["soname"]);
    ret.version = boost::python::extract<std::string>(dictionary["version"]);
    ret.description = boost::python::extract<std::string>(dictionary["description"]);
    ret.authors = ConvertStringList(boost::python::extract<boost::python::list>(dictionary["authors"]));
    ret.refs = ConvertStringList(boost::python::extract<boost::python::list>(dictionary["refs"]));
    ret.options = opmap;

    return ret;
}

// the main exception translator
void TranslateException(const BPModuleException & ex)
{
    PyErr_SetString(PyExc_RuntimeError, bpmodule::ExceptionString(ex).c_str());
}


// some wrappers
boost::python::list Wrap_ModuleStore_Keys(const ModuleStore * m)
{
    return ConvertListString(m->GetKeys());
}


BOOST_PYTHON_MODULE(bpmodule_core)
{
    register_exception_translator<BPModuleException>(&TranslateException);

    // for vector of strings
    //class_<std::vector<std::string>>("StringVec")
    //    .def(vector_indexing_suite<std::vector<std::string>>());


    def("SetOut_Stdout", bpmodule::SetOut_Stdout);
    def("SetOut_File", bpmodule::SetOut_File);
    def("SetColor", bpmodule::SetColor);

    class_<OptionMap>("OptionMap")
        .def("Set", &OptionMap::Set<int>)
        .def("Set", &OptionMap::Set<double>)
        .def("Set", &OptionMap::Set<std::string>)
        .def("Has", &OptionMap::Has)
        .def("GetHelp", &OptionMap::GetHelp)
        .def("GetAllHelp", &OptionMap::GetAllHelp)
        .def("GetValueStr", &OptionMap::GetValueStr)
        .def("GetAllValueStr", &OptionMap::GetAllValueStr)
        .def("GetKeys", &OptionMap::GetKeys)
        .def("Size", &OptionMap::Size);

    class_<ModuleInfo>("ModuleInfo")
           .def_readwrite("name", &ModuleInfo::name)
           .def_readwrite("soname", &ModuleInfo::soname)
           .def_readwrite("version", &ModuleInfo::version)
           .def_readwrite("authors", &ModuleInfo::authors)
           .def_readwrite("description", &ModuleInfo::description)
           .def_readwrite("refs", &ModuleInfo::refs)
           .def_readwrite("options", &ModuleInfo::options);

    class_<ModuleStore, boost::noncopyable>("ModuleStore")
           .def("LoadSO", &ModuleStore::LoadSO)
           .def("Lock", &ModuleStore::Lock)
           .def("Size", &ModuleStore::Size)
           .def("Has", &ModuleStore::Has)
           .def("GetKeys", Wrap_ModuleStore_Keys)
           .def("ModuleInfoFromID", &ModuleStore::ModuleInfoFromID)
           .def("ModuleInfoFromKey", &ModuleStore::ModuleInfoFromKey)
           .def("KeyFromID", &ModuleStore::KeyFromID)
           .def("GetModule", &ModuleStore::GetModule<ModuleBase>, return_value_policy<manage_new_object>())
           .def("GetModule_Test", &ModuleStore::GetModule<Test_Base>, return_value_policy<manage_new_object>());

    def("MakeInfo", MakeInfo);


    //class_<ModuleBase, boost::noncopyable>("ModuleBase", init<ModuleStore *, const OptionMap &>())
    class_<ModuleBase, boost::noncopyable>("ModuleBase", no_init)
           .def("MyInfo", &ModuleBase::MyInfo)
           .def("MyKey", &ModuleBase::MyKey)
           .def("Traits", &ModuleBase::Traits)
           .def("Options", &ModuleBase::Options);

    class_<Test_Base, bases<ModuleBase>, boost::noncopyable>("Test_Base", no_init)
           .def("RunTest", &Test_Base::RunTest);

}



} // close namespace bpmodule

