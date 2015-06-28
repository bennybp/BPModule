#include <dlfcn.h>

#include "BPModule/core/Output.hpp"
#include "BPModule/core/CModuleLoader.hpp"
#include "BPModule/core/ModuleStore.hpp"
#include "BPModule/core/ModuleBase.hpp"

namespace bpmodule {

namespace out = bpmodule::output;


CModuleLoader::CModuleLoader(ModuleStore * mst)
    : mst_(mst)
{
}



CModuleLoader::~CModuleLoader()
{
    DeleteAll();
    CloseHandles();
}



ModuleBase * CModuleLoader::CreateWrapper_(CreateFunc fn, const std::string & key,
                                           unsigned long id,
                                           const ModuleInfo & minfo)
{
    ModuleBase * newobj = fn(key, id, *mst_, minfo);
    objects_[id] = std::unique_ptr<ModuleBase>(newobj);
    return newobj;
}



void CModuleLoader::DeleteWrapper_(unsigned long id)
{
    DeleteObject_(id);
}



void CModuleLoader::LoadSO(const std::string & key,
                           const ModuleInfo & minfo)
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
        out::Debug("Looking to open so file: %1%\n", sopath);
        handle = dlopen(sopath.c_str(), RTLD_NOW | RTLD_GLOBAL);
        // open the module
        if(!handle)
            throw BPModuleException(
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
        throw BPModuleException(
                                 "Cannot find function in SO file",
                                 {
                                     { "File", sopath },
                                     { "Function", "CreateModule" },
                                     { "Error", error }
                                 }
                               );
    }

    if(handles_.count(sopath) == 0)
        handles_.insert(HandleMap::value_type(sopath, handle));

    out::Success("Successfully opened %1%\n", sopath);

    ModuleStore::ModuleGeneratorFunc cfunc = std::bind(&CModuleLoader::CreateWrapper_, this, fn,
                                                       std::placeholders::_1,
                                                       std::placeholders::_2,
                                                       std::placeholders::_3);

    ModuleStore::ModuleDeleterFunc dfunc = std::bind(&CModuleLoader::DeleteWrapper_, this, std::placeholders::_1);
    mst_->AddCreateFunc(key, cfunc, dfunc, minfo);
}



void CModuleLoader::DeleteObject_(unsigned long id)
{
    objects_.erase(id);
}



void CModuleLoader::DeleteAll(void)
{
    objects_.clear();
}



void CModuleLoader::CloseHandles(void)
{
    for(auto it : handles_)
    {
        out::Output("Closing %1%\n", it.first);
        dlclose(it.second);
    }
    handles_.clear();
}



} // close namespace bpmodule
