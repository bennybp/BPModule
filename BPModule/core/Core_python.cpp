#include "BPModule/core/OptionMap.h"
#include "BPModule/core/Output.h"
#include "BPModule/core/ModuleStore.h"

#include "BPModule/core/ModuleBase.h"
#include "BPModule/core/Test_Base.h"

#include <boost/python.hpp>

using namespace boost::python;

namespace bpmodule {

template<typename T>
static
std::vector<T> ConvertListToVec(const list & list)
{
    int length = extract<int>(list.attr("__len__")());
    std::vector<T> ret;
    ret.reserve(length);

    for (int i = 0; i < length; i++)
        ret.push_back(extract<std::string>(list[i]));

    return ret;
}

/*
template<typename T>
static 
T ConvertDictToMap(const dict & d)
{
    T m;
    int length = extract<int>(list.attr("__len__")());
    for(int i = 0; i < length; i++)
    

    return m;
}
*/

template<typename T>
static
list ConvertVecToList(const std::vector<T> & v)
{
    list result;
    for(auto & it : v)
        result.append(it);    
    return result;
}

template<typename T>
static
dict ConvertMapToDict(const T & m)
{
    dict d;
    for(auto & it : m)
        d[it.first] = it.second;
    return d;
}



template<typename T>
struct VectorConverter
{
    static PyObject* convert(const std::vector<T> & v)
    {
      boost::python::list lst = ConvertVecToList(v);
      return boost::python::incref(lst.ptr());
    }
};


template<class T1, class T2>
struct PairConverter
{
    static PyObject* convert(const std::pair<T1, T2>& pair)
    {
      return incref(make_tuple(pair.first, pair.second).ptr());
    }
};

template<typename T>
struct MapConverter
{
    static PyObject* convert(const T & m)
    {
      boost::python::dict d = ConvertMapToDict(m);
      return boost::python::incref(d.ptr());
    }
};


// the "options" element of the dict must be an OptionMap, not a list of tuples
ModuleInfo DictToModuleInfo(const dict & dictionary)
{
    ModuleInfo ret;
    ret.name = extract<std::string>(dictionary["name"]);
    ret.soname = extract<std::string>(dictionary["soname"]);
    ret.version = extract<std::string>(dictionary["version"]);
    ret.description = extract<std::string>(dictionary["description"]);
    ret.authors = ConvertListToVec<std::string>(extract<list>(dictionary["authors"]));
    ret.refs = ConvertListToVec<std::string>(extract<list>(dictionary["refs"]));
    ret.options = extract<OptionMap>(dictionary["options"]);

    return ret;
}

dict ModuleInfoToDict(const ModuleInfo & mi)
{
    dict d;

    //simple ones first
    d["name"] = mi.name;
    d["soname"] = mi.soname;
    d["version"] = mi.version;
    d["description"] = mi.description;

    // now lists of strings
    d["authors"] = ConvertVecToList(mi.authors);
    d["refs"] = ConvertVecToList(mi.refs);

    // now options
    // \todo only creates strings for values right now

    list opd;
    auto keys = mi.options.GetKeys();
    for(auto & k : keys)
        opd.append(boost::python::make_tuple(k, mi.options.GetValueStr(k), mi.options.GetHelp(k)));

    d["options"] = opd; 

    return d;
}


struct ModuleInfoConverter
{
    static PyObject* convert(const ModuleInfo & m)
    {
      boost::python::dict d = ModuleInfoToDict(m);
      return boost::python::incref(d.ptr());
    }
};


// the main exception translator
void TranslateException(const BPModuleException & ex)
{
    PyErr_SetString(PyExc_RuntimeError, bpmodule::ExceptionString(ex).c_str());
}


// some wrappers
// wraps ModuleStore::LoadSO so that it can take a dict
bool Wrap_ModuleStore_LoadSO(ModuleStore * ms, const std::string & key,
                             const std::string & sopath, const boost::python::dict & d)
{
    return ms->LoadSO(key, sopath, DictToModuleInfo(d));
}




////////////////////////////
// Main boost python part
////////////////////////////
BOOST_PYTHON_MODULE(bpmodule_core)
{
    register_exception_translator<BPModuleException>(&TranslateException);

    // converting pairs
    to_python_converter<std::pair<std::string, std::string>, PairConverter<std::string, std::string>>();

    // converting vector of string
    to_python_converter<std::vector<std::string>, VectorConverter<std::string>>();

    // converting vector of string pairs
    to_python_converter<std::vector<std::pair<std::string, std::string>>, VectorConverter<std::pair<std::string, std::string>>>();

    // convert module info to dict
    to_python_converter<ModuleInfo, ModuleInfoConverter>();

    // convert map to dict
    to_python_converter<std::map<std::string, std::string>, MapConverter<std::map<std::string, std::string>>>();
    to_python_converter<std::unordered_map<std::string, std::string>, MapConverter<std::unordered_map<std::string, std::string>>>();


    // setting the output
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

/*
    // commented since it is now converted to a dict
    class_<ModuleInfo>("ModuleInfo")
           .def_readwrite("name", &ModuleInfo::name)
           .def_readwrite("soname", &ModuleInfo::soname)
           .def_readwrite("version", &ModuleInfo::version)
           .def_readwrite("authors", &ModuleInfo::authors)
           .def_readwrite("description", &ModuleInfo::description)
           .def_readwrite("refs", &ModuleInfo::refs)
           .def_readwrite("options", &ModuleInfo::options);
*/

    class_<ModuleStore, boost::noncopyable>("ModuleStore")
           .def("LoadSO", Wrap_ModuleStore_LoadSO)
           .def("Lock", &ModuleStore::Lock)
           .def("Size", &ModuleStore::Size)
           .def("Has", &ModuleStore::Has)
           .def("GetKeys", &ModuleStore::GetKeys)
           .def("ModuleInfoFromID", &ModuleStore::ModuleInfoFromID)
           .def("ModuleInfoFromKey", &ModuleStore::ModuleInfoFromKey)
           .def("KeyFromID", &ModuleStore::KeyFromID)
           .def("GetModule", &ModuleStore::GetModule<ModuleBase>, return_value_policy<manage_new_object>())
           .def("GetModule_Test", &ModuleStore::GetModule<Test_Base>, return_value_policy<manage_new_object>());



    //class_<ModuleBase, boost::noncopyable>("ModuleBase", init<ModuleStore *, const OptionMap &>())
    class_<ModuleBase, boost::noncopyable>("ModuleBase", no_init)
           .def("Info", &ModuleBase::Info)
           .def("Key", &ModuleBase::Key)
           .def("Traits", &ModuleBase::Traits)
           .def("Options", &ModuleBase::Options);

    class_<Test_Base, bases<ModuleBase>, boost::noncopyable>("Test_Base", no_init)
           .def("RunTest", &Test_Base::RunTest);

}



} // close namespace bpmodule

