/*! \file
 *
 * \brief Loading and storing of C modules (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <dlfcn.h>

#include <boost/python.hpp>

#include "bpmodule/modulestore/CModuleLoader.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/modulebase/ModuleBase.hpp"

using bpmodule::modulebase::ModuleBase;
using bpmodule::exception::GeneralException;


namespace bpmodule {
namespace modulestore {

CModuleLoader::CModuleLoader(ModuleStore * mst)
    : BASE(mst)
{ }



CModuleLoader::~CModuleLoader()
{
    // order is important. Delete before closing handles
    BASE::DeleteAll();

    // close all the handles
    for(auto it : handles_)
    {
        output::Output("Closing %1%\n", it.first);
        dlclose(it.second);
    }
    handles_.clear();
}



ModuleBase * CModuleLoader::CreateWrapper_(CreateFunc fn,
                                           const std::string & key,
                                           unsigned long id,
                                           ModuleStore & mstore,
                                           const ModuleInfo & minfo)
{
    // Have the base ModuleLoaderBase class take ownership,
    // but return the ptr

    ModuleBase * newobj = fn(key, id, mstore, minfo);
    std::unique_ptr<ModuleBase> uptr(newobj);
    BASE::TakeObject(id, std::move(uptr));  // strong exception guarantee
    return newobj;
}



void CModuleLoader::LoadSO_(const std::string & key, const ModuleInfo & minfo)
{
    // trailing slash on path should have been added by python scripts
    std::string sopath = minfo.path + minfo.soname;

    char * error; // for dlerror
    void * handle;

    // see if the module is loaded
    // if so, reuse that handle
    if(handles_.count(sopath) > 0)
        handle = handles_[sopath];
    else
    {
        output::Debug("Looking to open so file: %1%\n", sopath);
        handle = dlopen(sopath.c_str(), RTLD_NOW | RTLD_GLOBAL);
        // open the module
        if(!handle)
            throw GeneralException(
                                     "Cannot load SO file",
                                     {
                                         { "File", sopath },
                                         { "Error", dlerror() }
                                     }
                                   );
    }


    // get the pointer to the CreateModule function
    CreateFunc fn = reinterpret_cast<CreateFunc>(dlsym(handle, "CreateModule"));
    if((error = dlerror()) != NULL)
    {
        dlclose(handle);
        throw GeneralException(
                                 "Cannot find function in SO file",
                                 {
                                     { "File", sopath },
                                     { "Function", "CreateModule" },
                                     { "Error", error }
                                 }
                               );
    }


    output::Success("Successfully opened %1%\n", sopath);

    ModuleStore::ModuleGeneratorFunc cfunc = std::bind(&CModuleLoader::CreateWrapper_, this, fn,
                                                       std::placeholders::_1,
                                                       std::placeholders::_2,
                                                       std::placeholders::_3,
                                                       std::placeholders::_4);

    ModuleStore::ModuleRemoverFunc dfunc = std::bind(&CModuleLoader::DeleteObject, this, std::placeholders::_1);

    BASE::AddModule(key, cfunc, dfunc, minfo); // strong exception guarantee
    if(handles_.count(sopath) == 0)
        handles_.emplace(sopath, handle);      // strong exception guarantee, but shouldn't ever throw
}



void CModuleLoader::LoadSO(const std::string & key,
                           const boost::python::dict & minfo)
{
    LoadSO_(key, ModuleInfo(minfo)); // conversion constructor for ModuleInfo
}




} // close namespace modulestore
} // close namespace bpmodule
