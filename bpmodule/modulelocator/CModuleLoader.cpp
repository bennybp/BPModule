/*! \file
 *
 * \brief Loading and storing of C modules (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include <dlfcn.h>

#include "bpmodule/modulelocator/CModuleLoader.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/modulebase/ModuleBase.hpp"
#include "bpmodule/exception/ModuleLoadException.hpp"

using bpmodule::modulebase::ModuleBase;
using bpmodule::exception::ModuleLoadException;


namespace bpmodule {
namespace modulelocator {

CModuleLoader::CModuleLoader(ModuleLocator * mlt)
    : mlt_(mlt)
{ }



CModuleLoader::~CModuleLoader()
{
    // delete creator functions
    creators_.clear();

    // close all the handles
    for(auto it : handles_)
    {
        output::Output("Closing %1%\n", it.first);
        dlclose(it.second);
    }
    handles_.clear();
}



void CModuleLoader::LoadSO(const std::string & key, pybind11::dict minfo)
{
    // function in the so file
    typedef CreatorFunctions (*GeneratorFunc)(void);


    // may throw
    ModuleInfo mi(minfo);

    // trailing slash on path should have been added by python scripts
    std::string sopath = mi.path + mi.soname;

    CreatorFunctions cf;

    // see if the module is loaded
    // if so, reuse that handle
    if(handles_.count(sopath) > 0)
    {
        //! \todo exceptions
        cf = creators_.at(sopath);
    }
    else
    {
        void * handle;
        char const * error; // for dlerror

        output::Debug("Looking to open so file: %1%\n", sopath);
        handle = dlopen(sopath.c_str(), RTLD_NOW | RTLD_GLOBAL);
        // open the module
        if(!handle)
            throw ModuleLoadException("Cannot open SO file",
                                      sopath, key, mi.name, "dlerror", std::string(dlerror()));

        // get the pointer to the GeneratorModule function
        GeneratorFunc fn = reinterpret_cast<GeneratorFunc>(dlsym(handle, "CreateModule"));
        if((error = dlerror()) != NULL)
        {
            dlclose(handle);
            throw ModuleLoadException("Cannot find function in SO file",
                                        sopath, key, mi.name, "dlerror", error);
        }

        output::Success("Successfully opened %1%\n", sopath);

        cf = fn();

        creators_.emplace(sopath, cf);

        if(handles_.count(sopath) == 0)
            handles_.emplace(sopath, handle);      // strong exception guarantee, but shouldn't ever throw
    }


    mlt_->InsertModule(key, cf, mi); // strong exception guarantee
}



} // close namespace modulelocator
} // close namespace bpmodule
