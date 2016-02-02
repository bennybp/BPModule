#include "bpmodule/modulemanager/PyModuleLoader.hpp"
#include "bpmodule/util/Filesystem.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/modulemanager/ModuleManager.hpp"
#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/exception/Assert.hpp"
#include "bpmodule/python/Call.hpp"

using namespace bpmodule::exception;


namespace bpmodule {
namespace modulemanager {

ModuleCreationFuncs::Func PyModuleLoader::LoadModule(const ModuleInfo & minfo)
{
    if(!objmap_.count(minfo.path))
    {
        pybind11::module mod_sys = pybind11::module::import("sys");
        pybind11::module mod_importlib = pybind11::module::import("importlib");


        //! \todo fragile? Empty paths, etc

        // In the minfo, path = full path to module
        // we need the directory above that
        std::pair<std::string, std::string> splitpath = util::SplitPath(minfo.path);

        // update the python search paths
        pybind11::object oldpaths = mod_sys.attr("path");
        mod_sys.attr("path") = python::ConvertToPy(splitpath.first);

        pybind11::module mod = python::CallPyFuncAttr<pybind11::module>(mod_importlib, "import_module", splitpath.second);

        // reset the python search paths
        mod_sys.attr("path") = oldpaths;
        /*
                if(!python::HasCallableAttr(mod, "InsertSupermodule"))
                    throw exception

                if(python::HasCallableAttr(mod, "InitializeSupermodule"))
                    CallPyFuncAttr(mod, "InitializeSupermodule");
        */
        ModuleCreationFuncs creators = python::CallPyFuncAttr<ModuleCreationFuncs>(mod, "InsertSupermodule");

        PyModInfo pyinfo {std::move(mod), std::move(creators)};

        objmap_.emplace(minfo.path, std::move(pyinfo));
    }

    Assert<ModuleLoadException>(objmap_.count(minfo.path), "I don't have information for that module");

}


} // close namespace modulemanager
} // close namespace bpmodule
