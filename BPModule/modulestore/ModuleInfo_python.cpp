#include "BPModule/modulestore/ModuleInfo.hpp"
#include "BPModule/python_helper/Python_stdconvert.hpp"

using bpmodule::python_helper::ConvertListToVec;

namespace bpmodule {
namespace modulestore {

// the "options" element of the dict must be a list of tuples
ModuleInfo::ModuleInfo(const boost::python::dict & dictionary)
{
    try {
        key = boost::python::extract<std::string>(dictionary["key"]);
        name = boost::python::extract<std::string>(dictionary["name"]);
        type = boost::python::extract<std::string>(dictionary["type"]);
        path = boost::python::extract<std::string>(dictionary["path"]);
        version = boost::python::extract<std::string>(dictionary["version"]);
        description = boost::python::extract<std::string>(dictionary["description"]);
        authors = ConvertListToVec<std::string>(boost::python::extract<boost::python::list>(dictionary["authors"]));
        refs = ConvertListToVec<std::string>(boost::python::extract<boost::python::list>(dictionary["refs"]));
    
        if(dictionary.has_key("passedoptions"))
        {
            datastore::OptionMap op;
            boost::python::list olist = boost::python::extract<boost::python::list>(dictionary["passedoptions"]);
            options = datastore::OptionMap(olist);
        }

        if(dictionary.has_key("soname"))
            soname = boost::python::extract<std::string>(dictionary["soname"]);
    }
    catch(...)
    {
        throw BPModuleException("Malformed dictionary for ModuleInfo");
    }
}


} // close namespace modulestore
} // close namespace bpmodule

