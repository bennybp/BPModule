/*! \file
 *
 * \brief Loading and storing of C/C++ supermodules (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


// For dlsym, etc.
#include <dlfcn.h>

#include "bpmodule/modulemanager/CppSupermoduleLoader.hpp"
#include "bpmodule/output/GlobalOutput.hpp"
#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/exception/Assert.hpp"

using namespace bpmodule::exception;
using namespace bpmodule::output;


namespace bpmodule {
namespace modulemanager {


CppSupermoduleLoader::~CppSupermoduleLoader()
{
    // Finalization function in the so file
    typedef void (*FinalizeFunc)(void);

    // delete creator functions
    for(auto & it : soinfo_)
        it.second.creators.Clear();

    // close all the handles
    for(auto & it : soinfo_)
    {
        void * handle = it.second.handle;

        GlobalOutput("Looking to close C++ supermodule %?\n", it.first);
        FinalizeFunc ffn = reinterpret_cast<FinalizeFunc>(dlsym(handle, "FinalizeSupermodule"));

        // it's ok if it doesn't exist
        char const * error;
        if((error = dlerror()) != NULL)
            GlobalDebug("Supermodule %? doesn't have finalization function. Skipping\n", it.first);
        else
        {
            GlobalDebug("Running finalization function in %?\n", it.first);
            ffn();
        }

        // close the so
        dlclose(handle);

        GlobalOutput("Closed supermodule %?\n", it.first);
    }

    // soinfo_ is cleared via its destructor
}




const ModuleCreationFuncs & CppSupermoduleLoader::LoadSupermodule(const std::string & spath)
{
    // Initializing/Finalizing the so file
    typedef void (*InitializeFunc)(void);

    // Function for creating modules
    typedef ModuleCreationFuncs (*GeneratorFunc)(void);

    if(spath.size() == 0)
        throw ModuleLoadException("Cannot open supermodule SO file - path not given");

    GlobalDebug("Loading supermodule %?\n", spath);

    if(soinfo_.count(spath) == 0)
    {
        // first time loading this so file. Load it, get the
        // creators and run the initialization functions

        void * handle;
        char const * error; // for dlerror

        GlobalDebug("Supermodule not yet loaded. Looking to open C++ supermodule SO file: %?\n", spath);
        handle = dlopen(spath.c_str(), RTLD_NOW | RTLD_LOCAL);

        // open the module
        if(!handle)
            throw ModuleLoadException("Cannot open supermodule SO file - dlopen error",
                                      "path", spath,
                                      "dlerror", std::string(dlerror()));

        GlobalSuccess("Successfully opened supermodule %?\n", spath);

        // Check for the InsertSupermodule function first (before initializing)
        GeneratorFunc fn = reinterpret_cast<GeneratorFunc>(dlsym(handle, "InsertSupermodule"));
        if((error = dlerror()) != NULL)
        {
            dlclose(handle);
            throw ModuleLoadException("Cannot find InsertSupermodule function in SO file",
                                      "path", spath, "dlerror", error);
        }


        // Now initialize the supermodule if the function exists
        InitializeFunc ifn = reinterpret_cast<InitializeFunc>(dlsym(handle, "InitializeSupermodule"));

        if((error = dlerror()) != NULL) // it's ok if it doesn't exist
            GlobalDebug("SO file %? doesn't have initialization function. Skipping\n", spath);
        else
        {
            GlobalDebug("Running initialization function for supermodule %?\n", spath);
            ifn();
        }


        // get the module creation functions
        // and put them in the map, under the key for this supermodule
        soinfo_.emplace(spath, SOInfo{std::move(handle), fn()});  // only line that modifies the object

        GlobalDebug("Finished loading supermodule %?\n", spath);
    }
    else
        GlobalDebug("Supermodule %? has already been loaded\n", spath);

    // just to be safe
    Assert<ModuleManagerException>(soinfo_.count(spath) == 1, "CppSupermoduleLoader SOInfo doesn't this information...");

    return soinfo_.at(spath).creators;
}


} // close namespace modulemanager
} // close namespace bpmodule
