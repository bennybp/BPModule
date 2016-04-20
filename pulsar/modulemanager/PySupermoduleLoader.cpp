/*! \file
 *
 * \brief Loading and storing of Python supermodules (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bpmodule/modulemanager/PySupermoduleLoader.hpp"
#include "bpmodule/util/Filesystem.hpp"
#include "bpmodule/output/GlobalOutput.hpp"
#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/exception/Assert.hpp"
#include "bpmodule/python/Call.hpp"

using namespace bpmodule::exception;
using namespace bpmodule::output;


namespace bpmodule {
namespace modulemanager {


PySupermoduleLoader::~PySupermoduleLoader()
{
    // delete creator functions
    for(auto & it : objmap_)
        it.second.creators.Clear();

    // close all the handles
    for(auto & it : objmap_)
    {
        GlobalOutput("Looking to close python supermodule %?\n", it.first);

        if(python::HasCallableAttr(it.second.mod, "FinalizeSupermodule"))
        {
            GlobalDebug("Running finalization function in %?\n", it.first);
            python::CallPyFuncAttr<void>(it.second.mod, "FinalizeSupermodule");
        }
        else
            GlobalDebug("Supermodule %? doesn't have finalization function. Skipping\n", it.first);

        GlobalOutput("Closed supermodule %?\n", it.first);
    }

    // objmap_ is cleared via its destructor
}


const ModuleCreationFuncs & PySupermoduleLoader::LoadSupermodule(const std::string & spath)
{
    if(spath.size() == 0)
        throw ModuleLoadException("Cannot open python supermodule - path not given");

    GlobalDebug("Loading python supermodule %?\n", spath);


    if(!objmap_.count(spath))
    {
        GlobalDebug("Supermodule not yet loaded. Looking to import python supermodule: %?\n", spath);

        // Need a few packages
        pybind11::module mod_sys = pybind11::module::import("sys");
        pybind11::module mod_importlib = pybind11::module::import("importlib");

        // spath = full path to module
        // we need the directory above that
        std::pair<std::string, std::string> splitpath = util::SplitPath(spath);

        GlobalDebug("Importing %?  from  %?\n", splitpath.first, splitpath.second);

        // update the python search paths
        pybind11::object oldpaths = mod_sys.attr("path");
        mod_sys.attr("path") = python::ConvertToPy(splitpath.first);

        pybind11::module mod = python::CallPyFuncAttr<pybind11::module>(mod_importlib, "import_module", splitpath.second);

        // reset the python search paths
        mod_sys.attr("path") = oldpaths;


        GlobalSuccess("Successfully imported supermodule %?\n", spath);


        // Check for the InsertSupermodule function first (before initializing)
        if(!python::HasCallableAttr(mod, "InsertSupermodule"))
            throw ModuleLoadException("Cannot find InsertSupermodule function in python module",
                                      "path", spath);


        // Now initialize if the function exists
        if(python::HasCallableAttr(mod, "InitializeSupermodule"))
        {
            GlobalDebug("Running initialization function for supermodule %?\n", spath);
            python::CallPyFuncAttr<void>(mod, "InitializeSupermodule");
        }
        else
            GlobalDebug("Supermodule %? doesn't have initialization function. Skipping\n", spath);


        // get the module creation functions
        // and put them right in the map
        ModuleCreationFuncs creators = python::CallPyFuncAttr<ModuleCreationFuncs>(mod, "InsertSupermodule");

        objmap_.emplace(spath, PyModInfo{std::move(mod), std::move(creators)});
    }

    // just to be safe
    Assert<ModuleLoadException>(objmap_.count(spath) == 1, "PySupermoduleLoader PyModInfo doesn't have this information...");

    return objmap_.at(spath).creators;
}


} // close namespace modulemanager
} // close namespace bpmodule
