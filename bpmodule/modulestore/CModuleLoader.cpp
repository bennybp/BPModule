/*! \file
 *
 * \brief Loading and storing of C modules (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <dlfcn.h>

#include <boost/python.hpp>

#include "bpmodule/modulestore/CModuleLoader.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/modulestore/ModuleStore.hpp"
#include "bpmodule/modulebase/ModuleBase.hpp"

using bpmodule::modulebase::ModuleBase;
using bpmodule::exception::GeneralException;


namespace bpmodule {
namespace modulestore {

CModuleLoader::CModuleLoader(ModuleStore * mst)
    : mst_(mst)
{
}



CModuleLoader::~CModuleLoader()
{
    UnloadAll();
    CloseHandles();
}



ModuleBase * CModuleLoader::CreateWrapper_(CreateFunc fn,
                                           const std::string & key,
                                           unsigned long id,
                                           ModuleStore & mstore,
                                           const ModuleInfo & minfo)
{
    ModuleBase * newobj = fn(key, id, mstore, minfo);
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

    if(handles_.count(sopath) == 0)
        handles_.insert(HandleMap::value_type(sopath, handle));

    output::Success("Successfully opened %1%\n", sopath);

    ModuleStore::ModuleGeneratorFunc cfunc = std::bind(&CModuleLoader::CreateWrapper_, this, fn,
                                                       std::placeholders::_1,
                                                       std::placeholders::_2,
                                                       std::placeholders::_3,
                                                       std::placeholders::_4);

    ModuleStore::ModuleRemoverFunc dfunc = std::bind(&CModuleLoader::DeleteWrapper_, this, std::placeholders::_1);
    mst_->AddModule(key, cfunc, dfunc, minfo);
}


void CModuleLoader::LoadSO(const std::string & key,
                           const boost::python::dict & minfo)
{
    LoadSO(key, ModuleInfo(minfo)); // conversion constructor for ModuleInfo
}


void CModuleLoader::DeleteObject_(unsigned long id)
{
    objects_.erase(id);
}



void CModuleLoader::UnloadAll(void)
{
    objects_.clear();
}



void CModuleLoader::CloseHandles(void)
{
    for(auto it : handles_)
    {
        output::Output("Closing %1%\n", it.first);
        dlclose(it.second);
    }
    handles_.clear();
}


} // close namespace modulestore
} // close namespace bpmodule
