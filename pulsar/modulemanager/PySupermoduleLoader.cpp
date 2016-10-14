/*! \file
 *
 * \brief Loading and storing of Python supermodules (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "pulsar/modulemanager/PySupermoduleLoader.hpp"
#include "pulsar/util/Filesystem.hpp"
#include "pulsar/output/GlobalOutput.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/exception/Assert.hpp"
#include "pulsar/python/Call.hpp"


using namespace pulsar::output;


namespace pulsar{
namespace modulemanager {


PySupermoduleLoader::~PySupermoduleLoader()
{
    // delete creator functions
    for(auto & it : objmap_)
        it.second.creators.clear();

    // close all the handles
    for(auto & it : objmap_)
    {
        print_global_output("Looking to close python supermodule %?\n", it.first);

        if(python::has_callable_attr(it.second.mod, "finalize_supermodule"))
        {
            print_global_debug("Running finalization function in %?\n", it.first);
            python::call_py_func_attr<void>(it.second.mod, "finalize_supermodule");
        }
        else
            print_global_debug("Supermodule %? doesn't have finalization function. Skipping\n", it.first);

        print_global_output("Closed supermodule %?\n", it.first);
    }

    // objmap_ is cleared via its destructor
}


const ModuleCreationFuncs & PySupermoduleLoader::load_supermodule(const std::string & spath)
{
    if(spath.size() == 0)
        throw ModuleLoadException("Cannot open python supermodule - path not given");

    print_global_debug("Loading python supermodule %?\n", spath);


    if(!objmap_.count(spath))
    {
        print_global_debug("Supermodule not yet loaded. Looking to import python supermodule: %?\n", spath);

        // Need a few packages
        pybind11::module mod_sys = pybind11::module::import("sys");
        pybind11::module mod_importlib = pybind11::module::import("importlib");

        // spath = full path to module
        // we need the directory above that
        std::pair<std::string, std::string> splitpath = util::split_path(spath);

        print_global_debug("Importing %?  from  %?\n", splitpath.first, splitpath.second);

        // update the python search paths
        pybind11::object oldpaths = mod_sys.attr("path");
        mod_sys.attr("path") = python::convert_to_py(splitpath.first);

        pybind11::module mod = python::call_py_func_attr<pybind11::module>(mod_importlib, "import_module", splitpath.second);

        // reset the python search paths
        mod_sys.attr("path") = oldpaths;


        print_global_success("Successfully imported supermodule %?\n", spath);


        // Check for the insert_supermodule function first (before initializing)
        if(!python::has_callable_attr(mod, "insert_supermodule"))
            throw ModuleLoadException("Cannot find insert_supermodule function in python module",
                                      "path", spath);


        // Now initialize if the function exists
        if(python::has_callable_attr(mod, "initialize_supermodule"))
        {
            print_global_debug("Running initialization function for supermodule %?\n", spath);
            python::call_py_func_attr<void>(mod, "initialize_supermodule");
        }
        else
            print_global_debug("Supermodule %? doesn't have initialization function. Skipping\n", spath);


        // get the module creation functions
        // and put them right in the map
        ModuleCreationFuncs creators = python::call_py_func_attr<ModuleCreationFuncs>(mod, "insert_supermodule");

        objmap_.emplace(spath, PyModInfo{std::move(mod), std::move(creators)});
    }

    // just to be safe
    psr_assert<ModuleLoadException>(objmap_.count(spath) == 1, "PySupermoduleLoader PyModInfo doesn't have this information...");

    return objmap_.at(spath).creators;
}


} // close namespace modulemanager
} // close namespace pulsar
