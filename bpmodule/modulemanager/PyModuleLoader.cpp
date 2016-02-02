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

        output::Warning("LOADING: %1%  :  %2%\n", splitpath.first, splitpath.second);
        // update the python search paths
        pybind11::object oldpaths = mod_sys.attr("path");
        mod_sys.attr("path") = python::ConvertToPy(splitpath.first);

        pybind11::module mod = python::CallPyFuncAttr<pybind11::module>(mod_importlib, "import_module", splitpath.second);

        // reset the python search paths
        mod_sys.attr("path") = oldpaths;


        if(python::HasCallableAttr(mod, "InitializeSupermodule"))
        {
            output::Debug("Running initialization function in %1%\n", minfo.path);
            python::CallPyFuncAttr<void>(mod, "InitializeSupermodule");
        }


        if(!python::HasCallableAttr(mod, "InsertSupermodule"))
            throw ModuleLoadException("Cannot find InsertSupermodule function in python module",
                                      "path", minfo.path, "modulename", minfo.name);

        ModuleCreationFuncs creators = python::CallPyFuncAttr<ModuleCreationFuncs>(mod, "InsertSupermodule");

        PyModInfo pyinfo{std::move(mod), std::move(creators)};

        objmap_.emplace(minfo.path, std::move(pyinfo));
    }

    Assert<ModuleLoadException>(objmap_.count(minfo.path) == 1, "PyModuleLoader PyModInfo doesn't have just-added module");


    // does this module actually create something with this key?
    const ModuleCreationFuncs & cf2 = objmap_.at(minfo.path).creators;
    if(!cf2.HasCreator(minfo.name))
        throw ModuleLoadException("Creators from python module cannot create a module with this name",
                                  "path", minfo.path, "modulename", minfo.name);

    return cf2.GetCreator(minfo.name);
}


} // close namespace modulemanager
} // close namespace bpmodule
