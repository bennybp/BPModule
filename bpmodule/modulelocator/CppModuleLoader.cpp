/*! \file
 *
 * \brief Loading and storing of C modules (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include <dlfcn.h>

#include "bpmodule/modulelocator/CppModuleLoader.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/modulelocator/ModuleLocator.hpp"
#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/exception/Assert.hpp"

using namespace bpmodule::exception;


namespace bpmodule {
namespace modulelocator {

CppModuleLoader::CppModuleLoader(ModuleLocator * mlt)
    : mlt_(mlt)
{ }



CppModuleLoader::~CppModuleLoader()
{
    // Finalization function in the so file
    typedef void (*FinalizeFunc)(void);

    // delete creator functions
    creators_.clear();

    // close all the handles
    for(auto it : handles_)
    {
        output::Output("Closing %1%\n", it.first);
        FinalizeFunc ffn = reinterpret_cast<FinalizeFunc>(dlsym(it.second, "FinalizeSupermodule"));

        // it's ok if it doesn't exist
        char const * error;
        if((error = dlerror()) != NULL)
            output::Debug("SO file %1% doesn't have finalization function. Skipping\n", it.first);
        else
        {
            output::Debug("Running finalization function in %1%\n", it.first);
            ffn();
        }

        // close the so
        dlclose(it.second);
    }
    handles_.clear();
}



void CppModuleLoader::LoadSO(const ModuleInfo & minfo)
{
    // Initializing/Finalizing the so file
    typedef void (*InitializeFunc)(void);
    
    // Function for creating modules
    typedef ModuleCreationFuncs (*GeneratorFunc)(void);


    // may throw
    ModuleInfo mi(minfo);

    // trailing slash on path should have been added by python scripts
    std::string sopath = mi.path + mi.soname;

    ModuleCreationFuncs cf;

    // see if the module is loaded
    // if so, reuse that handle
    if(handles_.count(sopath) > 0)
    {
        Assert<GeneralException>(creators_.count(sopath) > 0, "Creators does not have something for an already-loaded SO file");
        cf = creators_.at(sopath);
    }
    else
    {
        // first time loading this so file. Load it, get the
        // creators and run the initialization functions

        void * handle;
        char const * error; // for dlerror

        output::Debug("Looking to open so file: %1%\n", sopath);
        handle = dlopen(sopath.c_str(), RTLD_NOW | RTLD_GLOBAL);

        // open the module
        if(!handle)
            throw ModuleLoadException("Cannot open SO file",
                                      "path", sopath, "modulekey", mi.key,
                                      "modulename", mi.name, "dlerror", std::string(dlerror()));

        // 1.) Initialize the supermodule if the function exists
        InitializeFunc ifn = reinterpret_cast<InitializeFunc>(dlsym(handle, "InitializeSupermodule"));
        // it's ok if it doesn't exist
        if((error = dlerror()) != NULL)
            output::Debug("SO file %1% doesn't have initialization function. Skipping\n", sopath);
        else
        {
            output::Debug("Running initialization function in %1%\n", sopath);
            ifn();
        }
        


        // 2.) Module creator generation
        GeneratorFunc fn = reinterpret_cast<GeneratorFunc>(dlsym(handle, "InsertSupermodule"));
        if((error = dlerror()) != NULL)
        {
            dlclose(handle);
            throw ModuleLoadException("Cannot find function in SO file",
                                      "path", sopath, "modulekey", mi.key,
                                      "modulename", mi.name, "dlerror", error);
        }

        output::Success("Successfully opened %1%\n", sopath);

        cf = fn();

        creators_.emplace(sopath, cf);

        if(handles_.count(sopath) == 0)
            handles_.emplace(sopath, handle);      // strong exception guarantee, but shouldn't ever throw
    }


    mlt_->InsertModule(cf, mi); // strong exception guarantee
}



} // close namespace modulelocator
} // close namespace bpmodule
