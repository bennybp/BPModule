#include "BPModule/core/Output.h"
#include "BPModule/export_core/OptionMap_python.h"
#include "BPModule/export_core/Python_stdconvert.h"
#include "BPModule/export_core/ModuleInfo_python.h"

namespace out = bpmodule::output;

namespace bpmodule {
namespace export_python {

// the "options" element of the dict must be a list of tuples
ModuleInfo DictToModuleInfo(const boost::python::dict & dictionary)
{
    ModuleInfo ret;
    try
    {
        ret.key = boost::python::extract<std::string>(dictionary["key"]);
        ret.name = boost::python::extract<std::string>(dictionary["name"]);
        ret.type = boost::python::extract<std::string>(dictionary["type"]);
        ret.path = boost::python::extract<std::string>(dictionary["path"]);
        ret.version = boost::python::extract<std::string>(dictionary["version"]);
        ret.description = boost::python::extract<std::string>(dictionary["description"]);
        ret.authors = ConvertListToVec<std::string>(boost::python::extract<boost::python::list>(dictionary["authors"]));
        ret.refs = ConvertListToVec<std::string>(boost::python::extract<boost::python::list>(dictionary["refs"]));

        OptionMap op;
        boost::python::list olist = boost::python::extract<boost::python::list>(dictionary["options"]);
        ret.options = ListToOptionMap(olist);

        if(dictionary.has_key("soname"))
            ret.soname = boost::python::extract<std::string>(dictionary["soname"]);

        //if(dictionary.has_key("path"))

    }
    catch(...)
    {
        out::Error("HEREEEE\n");
    }

    return ret;
}



boost::python::dict ModuleInfoToDict(const ModuleInfo & mi)
{
    boost::python::dict d;

    //simple ones first
    d["key"] = mi.key;
    d["name"] = mi.name;
    d["type"] = mi.type;
    d["path"] = mi.path;
    d["soname"] = mi.soname;
    d["version"] = mi.version;
    d["description"] = mi.description;

    // now lists of strings
    d["authors"] = ConvertVecToList(mi.authors);
    d["refs"] = ConvertVecToList(mi.refs);

    // now options
    d["options"] = OptionMapToList(mi.options);

    return d;
}


} // close namespace export_python
} // close namespace bpmodule

