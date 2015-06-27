#ifndef MODULEINFO_H
#define MODULEINFO_H

#include <string>
#include <vector>

#include "BPModule/core/OptionMap.hpp"


namespace boost {
namespace python {

class dict;

}
}


namespace bpmodule {


struct ModuleInfo
{
    std::string key;
    std::string name;
    std::string type;
    std::string path;
    std::string soname;
    std::string version;
    std::vector<std::string> authors;
    std::string description;
    std::vector<std::string> refs;
    OptionMap options;

    void FromPythonDict(const boost::python::dict & dictionary);
};


} // close namespace bpmodule


#endif
