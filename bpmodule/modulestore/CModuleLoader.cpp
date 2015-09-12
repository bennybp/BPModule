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
#include "bpmodule/exception/ModuleLoadException.hpp"

using bpmodule::modulebase::ModuleBase;
using bpmodule::exception::ModuleLoadException;


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



ModuleBase * CModuleLoader::GeneratorWrapper_(GeneratorFunc fn,
                                              const std::string & name,
                                              unsigned long id,
                                              ModuleStore & mstore,
                                              const ModuleInfo & minfo)
{
    // Have the base ModuleLoaderBase class take ownership,
    // but return the ptr

    ModuleBase * newobj = fn(name, id, mstore, minfo);
    std::unique_ptr<ModuleBase> uptr(newobj);
    BASE::TakeObject(id, std::move(uptr));  // strong exception guarantee
    return newobj;
}



void CModuleLoader::LoadSO(const std::string & key, const boost::python::dict & minfo)
{
    ModuleInfo mi(minfo); // conversion constructor

    // trailing slash on path should have been added by python scripts
    std::string sopath = mi.path + mi.soname;

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
            throw ModuleLoadException("Cannot open SO file",
                                        sopath, key, mi.name, dlerror());
    }


    // get the pointer to the GeneratorModule function
    GeneratorFunc fn = reinterpret_cast<GeneratorFunc>(dlsym(handle, "CreateModule"));
    if((error = dlerror()) != NULL)
    {
        dlclose(handle);
        throw ModuleLoadException("Cannot find function in SO file",
                                    sopath, key, mi.name, dlerror());
    }


    output::Success("Successfully opened %1%\n", sopath);

    ModuleStore::ModuleGeneratorFunc cfunc = std::bind(&CModuleLoader::GeneratorWrapper_, this, fn,
                                                       std::placeholders::_1,
                                                       std::placeholders::_2,
                                                       std::placeholders::_3,
                                                       std::placeholders::_4);

    ModuleStore::ModuleRemoverFunc dfunc = std::bind(&CModuleLoader::DeleteObject, this, std::placeholders::_1);

    BASE::InsertModule(key, cfunc, dfunc, mi); // strong exception guarantee
    if(handles_.count(sopath) == 0)
        handles_.emplace(sopath, handle);      // strong exception guarantee, but shouldn't ever throw
}



} // close namespace modulestore
} // close namespace bpmodule
