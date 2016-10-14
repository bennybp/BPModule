/*! \file
 *
 * \brief Loading and storing of C/C++ supermodules (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


// For dlsym, etc.
#include <dlfcn.h>

#include "pulsar/modulemanager/CppSupermoduleLoader.hpp"
#include "pulsar/output/GlobalOutput.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/exception/Assert.hpp"


using namespace pulsar::output;


namespace pulsar{
namespace modulemanager {


CppSupermoduleLoader::~CppSupermoduleLoader()
{
    // Finalization function in the so file
    typedef void (*FinalizeFunc)(void);

    // delete creator functions
    for(auto & it : soinfo_)
        it.second.creators.clear();

    // close all the handles
    for(auto & it : soinfo_)
    {
        void * handle = it.second.handle;

        print_global_output("Looking to close C++ supermodule %?\n", it.first);
        FinalizeFunc ffn = reinterpret_cast<FinalizeFunc>(dlsym(handle, "finalize_supermodule"));

        // it's ok if it doesn't exist
        char const * error;
        if((error = dlerror()) != NULL)
            print_global_debug("Supermodule %? doesn't have finalization function. Skipping\n", it.first);
        else
        {
            print_global_debug("Running finalization function in %?\n", it.first);
            ffn();
        }

        // close the so
        dlclose(handle);

        print_global_output("Closed supermodule %?\n", it.first);
    }

    // soinfo_ is cleared via its destructor
}




const ModuleCreationFuncs & CppSupermoduleLoader::load_supermodule(const std::string & spath)
{
    // Initializing/Finalizing the so file
    typedef void (*InitializeFunc)(void);

    // Function for creating modules
    typedef ModuleCreationFuncs (*GeneratorFunc)(void);

    if(spath.size() == 0)
        throw ModuleLoadException("Cannot open supermodule SO file - path not given");

    print_global_debug("Loading supermodule %?\n", spath);

    if(soinfo_.count(spath) == 0)
    {
        // first time loading this so file. Load it, get the
        // creators and run the initialization functions

        void * handle;
        char const * error; // for dlerror

        print_global_debug("Supermodule not yet loaded. Looking to open C++ supermodule SO file: %?\n", spath);
        handle = dlopen(spath.c_str(), RTLD_NOW | RTLD_LOCAL);

        // open the module
        if(!handle)
            throw ModuleLoadException("Cannot open supermodule SO file - dlopen error",
                                      "path", spath,
                                      "dlerror", std::string(dlerror()));

        print_global_success("Successfully opened supermodule %?\n", spath);

        // Check for the insert_supermodule function first (before initializing)
        GeneratorFunc fn = reinterpret_cast<GeneratorFunc>(dlsym(handle, "insert_supermodule"));
        if((error = dlerror()) != NULL)
        {
            dlclose(handle);
            throw ModuleLoadException("Cannot find insert_supermodule function in SO file",
                                      "path", spath, "dlerror", error);
        }


        // Now initialize the supermodule if the function exists
        InitializeFunc ifn = reinterpret_cast<InitializeFunc>(dlsym(handle, "initialize_supermodule"));

        if((error = dlerror()) != NULL) // it's ok if it doesn't exist
            print_global_debug("SO file %? doesn't have initialization function. Skipping\n", spath);
        else
        {
            print_global_debug("Running initialization function for supermodule %?\n", spath);
            ifn();
        }


        // get the module creation functions
        // and put them in the map, under the key for this supermodule
        soinfo_.emplace(spath, SOInfo{std::move(handle), fn()});  // only line that modifies the object

        print_global_debug("Finished loading supermodule %?\n", spath);
    }
    else
        print_global_debug("Supermodule %? has already been loaded\n", spath);

    // just to be safe
    psr_assert<ModuleManagerException>(soinfo_.count(spath) == 1, "CppSupermoduleLoader SOInfo doesn't this information...");

    return soinfo_.at(spath).creators;
}


} // close namespace modulemanager
} // close namespace pulsar
