#include "BPModule/core/OptionMap.h"
#include "BPModule/core/ModuleStore.h"

#include <iostream>
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


BOOST_PYTHON_MODULE(bpmodule_core)
{
    class_<OptionMap>("OptionMap")
        .def("Set", &OptionMap::Set<int>)
        .def("Set", &OptionMap::Set<double>)
        .def("Set", &OptionMap::Set<std::string>)
        .def("Get", &OptionMap::GetString)
        .def("Has", &OptionMap::Has)
        .def("Size", &OptionMap::Size)
        .def("Dump", &OptionMap::Dump);

    class_<ModuleInfo>("ModuleInfo")
           .def_readwrite("name", &ModuleInfo::name)
           .def_readwrite("soname", &ModuleInfo::soname)
           .def_readwrite("version", &ModuleInfo::version)
           .def_readwrite("description", &ModuleInfo::description)
           .def_readwrite("authors", &ModuleInfo::authors)
           .def_readwrite("refs", &ModuleInfo::refs)
           .def_readwrite("options", &ModuleInfo::options);

    class_<ModuleStore, boost::noncopyable>("ModuleStore")
           .def("LoadSO", &ModuleStore::LoadSO)
           .def("Lock", &ModuleStore::Lock)
           .def("Size", &ModuleStore::Size)
           .def("Dump", &ModuleStore::Dump);

    def("MakeInfo", MakeInfo);
}


} // close namespace bpmodule

